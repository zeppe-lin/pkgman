//! \file pkgdb_root_test.cpp
//! \brief Alternate-root package database contract test.
//! \copyright See COPYING for license terms and COPYRIGHT for notices.

#include <cassert>
#include <string>

#include <libpkgcore/libpkgcore.h>

#include "argparser.h"
#include "pkgdb.h"

using namespace std;

int
main()
{
  char program[] = "pkgman";
  char rootOption[] = "--root=/mnt/target-root";
  char command[] = "isinst";
  char package[] = "target-package";
  char* argv[] = { program, rootOption, command, package };

  ArgParser parser( 4, argv );
  assert( parser.parse() );
  assert( parser.root() == "/mnt/target-root" );

  PkgDB db( parser.root() );
  const auto& packages = db.installedPackages();

  assert( libpkgcore_test::lock_root == parser.root() );
  assert( libpkgcore_test::open_root == parser.root() );
  assert( packages.find( "target-package" ) != packages.end() );
  assert( db.isInstalled( "target-package" ) );
  assert( db.getVersionRelease( "target-package" ) == "1.0-1" );

  return 0;
}
