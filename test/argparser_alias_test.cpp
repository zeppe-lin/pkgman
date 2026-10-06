//! \file argparser_alias_test.cpp
//! \brief ArgParser command-line alias tests.
//! \copyright See COPYING for license terms and COPYRIGHT for notices.

#include <cassert>
#include <string>

#include "argparser.h"

using namespace std;

static void
test_pkgadd_force_alias( const char* alias )
{
  char program[] = "pkgman";
  char command[] = "install";
  char package[] = "example";
  char option[ 4 ] = {};

  string value = alias;
  assert( value.size() < sizeof( option ) );
  value.copy( option, value.size() );

  char* argv[] = { program, command, option, package };
  ArgParser parser( 4, argv );

  assert( parser.parse() );
  assert( parser.commandID() == ArgParser::INSTALL );
  assert( parser.pkgaddArgs() == " -f" );
}

int
main()
{
  test_pkgadd_force_alias( "-f" );
  test_pkgadd_force_alias( "-i" );

  // Keep accepting the historical spelling even though it is not
  // documented or offered by shell completion.
  test_pkgadd_force_alias( "-fi" );

  return 0;
}
