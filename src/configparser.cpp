//! \file configparser.cpp
//! \brief Configuration Parser implementation.
//! \copyright See COPYING and COPYRIGHT files for corresponding information.

#include <filesystem>
#include <fstream>
#include <iostream>

#include "argparser.h"
#include "configparser.h"
#include "helpers.h"

namespace fs = std::filesystem;

using namespace std;
using namespace StringHelper;

ConfigParser::ConfigParser( const string&    configFile,
                            const ArgParser* parser ):
  m_configFile( configFile ),
  m_parser( parser ),
  m_rootList(),
  m_logFilePattern( "" ),
  m_writeLog( false ),
  m_logMode( OVERWRITE_MODE ),
  m_removeLogOnSuccess( false ),
  m_readmeMode( VERBOSE_README ),
  m_preferHigher( false ),
  m_useRegex( false ),
  m_runScripts( false ),
  m_runscriptCommand( "sh" ),
  m_makeCommand( PATH_PKGMK_BIN ),
  m_addCommand( PATH_PKGADD_BIN ),
  m_removeCommand( PATH_PKGRM_BIN )
{
}

bool
ConfigParser::parse()
{
  ifstream file( m_configFile );
  if ( ! file.is_open() )
    return false;

  string line;
  while ( getline( file, line ) )
    parseLine( stripWhiteSpace( getValueBefore( line, '#' ) ) );

  file.close();

  //! \note Command line argument \a writeLog overrides config
  if ( m_parser->writeLog() )
    m_writeLog = true;

  return true;
}

void
ConfigParser::addConfig( const string&  line,
                         bool           configSet,
                         bool           configPrepend )
{
  if ( configSet && startsWithNoCase( line, "pkgsrcdir" ) )
    m_rootList.clear();

  parseLine( line, configPrepend );
}

bool
ConfigParser::writeLog()
  const
{
  return m_writeLog;
}

ConfigParser::logMode_t
ConfigParser::logMode()
  const
{
  return m_logMode;
}

bool
ConfigParser::removeLogOnSuccess()
  const
{
  return m_removeLogOnSuccess;
}

string
ConfigParser::logFilePattern()
  const
{
  return m_logFilePattern;
}

const
rootList_t& ConfigParser::rootList()
  const
{
  return m_rootList;
}

ConfigParser::readmeMode_t
ConfigParser::readmeMode()
  const
{
  return m_readmeMode;
}

bool
ConfigParser::preferHigher()
  const
{
  return m_preferHigher;
}

bool
ConfigParser::useRegex()
  const
{
  return m_useRegex;
}

bool
ConfigParser::runScripts()
  const
{
  return m_runScripts;
}

string
ConfigParser::runscriptCommand()
  const
{
  return m_runscriptCommand;
}

string
ConfigParser::makeCommand()
  const
{
  return m_makeCommand;
}

string
ConfigParser::addCommand()
  const
{
  return m_addCommand;
}

string
ConfigParser::removeCommand()
  const
{
  return m_removeCommand;
}

void
ConfigParser::parseLine( const string& line, bool prepend )
{
  if ( line.empty() )
    return;

  string s = line;

  if ( startsWithNoCase( s, "pkgsrcdir" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 9, "" ) );
    string path = stripWhiteSpace( getValueBefore( s, ':' ) );
    string packages = getValue( s, ':' );

    if ( fs::is_directory( fs::path( path ) ) )
    {
      if ( prepend )
        m_rootList.push_front( make_pair( path, packages ) );
      else
        m_rootList.push_back(  make_pair( path, packages ) );
    }
    else
      cerr << "pkgman: [config error]: can't access " << path << endl;
  }

  else if ( startsWithNoCase( s, "writelog" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 8, "" ) );
    if ( s == "enabled" )
      m_writeLog = true;
    else if ( s == "disabled" )
      m_writeLog = false;
  }

  else if ( startsWithNoCase( s, "logmode" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 7, "" ) );
    if ( s == "overwrite" )
      m_logMode = OVERWRITE_MODE;
    else if ( s == "append" )
      m_logMode = APPEND_MODE;
  }

  else if ( startsWithNoCase( s, "logfile" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 7, "" ) );
    m_logFilePattern = s;
  }

  else if ( startsWithNoCase( s, "rmlog_on_success" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 16, "" ) );
    if ( s == "yes" )
      m_removeLogOnSuccess = true;
    else if ( s == "no" )
      m_removeLogOnSuccess = false;
  }

  else if ( startsWithNoCase( s, "readme" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 6, "" ) );
    if ( s == "verbose" )
      m_readmeMode = VERBOSE_README;
    else if ( s == "compact" )
      m_readmeMode = COMPACT_README;
    else if ( s == "disabled" )
      m_readmeMode = WITHOUT_README;
  }

  else if ( startsWithNoCase( s, "runscripts" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 10, "" ) );
    if ( s == "yes" )
      m_runScripts = true;
    else if ( s == "no" )
      m_runScripts = false;
  }

  else if ( startsWithNoCase( s, "preferhigher" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 12, "" ) );
    if ( s == "yes" )
      m_preferHigher = true;
    else if ( s == "no" )
      m_preferHigher = false;
  }

  else if ( startsWithNoCase( s, "useregex" ) )
  {
    s = stripWhiteSpace( s.replace( 0, 8, "" ) );
    if ( s == "yes" )
      m_useRegex = true;
    else if ( s == "no" )
      m_useRegex = false;
  }

  else if ( startsWithNoCase( s, "makecommand" ) )
    m_makeCommand = stripWhiteSpace( s.replace( 0, 11, "" ) );

  else if ( startsWithNoCase( s, "addcommand" ) )
    m_addCommand = stripWhiteSpace( s.replace( 0, 10, "" ) );

  else if ( startsWithNoCase( s, "removecommand" ) )
    m_removeCommand = stripWhiteSpace( s.replace( 0, 13, "" ) );

  else if ( startsWithNoCase( s, "runscriptcommand" ) )
    m_runscriptCommand = stripWhiteSpace( s.replace( 0, 16, "" ) );

  else
    cerr << "pkgman: [config error]: unknown key/value: " << s << endl;
}

// vim: sw=2 ts=2 sts=2 et cc=72 tw=70
// End of file.
