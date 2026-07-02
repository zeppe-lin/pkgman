//! \file pkgdb.cpp
//! \brief PkgDB implementation.
//! \copyright See COPYING for license terms and COPYRIGHT for notices.

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>

#include <libpkgcore/pkgcore.h>
#include <fnmatch.h>

#include "datafileparser.h"
#include "p_regex.h"
#include "pkgdb.h"
#include "helpers.h"

using namespace std;
using namespace StringHelper;

PkgDB::PkgDB( const string& root ):
  m_isLoaded( false ),
  m_root( root )
{
}

bool
PkgDB::isInstalled( const pkgname_t& name )
  const
{
  if ( ! load() )
    return false;

  return m_packages.find( name ) != m_packages.end();
}

string
PkgDB::getVersionRelease( const pkgname_t& name )
  const
{
  if ( ! load() )
    return "";

  const auto& pkg = m_packages.find( name );
  if ( pkg == m_packages.end() )
    return "";

  return pkg->second.version; /* version-release */
}

const std::map< pkgname_t, pkgutil::pkginfo_t >&
PkgDB::installedPackages()
{
  load();
  return m_packages;
}

void
PkgDB::getMatchingPackages( const string&                pattern,
                            map< pkgname_t, pkgutil::pkginfo_t >&  target,
                            bool                         useRegex )
  const
{
  if ( ! load() )
    return;

  if ( useRegex )
  {
    RegEx re( pattern );

    for (const auto& [name, info] : m_packages)
      if ( re.match( name ) )
        target[name] = info;
  }
  else
  {
    for (const auto& [name, info] : m_packages)
    {
      // jw: I assume fnmatch will be quite fast for "match all" (*),
      // so I didn't add a boolean to check for this explicitely
      if ( fnmatch( pattern.c_str(), name.c_str(), 0 ) == 0 )
        target[name] = info;
    }
  }
}

bool
PkgDB::load()
  const
{
  if ( m_isLoaded )
    return true;

  pkgutil util("pkgman");

  try
  {
    db_lock lock(m_root, false);
    util.db_open(m_root);
  }
  catch (const runtime_error& error)
  {
    cerr << "error: " << error.what() << endl;
    return false;
  }

  m_packages = util.getPackages();

  return m_isLoaded = true;
}
