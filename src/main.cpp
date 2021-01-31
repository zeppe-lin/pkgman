//! \file       main.cpp
//! \brief      Command-line utility of \a prt
//! \copyright  See LICENSE file for copyright and license details.

#include <cstdlib>
#include <iostream>

#include <signal.h>

#include "argparser.h"
#include "prt.h"
#include "signaldispatcher.h"
#include "process.h"

using namespace std;

static void inline die( const string& message, int rc=1 )
{
  cerr << "prt: " << message << endl;
  exit( rc );
}

static void inline dieman( const string& cmd )
{
  Process man( "/usr/bin/man", cmd, /*logfd*/ 0, /*log2stdout*/false );
  exit( man.execute() );
}

int main( int argc, char** argv )
{
  ArgParser parser( argc, argv );
  parser.parse();

  if ( parser.unknownOpt().size() )
    die( "unknown option: " + parser.unknownOpt() );

  if ( parser.unknownCmdOpt().size() )
    die( "unknown command option: " + parser.unknownCmdOpt() );

  if ( parser.verbose() > 2 )
    die( "can't specify both -v and -vv" );

  const string& cmd = parser.cmdName();
  if ( cmd.empty() )
    die( "no command given" );
  else if ( cmd == "version" )
    die( "v" VERSION, 0 );
  else if ( cmd == "help" || cmd == "--help" )
    dieman( "prt" );
  else if ( parser.isHelp() )
    dieman( "prt-" + cmd );

  if ( parser.isTest() )
    cout << "\n*** TEST MODE ***\n";

  Prt prt( &parser );
  //if ( prt.returnValue() == -1 )
  //  exit( EXIT_FAILURE );

  signal( SIGHUP,  SignalDispatcher::dispatch );
  signal( SIGINT,  SignalDispatcher::dispatch );
  signal( SIGQUIT, SignalDispatcher::dispatch );
  signal( SIGILL,  SignalDispatcher::dispatch );

  SignalDispatcher::instance()->registerHandler( &prt, SIGINT  );
  SignalDispatcher::instance()->registerHandler( &prt, SIGHUP  );
  SignalDispatcher::instance()->registerHandler( &prt, SIGQUIT );
  SignalDispatcher::instance()->registerHandler( &prt, SIGILL  );

  // some useful lambdas to avoid boilerplate

  auto argify = []( size_t count )
  {
    return  to_string( count ) +
          ( count > 1 ? " arguments" : " argument" );
  };

  auto assertMinArgCount = [ &parser, &argify ]( size_t count )
  {
    if ( parser.cmdArgs().size() < count )
      die( parser.cmdName() + " takes at least " +
                 argify( count ) );
  };

  auto assertMaxArgCount = [ &parser, &argify ]( size_t count )
  {
    if ( parser.cmdArgs().size() > count )
      die( parser.cmdName() + " takes at most "  +
                 argify( count ) );
  };

  auto assertExactArgCount = [ &parser, &argify ]( size_t count )
  {
    if ( parser.cmdArgs().size() != count )
      die( parser.cmdName() + " takes exactly "  +
                 argify( count ) );
  };

  /////////////////////////////////////////////////////////////////////
  /////////////////////// Prt Commands Handling ///////////////////////
  /////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////
  //                          Informational                          //
  /////////////////////////////////////////////////////////////////////
  if ( cmd == "dumpconfig" )
  {
    prt.dumpConfig();
  }
  else if ( cmd == "list" )
  {
    prt.listPackages();
  }
  else if ( cmd == "list-dup" )
  {
    assertMaxArgCount( 1 );
    prt.listShadowed();
  }
  else if ( cmd == "list-orphans" )
  {
    prt.listOrphans();
  }
  else if ( cmd == "list-locked" )
  {
    prt.listLocked();
  }
  else if ( cmd == "printf" )
  {
    assertExactArgCount( 1 );
    prt.printf();
  }
  else if ( cmd == "info" )
  {
    assertExactArgCount( 1 );
    prt.printInfo();
  }
  else if ( cmd == "readme" )
  {
    assertExactArgCount( 1 );
    prt.printReadme();
  }
  else if ( cmd == "path" )
  {
    assertExactArgCount( 1 );
    prt.printPath();
  }
  else if ( cmd == "isinst" )
  {
    assertMinArgCount( 1 );
    prt.printIsInstalled();
  }
  else if ( cmd == "current" )
  {
    assertExactArgCount( 1 );
    prt.printCurrentVersion();
  }

  /////////////////////////////////////////////////////////////////////
  //                 Differences / Check for updates                 //
  /////////////////////////////////////////////////////////////////////
  else if ( cmd == "diff" )
  {
    prt.printDiff();
  }
 
  /////////////////////////////////////////////////////////////////////
  //                          Dependencies                           //
  /////////////////////////////////////////////////////////////////////
  else if ( cmd == "mdep" )
  {
    prt.printMissingDep();
  }
  else if ( cmd == "dep" )
  {
    assertExactArgCount( 1 );
    prt.printDep();
  }
  else if ( cmd == "rdep" )
  {
    assertExactArgCount( 1 );
    prt.printRevDep();
  }

  /////////////////////////////////////////////////////////////////////
  //                            Searching                            //
  /////////////////////////////////////////////////////////////////////
  else if ( cmd == "search" )
  {
    assertExactArgCount( 1 );
    prt.psearch();
  }
  else if ( cmd == "dsearch" )
  {
    assertExactArgCount( 1 );
    prt.psearch( /*desc*/ true );
  }
  else if ( cmd == "fsearch" )
  {
    assertMinArgCount( 1 );
    prt.fsearch();
  }

  /////////////////////////////////////////////////////////////////////
  //                    Install / Update / Remove                    //
  /////////////////////////////////////////////////////////////////////
  else if ( cmd == "install" )
  {
    assertMinArgCount( 1 );
    prt.install( Transaction::INSTALL );
  }
  else if ( cmd == "update" )
  {
    assertMinArgCount( 1 );
    prt.install( Transaction::UPDATE );
  }
  else if ( cmd == "remove" )
  {
    assertMinArgCount( 1 );
    prt.remove();
  }

  /////////////////////////////////////////////////////////////////////
  //                          System update                          //
  /////////////////////////////////////////////////////////////////////
  else if ( cmd == "sync" )
  {
    prt.sync();
  }
  else if ( cmd == "sysup" )
  {
    prt.sysup();
  }
  else if ( cmd == "lock" )
  {
    assertMinArgCount( 1 );   // package name
    prt.setLock( /*lock*/ true );
  }
  else if ( cmd == "unlock" )
  {
    assertMinArgCount( 1 );
    prt.setLock( /*lock*/ false );
  }

  /////////////////////////////////////////////////////////////////////
  //                         File operations                         //
  /////////////////////////////////////////////////////////////////////
  else if ( cmd == "ls" )
  {
    assertExactArgCount( 1 ); // package name
    prt.ls();
  }
  else if ( cmd == "cat" )
  {
    assertMinArgCount( 1 );   // package name
    assertMaxArgCount( 2 );   // optional file
    prt.cat();
  }
  else if ( cmd == "edit" )
  {
    assertMinArgCount( 1 );   // package name
    assertMaxArgCount( 2 );   // optional file
    prt.edit();
  }

  /////////////////////////////////////////////////////////////////////
  else
    die( "unknown command " + cmd );

  if ( parser.isTest() )
    cout << "\n*** TEST MODE END ***\n";

  return prt.returnValue();
}

// vim:sw=2:ts=2:sts=2:et:cc=72
// End of file
