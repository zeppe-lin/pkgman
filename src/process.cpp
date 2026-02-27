//! \file process.cpp
//! \brief Process implementation.
//! \copyright See COPYING for license terms and COPYRIGHT for notices.

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <csignal>
#include <list>

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#include "process.h"
#include "helpers.h"

using namespace std;
using namespace StringHelper;

static int
exit_code_from_wait_status(int st)
{
  if (st < 0)
    return 1;

  if (WIFEXITED(st))
    return WEXITSTATUS(st);

  if (WIFSIGNALED(st))
    return 128 + WTERMSIG(st);

  // Should not happen, but don't report success.
  return 1;
}

std::atomic<pid_t> Process::s_active_pgid{0};

pid_t
Process::activePgid()
{
  return s_active_pgid.load();
}

void
Process::terminateActiveGroup(int signum)
{
  pid_t pgid = activePgid();

  if (pgid > 0)
    (void) kill(-pgid, signum);
}

// RAII guard: temporarily give foreground tty to a process group
class ForegroundPgrpGuard
{
  public:
    ForegroundPgrpGuard(pid_t new_pgrp)
      : m_tty(-1), m_old_pgrp(-1), m_active(false)
    {
      if (!isatty(STDIN_FILENO))
        return;

      m_tty = STDIN_FILENO;
      m_old_pgrp = tcgetpgrp(m_tty);
      if (m_old_pgrp < 0)
        return;

      // Avoid being stopped by SIGTTOU when calling tcsetpgrp().
      struct sigaction sa_old{}, sa_ign{};
      sa_ign.sa_handler = SIG_IGN;
      sigemptyset(&sa_ign.sa_mask);
      sa_ign.sa_flags = 0;
      sigaction(SIGTTOU, &sa_ign, &sa_old);

      if (tcsetpgrp(m_tty, new_pgrp) == 0)
        m_active = true;

      sigaction(SIGTTOU, &sa_old, nullptr);
    }

    ~ForegroundPgrpGuard()
    {
      if (!m_active)
        return;

      struct sigaction sa_old{}, sa_ign{};
      sa_ign.sa_handler = SIG_IGN;
      sigemptyset(&sa_ign.sa_mask);
      sa_ign.sa_flags = 0;
      sigaction(SIGTTOU, &sa_ign, &sa_old);

      (void) tcsetpgrp(m_tty, m_old_pgrp);

      sigaction(SIGTTOU, &sa_old, nullptr);
    }

  private:
    int m_tty;
    pid_t m_old_pgrp;
    bool m_active;
};

Process::Process( const string&  app,
                  const string&  arguments,
                  int            logfd,
                  bool           log2stdout ):
  m_app( app ),
  m_arguments( arguments ),
  m_logfd( logfd ),
  m_log2stdout( log2stdout )
{
}

int
Process::execute()
{
  list< string > args;
  split( m_arguments, ' ', args, /*startPos*/ 0, /*useEmpty*/ false );

  const size_t argc = 1 + args.size() + 1; // app, args, NULL

  char** argv = new char*[ argc ];

  size_t i = 0;
  argv[ i ] = const_cast< char* >( m_app.c_str() );
  for ( const auto& arg: args )
  {
    ++i;
    argv[ i ] = const_cast< char* >( arg.c_str() );
  }

  ++i;
  assert( i + 1 == argc );
  argv[ i ] = NULL;

  int status = m_logfd > 0 ? execLog( argc, argv )
                           : exec( argc, argv );

  delete [] argv;

  return exit_code_from_wait_status(status);
}

int
Process::executeShell( const char *shell )
{
  int status = m_logfd > 0 ? execShellLog( shell )
                           : execShell( shell );

  return exit_code_from_wait_status(status);
}

int
Process::exec( const size_t argc, char** argv )
{
  (void) argc;
  int status = 0;
  pid_t pid = fork();

  if ( pid == 0 )     // child process
  {
    // New process group for the job chain.
    (void) setpgid(0, 0);

    execv( m_app.c_str(), argv );
    _exit( EXIT_FAILURE );
  }
  else if ( pid < 0 ) // fork failed
    status = -1;
  else                // parent process
  {
    // Race-proof: enforce pgid from parent too.
    (void) setpgid(pid, pid);
    s_active_pgid.store(pid);

    // Make the job the foreground process group so CTRL+C hits
    // sudo/fakeroot/pkgmk.
    ForegroundPgrpGuard fg(pid);

    while (waitpid(pid, &status, 0) != pid)
    {
      if (errno == EINTR)
        continue;

      status = -1;
      break;
    }

    s_active_pgid.store(0);
  }

  return status;
}

int
Process::execLog( const size_t argc, char** argv )
{
  (void) argc;
  int status = 0;
  int fdpipe[ 2 ];

  pipe( fdpipe );

  // Make read end non-blocking to avoid deadlocks in logging loop.
  int flags = fcntl(fdpipe[0], F_GETFL, 0);
  if (flags >= 0)
    fcntl(fdpipe[0], F_SETFL, flags | O_NONBLOCK);

  pid_t pid = fork();
  if ( pid == 0 )     // child process
  {
    (void) setpgid(0, 0);

    close( fdpipe[ 0 ] );

    dup2( fdpipe[ 1 ], STDOUT_FILENO );
    dup2( fdpipe[ 1 ], STDERR_FILENO );

    execv( m_app.c_str(), argv );
    exit( EXIT_FAILURE );
  }
  else if ( pid < 0 ) // fork failed
    status = -1;
  else                // parent process
  {
    (void) setpgid(pid, pid);
    s_active_pgid.store(pid);

    ForegroundPgrpGuard fg(pid);

    close( fdpipe[ 1 ] );

    char readbuf[ 1024 ];
    int  wpval;

    while (true)
    {
      wpval = waitpid(pid, &status, WNOHANG);
      if (wpval == -1 && errno == EINTR)
        continue; // retry waitpid
      if (wpval != 0)
        break; // pid (done) or -1 (real error)

      while (true)
      {
        ssize_t bytes = read(fdpipe[0], readbuf, sizeof(readbuf)-1);

        if (bytes > 0)
        {
          readbuf[bytes] = 0;
          if (m_log2stdout)
          {
            printf("%s", readbuf);
            fflush(stdout);
            fflush(stderr);
          }
          write(m_logfd, readbuf, bytes);
          continue;
        }

        if (bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
          break; // nothing more available now

        break; // EOF or real error
      }
    }
    close( fdpipe[ 0 ] );

    if ( wpval != pid )
      status = -1;

    s_active_pgid.store(0);
  }

  return status;
}

int
Process::execShell( const char* shell )
{
  int status = 0;
  pid_t pid = fork();

  if ( pid == 0 )     // child process
  {
    (void) setpgid(0, 0);

    execl( shell, shell, "-c",
          ( m_app + " " + m_arguments ).c_str(), NULL );
    _exit( EXIT_FAILURE );
  }
  else if ( pid < 0 ) // fork failed
    status = -1;
  else                // parent process
  {
    (void) setpgid(pid, pid);
    s_active_pgid.store(pid);

    ForegroundPgrpGuard fg(pid);

    while (waitpid(pid, &status, 0) != pid)
    {
      if (errno == EINTR)
        continue;

      status = -1;
      break;
    }
    s_active_pgid.store(0);
  }

  return status;
}

int
Process::execShellLog( const char* shell )
{
  int status = 0;
  int fdpipe[ 2 ];

  pipe( fdpipe );

  // Make read end non-blocking to avoid deadlocks in logging loop.
  int flags = fcntl(fdpipe[0], F_GETFL, 0);
  if (flags >= 0)
    fcntl(fdpipe[0], F_SETFL, flags | O_NONBLOCK);

  pid_t pid = fork();
  if ( pid == 0 )     // child process
  {
    (void) setpgid(0, 0);

    close( fdpipe[ 0 ] );

    dup2( fdpipe[ 1 ], STDOUT_FILENO );
    dup2( fdpipe[ 1 ], STDERR_FILENO );

    execl( shell, shell, "-x", "-c",
          ( m_app + " " + m_arguments ).c_str(), NULL );
    _exit( EXIT_FAILURE );
  }
  else if ( pid < 0 ) // fork failed
    status = -1;
  else                // parent process
  {
    (void) setpgid(pid, pid);
    s_active_pgid.store(pid);

    ForegroundPgrpGuard fg(pid);

    close( fdpipe[ 1 ] );

    char readbuf[ 1024 ];
    int  wpval;

    while (true)
    {
      wpval = waitpid(pid, &status, WNOHANG);
      if (wpval == -1 && errno == EINTR)
        continue; // retry waitpid
      if (wpval != 0)
        break; // pid (done) or -1 (real error)

      while (true)
      {
        ssize_t bytes = read(fdpipe[0], readbuf, sizeof(readbuf)-1);

        if (bytes > 0)
        {
          readbuf[bytes] = 0;
          if (m_log2stdout)
          {
            printf("%s", readbuf);
            fflush(stdout);
            fflush(stderr);
          }
          write(m_logfd, readbuf, bytes);
          continue;
        }

        if (bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
          break; // nothing more available now

        break; // EOF or real error
      }
    }
    close( fdpipe[ 0 ] );

    if ( wpval != pid )
      status = -1;

    s_active_pgid.store(0);
  }

  return status;
}

// vim: sw=2 ts=2 sts=2 et cc=72 tw=70
// End of file.
