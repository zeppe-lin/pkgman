#pragma once

#include <map>
#include <string>

namespace libpkgcore_test
{
inline std::string lock_root;
inline std::string open_root;
} // namespace libpkgcore_test

class db_lock
{
public:
  db_lock( const std::string& root, bool )
  {
    libpkgcore_test::lock_root = root;
  }
};

class pkgutil
{
public:
  struct pkginfo_t
  {
    std::string version;
  };

  explicit pkgutil( const std::string& )
  {
  }

  void db_open( const std::string& root )
  {
    libpkgcore_test::open_root = root;
  }

  std::map< std::string, pkginfo_t > getPackages()
  {
    pkginfo_t info;
    info.version = "1.0-1";
    return { { "target-package", info } };
  }
};
