//! \file       argparser.cpp
//! \brief      ArgParser class implementation
//! \copyright  See LICENSE file for copyright and license details.

#include "argparser.h"

using namespace std;

ArgParser::ArgParser( int argc, char** argv ):
  m_Help( false ),
  m_isForced( false ),
  m_isTest( false ),
  m_isAlternateConfigGiven( false ),
  m_hasFilter( false ),
  m_noStdConfig( false ),
  m_writeLog( false ),
  m_deps( false ),
  m_all( false ),
  m_full( false ),
  m_printPath( false ),
  m_execPreInstall( false ),
  m_execPostInstall( false ),
  m_execPreRemove( false ),
  m_execPostRemove( false ),
  m_useRegex( false ),
  m_recursive( false ),
  m_printTree( false ),
  m_group( false ),
  m_depSort( false ),
  m_alternateConfigFile( "" ),
  m_pkgmkArgs( "" ),
  m_pkgaddArgs( "" ),
  m_pkgrmArgs( "" ),
  m_sortArgs( "" ),
  m_filter( "" ),
  m_cmdName( "" ),
  m_unknownOpt( "" ),
  m_unknownCmdOpt( "" ),
  m_root( "" ),
  m_ignore( "" ),
  m_argc( argc ),
  m_argv( argv ),
  m_verbose( 0 )
{
}

void ArgParser::parse()
{
  if ( m_argc < 2 )
    return;

  int i = 1;

  // parse prt options
  for ( ; i < m_argc; i++ )
  {
    if ( m_argv[ i ][ 0 ] == '-' )
    {
      string opt = m_argv[ i ];

      if ( opt == "--no-std-config" )
        m_noStdConfig = true;

      else if ( opt.substr( 0, 9 ) == "--config=" )
      {
        m_alternateConfigFile = opt.substr( 9 );
        m_isAlternateConfigGiven = true;
      }

      else if ( opt.substr( 0, 16 ) == "--config-append=" )
        m_configData.push_back(
            make_pair( m_argv[ i ] + 16, CONFIG_APPEND ) );

      else if ( opt.substr( 0, 17 ) == "--config-prepend=" )
        m_configData.push_back(
            make_pair( m_argv[ i ] + 17, CONFIG_PREPEND ) );

      else if ( opt.substr( 0, 13 ) == "--config-set=" )
        m_configData.push_back(
            make_pair( m_argv[ i ] + 13, CONFIG_SET ) );

      else if ( opt.substr( 0, 7 ) == "--root=" )
        m_root = opt.substr( 7 );

      else
      {
        m_unknownOpt = opt;
        return;
      }
    }
    else
      break; // command found
  } // for

  // command
  m_cmdName = m_argv[ i++ ];
  //FIXME i++;

  // parse command and command options
  for ( ; i < m_argc; i++ )
  {
    if ( m_argv[ i ][ 0 ] == '-' )
    {
      string opt = m_argv[ i ];

      // options
      if ( opt == "-v" )
        m_verbose += 1;

      else if ( opt == "-vv" )
        m_verbose += 2;

      else if ( opt == "--help" )
        m_Help = true;

      else if ( opt == "--force" )
        m_isForced = true;

      else if ( opt == "--test" )
        m_isTest = true;

      else if ( opt == "--deps" )
        m_deps = true;

      else if ( opt == "--all" )
        m_all = true;

      else if ( opt == "--full" )
        m_full = true;

      else if ( opt == "--path" )
        m_printPath = true;

      else if ( opt == "--log" )
        m_writeLog = true;

      else if ( opt == "--pre-install" )
        m_execPreInstall = true;

      else if ( opt == "--post-install" )
        m_execPostInstall = true;

      else if ( opt == "--install-scripts" )
        m_execPreInstall = m_execPostInstall = true;

      else if ( opt == "--pre-remove" )
        m_execPreRemove = true;

      else if ( opt == "--post-remove" )
        m_execPostRemove = true;

      else if ( opt == "--remove-scripts" )
        m_execPreRemove = m_execPostRemove = true;

      else if ( opt == "--no-std-config" )
        m_noStdConfig = true;

      else if ( opt == "--regex" )
        m_useRegex = true;

      else if ( opt == "--recursive" )
        m_recursive = true;

      else if ( opt == "--tree" )
        m_printTree = true;

      else if ( opt == "--group" )
        m_group = true;

      else if ( opt == "--depsort" )
        m_depSort = true;

      // pkgadd arguments aliases
      else if ( opt == "-f" )
        m_pkgaddArgs += " -f";

      else if ( opt == "-fi" )
        m_pkgaddArgs += " -f";

      // pkgmk arguments aliases
      else if ( opt == "-d" )
        m_pkgmkArgs += " -d";

      else if ( opt == "-do" )
        m_pkgmkArgs += " -do";

      else if ( opt == "-fr" )
        m_pkgmkArgs += " -f";

      else if ( opt == "-if" )
        m_pkgmkArgs += " -if";

      else if ( opt == "-uf" )
        m_pkgmkArgs += " -uf";

      else if ( opt == "-im" )
        m_pkgmkArgs += " -im";

      else if ( opt == "-um" )
        m_pkgmkArgs += " -um";

      else if ( opt == "-is" )
        m_pkgmkArgs += " -is";

      else if ( opt == "-us" )
        m_pkgmkArgs += " -us";

      else if ( opt == "-kw" )
        m_pkgmkArgs += " -kw";

      else if ( opt == "-ns" )
        m_pkgmkArgs += " -ns";

      // substrings
      else if ( opt.substr( 0, 8 ) == "--margs=" )
        m_pkgmkArgs += " " + opt.substr( 8 );

      else if ( opt.substr( 0, 8 ) == "--aargs=" )
        m_pkgaddArgs += " " + opt.substr( 8 );

      else if ( opt.substr( 0, 8 ) == "--rargs=" )
        m_pkgrmArgs = opt.substr( 8 );

      else if ( opt.substr( 0, 7 ) == "--sort=" )
        m_sortArgs = opt.substr( 7 );

      else if ( opt.substr( 0, 9 ) == "--filter=" )
      {
        m_filter = opt.substr( 9 );
        m_hasFilter = true;
      }

      else if ( opt.substr( 0, 9 ) == "--ignore=" )
        m_ignore = opt.substr( 9 );

      else
      {
        m_unknownCmdOpt = opt;
        return;
      }
    }
    else
      m_cmdArgs.push_back(m_argv[i]);
  } // for
}

bool ArgParser::isHelp() const
{
  return m_Help;
}

bool ArgParser::isForced() const
{
  return m_isForced;
}

bool ArgParser::isTest() const
{
  return m_isTest;
}

bool ArgParser::isAlternateConfigGiven() const
{
  return m_isAlternateConfigGiven;
}

bool ArgParser::writeLog() const
{
  return m_writeLog;
}

bool ArgParser::hasFilter() const
{
  return m_hasFilter;
}

bool ArgParser::noStdConfig() const
{
  return m_noStdConfig;
}

bool ArgParser::deps() const
{
  return m_deps;
}

bool ArgParser::all() const
{
  return m_all;
}

bool ArgParser::full() const
{
  return m_full;
}

bool ArgParser::printPath() const
{
  return m_printPath;
}

bool ArgParser::execPreInstall() const
{
  return m_execPreInstall;
}

bool ArgParser::execPostInstall() const
{
  return m_execPostInstall;
}

bool ArgParser::execPreRemove() const
{
  return m_execPreRemove;
}

bool ArgParser::execPostRemove() const
{
  return m_execPostRemove;
}

bool ArgParser::useRegex() const
{
  return m_useRegex;
}

bool ArgParser::recursive() const
{
  return m_recursive;
}

bool ArgParser::printTree() const
{
  return m_printTree;
}

bool ArgParser::group() const
{
  return m_group;
}

bool ArgParser::depSort() const
{
  return m_depSort;
}

const string& ArgParser::alternateConfigFile() const
{
  return m_alternateConfigFile;
}

const string& ArgParser::pkgmkArgs() const
{
  return m_pkgmkArgs;
}

const string& ArgParser::pkgaddArgs() const
{
  return m_pkgaddArgs;
}

const string& ArgParser::pkgrmArgs() const
{
  return m_pkgrmArgs;
}

const string& ArgParser::sortArgs() const
{
  return m_sortArgs;
}

const string& ArgParser::filter() const
{
  return m_filter;
}

const string& ArgParser::root() const
{
  return m_root;
}

const string& ArgParser::ignore() const
{
  return m_ignore;
}

const string& ArgParser::cmdName() const
{
  return m_cmdName;
}

const string& ArgParser::unknownOpt() const
{
  return m_unknownOpt;
}

const string& ArgParser::unknownCmdOpt() const
{
  return m_unknownCmdOpt;
}

const list< char* >& ArgParser::cmdArgs() const
{
  return m_cmdArgs;
}

int ArgParser::verbose() const
{
  return m_verbose;
}

const
list< pair< char*, ArgParser::configArg_t > > ArgParser::configData()
  const
{
  return m_configData;
}

// vim:sw=2:ts=2:sts=2:et:cc=72
// End of file
