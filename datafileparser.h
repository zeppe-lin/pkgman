//! \file      datafilepatser.h
//! \brief     DataFileParser Definition
//! \copyright See LICENSE file for copyright and license details.

#pragma once

#include <map>
#include <string>

using namespace std;

//! \class  DataFileParser
//! \brief  Data \cFile Parser
//
//! Parser for files of the format "key:value1,value2"
class DataFileParser
{
public:
  //! \brief   Parse a file of the format "key:value1,value2" into std::map
  //!
  //! \param   filename  path to the file
  //! \param   target    save key/value into target[key]=value
  //!
  //! \return  \a true if parsing is ok, \a false otherwise
  bool parse( const string& filename, map< string, string >& target );
};

// vim:sw=2:ts=2:sts=2:et:cc=80
// End of file
