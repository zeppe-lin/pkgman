////////////////////////////////////////////////////////////////////////
// FILE:        configuration.cpp
// AUTHOR:      Johannes Winkelmann, jw@tks6.net
// COPYRIGHT:   (c) 2002 by Johannes Winkelmann
// ---------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdio>
#include <dirent.h>

#include "configuration.h"
#include "stringhelper.h"
#include "argparser.h"

using namespace std;
using namespace StringHelper;

Configuration::Configuration( const std::string& configFile,
                              const ArgParser* parser )
    : m_configFile( configFile ),
      m_parser( parser ),
      m_cacheFile( "" ),
      m_rootList(),
      m_logFilePattern( "" ),
      m_writeLog( false ),
      m_appendLog( false ),
      m_removeLogOnSuccess( false ),
      m_readmeMode( VERBOSE_README ),
      m_runScripts( false ),
      m_preferHigher( false ),
      m_useRegex( false ),
      m_makeCommand( "" ), m_addCommand( "" ),
      m_removeCommand( "" ), m_runscriptCommand( "" )
{

}


bool Configuration::parse()
{
    FILE* fp = fopen( m_configFile.c_str(), "r" );
    if ( !fp ) {
        return false;
    }

    const int length = BUFSIZ;
    char input[length];
    string s;
    while ( fgets( input, length, fp ) ) {
        s = input;
        s = getValueBefore( s, '#' );
        s = stripWhiteSpace( s );
        parseLine(s);
    }

    fclose( fp );

    // command line arguments override config:
    if ( m_parser->writeLog() ) {
        m_writeLog = true;
    }

    return true;
}

bool Configuration::writeLog() const
{
    return m_writeLog;
}

bool Configuration::appendLog() const
{
    return m_appendLog;
}

bool Configuration::removeLogOnSuccess() const
{
    return m_removeLogOnSuccess;
}

string Configuration::logFilePattern() const
{
    return m_logFilePattern;
}


const list< pair<string, string> >& Configuration::rootList() const
{
    return m_rootList;
}

Configuration::ReadmeMode Configuration::readmeMode() const
{
    return m_readmeMode;
}

std::string Configuration::cacheFile() const
{
    return m_cacheFile;
}


void Configuration::addConfig(const string& line,
                              bool configSet,
                              bool configPrepend)
{
    if (configSet && startsWithNoCase( line, "prtdir" )) {
        m_rootList.clear();
    }
    parseLine(line, configPrepend);
}

/*!
  parse a line and set the configuration data accordingly; if \a
  prepend is set, prepend the data for configuration options which are lists
 */
void Configuration::parseLine(const string& line, bool prepend)
{
    string s = line;
    if ( s.empty() ) {
        return;
    }

    if ( startsWithNoCase( s, "prtdir" ) ) {
        s = stripWhiteSpace( s.replace( 0, 6, "" ) );
        string path = stripWhiteSpace( getValueBefore( s, ':' ) );
        string packages = getValue( s, ':' );
        DIR* dir = opendir( path.c_str() );
        if ( dir ) {
            closedir( dir );

            if (prepend) {
                m_rootList.push_front( make_pair( path, packages ) );
            } else {
                m_rootList.push_back( make_pair( path, packages ) );
            }
        } else {
            cerr << "[Config error: can't access " << path
                 << "]" << endl;
        }
    } else if ( startsWithNoCase( s, "cachefile" ) ) {
        s = stripWhiteSpace( s.replace( 0, 9, "" ) );
        m_cacheFile = s;
    } else if ( startsWithNoCase( s, "writelog" ) ) {
        s = stripWhiteSpace( s.replace( 0, 8, "" ) );
        if ( s == "enabled" ) {
            // it's already set to false, so we can just enable it.
            // like this, the command line switch works as well
            m_writeLog = true;
        }
    } else if ( startsWithNoCase( s, "logfile" ) ) {
        s = stripWhiteSpace( s.replace( 0, 7, "" ) );
        m_logFilePattern = s;
    } else if ( startsWithNoCase( s, "logmode" ) ) {
        s = stripWhiteSpace( s.replace( 0, 7, "" ) );
        if ( s == "append" ) {
            m_appendLog = true;
        }
    } else if ( startsWithNoCase( s, "rmlog_on_success" ) ) {
        s = stripWhiteSpace( s.replace( 0, 16, "" ) );
        if ( s == "yes" ) {
            m_removeLogOnSuccess = true;
        }
    } else if ( startsWithNoCase( s, "readme" ) ) {
        s = stripWhiteSpace( s.replace( 0, 6, "" ) );
        if ( s == "compact" ) {
            m_readmeMode = COMPACT_README;
        } else if ( s == "disabled" ) {
            m_readmeMode = NO_README;
        }
    } else if ( startsWithNoCase( s, "runscripts" ) ) {
        s = stripWhiteSpace( s.replace( 0, 10, "" ) );
        if ( s == "yes" ) {
            m_runScripts = true;
        }
    } else if ( startsWithNoCase( s, "preferhigher" ) ) {
        s = stripWhiteSpace( s.replace( 0, 12, "" ) );
        if ( s == "yes" ) {
            m_preferHigher = true;
        }
    } else if ( startsWithNoCase( s, "useregex" ) ) {
        s = stripWhiteSpace( s.replace( 0, 8, "" ) );
        if ( s == "yes" ) {
            m_useRegex = true;
        }
    } else if ( startsWithNoCase( s, "makecommand" ) ) {
        m_makeCommand = stripWhiteSpace( s.replace( 0, 11, "" ) );
    } else if ( startsWithNoCase( s, "addcommand" ) ) {
        m_addCommand = stripWhiteSpace( s.replace( 0, 10, "" ) );
    } else if ( startsWithNoCase( s, "removecommand" ) ) {
        m_removeCommand = stripWhiteSpace( s.replace( 0, 13, "" ) );
    } else if ( startsWithNoCase( s, "runscriptcommand" ) ) {
        m_runscriptCommand = stripWhiteSpace( s.replace( 0, 16, "" ) );
    }
}

bool Configuration::runScripts() const
{
    return m_runScripts;
}

std::string Configuration::makeCommand() const
{
    return m_makeCommand;
}

std::string Configuration::addCommand() const
{
    return m_addCommand;
}

std::string Configuration::removeCommand() const
{
    return m_removeCommand;
}

std::string Configuration::runscriptCommand() const
{
    return m_runscriptCommand;
}

bool Configuration::preferHigher() const
{
    return m_preferHigher;
}

bool Configuration::useRegex() const
{
    return m_useRegex;
}


