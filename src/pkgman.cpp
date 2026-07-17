//! \file pkgman.cpp
//! \brief Pkgman main commands implementation.
//! \copyright See COPYING for license terms and COPYRIGHT for notices.

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <string>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <libgen.h>
#include <fnmatch.h>

#include "argparser.h"
#include "configparser.h"
#include "datafileparser.h"
#include "helpers.h"
#include "transaction.h"
#include "locker.h"
#include "process.h"
#include "pkgman.h"
#include "p_regex.h"
#include "pkgmksetting.h"
#include "repository.h"
#include "versioncomparator.h"
#include "helpers.h"

namespace fs = std::filesystem;

using namespace std;
using namespace StringHelper;
using namespace ListHelper;

using VersionComparator::COMP_RESULT;
using VersionComparator::GREATER;
using VersionComparator::LESS;
using VersionComparator::EQUAL;
using VersionComparator::UNDEFINED;

Pkgman::Pkgman( const ArgParser* parser, pkgstate::store& state ):
  m_parser( parser ),
  m_repo( 0 ),
  m_config( 0 ),
  m_locker(),
  m_currentTransaction( 0 ),
  m_returnValue( P_OK ),
  m_state( state )
{
  m_locker = new Locker( m_parser->root() );

  readConfig();

  m_useRegex = ( m_config->useRegex() || m_parser->useRegex() );
}

Pkgman::~Pkgman()
{
  if ( m_config )
    delete m_config;

  if ( m_repo )
    delete m_repo;

  delete m_locker;
}

void
Pkgman::dumpConfig()
  const
{
  PkgmkSetting pkgmkConf( m_parser );

  if ( ! m_parser->noStdConfig() )
  {
    string configFile = m_parser->root() + SYSCONFDIR"/pkgman.conf";
    if ( m_parser->isAlternateConfigGiven() )
      configFile = m_parser->alternateConfigFile();

    std::printf( "%-20s: %s\n", "Configuration file",
                 configFile.c_str() );
  }

  if ( m_config->makeCommand().size() )
    std::printf( "%-20s: %s\n", "Make command",
                 m_config->makeCommand().c_str() );

  if ( m_config->addCommand().size() )
    std::printf( "%-20s: %s\n", "Add command",
                 m_config->addCommand().c_str() );

  if ( m_config->removeCommand().size() )
    std::printf( "%-20s: %s\n", "Remove command",
                 m_config->removeCommand().c_str() );

  std::printf( "%-20s: %s\n", "Run scripts",
               m_config->runScripts() ? "yes" : "no" );

  if ( m_config->runscriptCommand().size() )
    std::printf( "%-20s: %s\n", "Runscript command",
                 m_config->runscriptCommand().c_str() );

  std::printf( "%-20s: %s\n", "Keep higher version",
               m_config->preferHigher() ? "yes" : "no" );

  std::printf( "%-20s: %s\n", "Readme mode",
    m_config->readmeMode() == ConfigParser::VERBOSE_README ? "verbose" :
    m_config->readmeMode() == ConfigParser::COMPACT_README ? "compact" :
    m_config->readmeMode() == ConfigParser::WITHOUT_README ? "off"     :
                                                             ""       );

  if ( m_config->writeLog() )
  {
    std::printf( "%-20s: yes\n", "Write log" );

    if ( m_config->logFilePattern().size() )
      std::printf( "%-20s: %s\n", "  Log file",
                   m_config->logFilePattern().c_str() );
  }
  else
    std::printf( "%-20s: no\n", "Write log" );

  std::printf( "%-20s:\n", "Pkgmk settings" );
  std::printf( "%-20s: %s\n", "  Package dir",
               pkgmkConf.get( "PKGMK_PACKAGE_DIR" ).c_str() );
  std::printf( "%-20s: %s\n", "  Compression mode",
               pkgmkConf.get( "PKGMK_COMPRESSION_MODE" ).c_str() );

  std::printf( "%-20s: %lu\n", m_config->rootList().size() == 1
                              ? "Packages sources directory"
                              : "Packages sources directories",
               m_config->rootList().size() );

  for ( const auto& [ path, packages ]: m_config->rootList() )
  {
    if ( packages.size() )
      std::printf( "  %-20s: %s\n", path.c_str(), packages.c_str() );
    else
      std::printf( "  %-20s\n", path.c_str() );
  }
}

void
Pkgman::listPackages()
{
  const auto installed = m_state.read();

  string filter = m_useRegex ? "." : "*";
  if ( m_parser->hasFilter() )
    filter = m_parser->filter();

  PackageNames packages;

  initRepo();

  if ( m_parser->all() )
  {
    list< Package* > pkglist;
    m_repo->getMatchingPackages( filter, pkglist );

    if ( pkglist.empty() )
    {
      if ( m_parser->cmdArgs().size() )
        return errx( "no matching packages found" );
      else
        return errx( "no packages found" );
    }

    cout << "-- list ([i] = installed)" << endl;

    if ( m_parser->depSort() )
    {
      for ( const auto& pkg: pkglist )
        packages.push_back( pkg->name() );

      return listDepSorted( std::move( packages ), installed );
    }

    for ( const auto& pkg: pkglist )
    {
      cout << ( isInstalled( installed, pkg->name() ) ? "[i] " : "[ ] " );
      briefInfo( pkg );
    }
  }
  else
  {
    expandInstalledPatterns( PackageNames{ filter }, installed, packages );

    if ( packages.empty() )
    {
      if ( m_parser->cmdArgs().size() )
        return errx( "no matching packages found" );
      else
        return errx( "no packages found" );
    }

    if ( m_parser->depSort() )
      return listDepSorted( std::move( packages ), installed );

    for ( const auto& name: packages )
      briefInfo( name, installed );
  }
}

void
Pkgman::listShadowed() // aka listDup
{
  string format = "%p1 %v1 %c %p2 %v2\n";
  if ( m_parser->cmdArgs().size() )
    format =  m_parser->cmdArgs().front();

  vector< string > compare{ "<", ">", "==", "!=" };

  initRepo( /*listDuplicate*/ true );

  for ( const auto& [ p2, p1 ]: m_repo->shadowedPackages() )
  {
    string output = format;
    const auto& idx =
      compareVersions( p1->version_release(), p2->version_release() );

    replaceAll( output, "%n",  p1->name()            );
    replaceAll( output, "%p1", p1->fullpath()        );
    replaceAll( output, "%p2", p2->fullpath()        );
    replaceAll( output, "%v1", p1->version_release() );
    replaceAll( output, "%v2", p2->version_release() );
    replaceAll( output, "\\n", "\n"                  );
    replaceAll( output, "\\t", "\t"                  );
    replaceAll( output, "%c",  compare[ idx ]        );

    cout << output;
  }
}

void
Pkgman::listNodependents()
{
  const auto installed = m_state.read();
  initRepo();

  map< pkgname_t, bool > required;

  for ( const auto& package: installed.packages() )
  {
    const auto& name = package.identity().name();
    const auto& pkg = m_repo->getPackage( name );
    if ( ! pkg )
      continue;

    list< string > dependencies;
    split( pkg->dependencies(), ',', dependencies );

    for ( const auto& dep: dependencies )
      required[ dep ] = true;
  }

  for ( const auto& package: installed.packages() )
  {
    const auto& name = package.identity().name();
    if ( required[ name ] )
      continue;

    briefInfo( name, installed );
  }
}

void
Pkgman::listLocked()
{
  const auto installed = m_state.read();

  if ( m_locker->openFailed() )
    return errx( "failed to open locker data file: " +
                 m_parser->root() + SHAREDSTATEDIR"/pkg/locked" );

  if ( m_locker->lockedPackages().empty() )
    return;

  for ( const auto& name: m_locker->lockedPackages() )
    briefInfo( name, installed );
}

void
Pkgman::listOrphans()
{
  const auto installed = m_state.read();
  initRepo();

  PackageNames packages = installedPackageNames( installed );

  if ( packages.empty() )
    return errx( "no packages found" );

  Transaction depcalc( std::move( packages ), m_repo, installed, m_parser,
                       m_config, m_locker );

  if ( depcalc.calcDeps() != Transaction::SUCCESS )
      return errx( depcalc.strerror(), depcalc.result() );

  for ( const auto& [ package1, package2 ]: depcalc.missing() )
  {
    if ( package2.empty() )
      cout << package1 << endl;
    else if ( m_parser->dependents() )
      cout << package1 << " (required by " << package2 << ")" << endl;
  }
}

void
Pkgman::printf()
{
  const auto installed = m_state.read();
  const string formatString = m_parser->cmdArgs().front();

  string sortString = stripWhiteSpace( m_parser->sortArgs() );
  sortString += "%n"; // make it unique

  string filter = m_useRegex ? "." : "*";
  if ( m_parser->hasFilter() )
    filter = m_parser->filter();

  map< string, string > sortedOutput;

  initRepo();

  list< Package* > pkglist;
  m_repo->getMatchingPackages( filter, pkglist );

  if ( pkglist.empty() )
  {
    m_returnValue = P_GENERAL_ERROR;
    return;
  }

  for ( const auto& pkg: pkglist )
  {
    string output = formatString;
    string sortkey = sortString;

    replaceAll( output, "%n", pkg->name()          );
    replaceAll( output, "%u", pkg->url()           );
    replaceAll( output, "%p", pkg->path()          );
    replaceAll( output, "%v", pkg->version()       );
    replaceAll( output, "%r", pkg->release()       );
    replaceAll( output, "%d", pkg->description()   );
    replaceAll( output, "%e", pkg->dependencies()  );
    replaceAll( output, "%P", pkg->packager()      );
    replaceAll( output, "%M", pkg->maintainer()    );
    replaceAll( output, "\\t", "\t"                );
    replaceAll( output, "\\n", "\n"                );

    replaceAll( sortkey, "%n", pkg->name()         );
    replaceAll( sortkey, "%u", pkg->url()          );
    replaceAll( sortkey, "%p", pkg->path()         );
    replaceAll( sortkey, "%v", pkg->version()      );
    replaceAll( sortkey, "%r", pkg->release()      );
    replaceAll( sortkey, "%d", pkg->description()  );
    replaceAll( sortkey, "%e", pkg->dependencies() );
    replaceAll( sortkey, "%P", pkg->packager()     );
    replaceAll( sortkey, "%M", pkg->maintainer()   );

    string isInst = "no";
    if ( isInstalled( installed, pkg->name() ) )
    {
      auto result = compareVersions(
          installedVersion( installed, pkg->name() ),
          pkg->version_release() );
      isInst = result == EQUAL ? "yes" : "diff";
    }
    replaceAll( output,  "%i", isInst );
    replaceAll( sortkey, "%i", isInst );

    string isLocked = m_locker->isLocked( pkg->name() ) ? "yes" : "no";
    replaceAll( output,  "%l", isLocked );
    replaceAll( sortkey, "%l", isLocked );

    string hasReadme = pkg->hasReadme() ? "yes" : "no";
    replaceAll( output,  "%R", hasReadme );
    replaceAll( sortkey, "%R", hasReadme );

    string hasPreInstall = pkg->hasPreInstall() ? "yes" : "no";
    replaceAll( output,  "%A", hasPreInstall );
    replaceAll( sortkey, "%A", hasPreInstall );

    string hasPostInstall = pkg->hasPostInstall() ? "yes" : "no";
    replaceAll( output,  "%B", hasPostInstall );
    replaceAll( sortkey, "%B", hasPostInstall );

    string hasPreRemove = pkg->hasPreRemove() ? "yes" : "no";
    replaceAll( output,  "%C", hasPreRemove );
    replaceAll( sortkey, "%C", hasPreRemove );

    string hasPostRemove = pkg->hasPostRemove() ? "yes" : "no";
    replaceAll( output,  "%D", hasPostRemove );
    replaceAll( sortkey, "%D", hasPostRemove );

    sortedOutput[ sortkey ] = output;
  }

  for ( const auto& [ sortkey, output ]: sortedOutput )
    if ( stripWhiteSpace( output ).size() )
      cout << output;
}

void
Pkgman::printInfo()
{
  string arg = m_parser->cmdArgs().front();

  initRepo();

  const auto& pkg = m_repo->getPackage( arg );
  if ( ! pkg )
    return errx( "package '" + arg + "' not found" );

  cout << "Name:         " << pkg->name()    << endl
       << "Path:         " << pkg->path()    << endl
       << "Version:      " << pkg->version() << endl
       << "Release:      " << pkg->release() << endl;

  if ( pkg->description().size() )
    cout << "Description:  " << pkg->description()  << endl;

  if ( pkg->url().size() )
    cout << "URL:          " << pkg->url()          << endl;

  if ( pkg->packager().size() )
    cout << "Packager:     " << pkg->packager()     << endl;

  if ( pkg->maintainer().size() )
    cout << "Maintainer:   " << pkg->maintainer()   << endl;

  if ( pkg->dependencies().size() )
  {
    list< string > deps;
    split(pkg->dependencies(), ',', deps);
    list< string >::iterator it = deps.begin();
    cout   << "Depends on:   " << *it++ << endl;
    for ( ; it != deps.end(); ++it)
      cout << "              " << *it   << endl;
  }

  // files that will not be shown in info
  list< string > skipFiles
  {
    "Pkgfile", ".footprint", ".signature", ".md5sum",
     /* .32bit, .nostrip */
  };

  list< string > Files;
  for ( const auto& file: fs::directory_iterator( pkg->fullpath() ) )
  {
    string filename = file.path().filename();

    if ( contains( skipFiles, filename ) )
      continue;

    Files.push_back( filename );
  }

  if ( Files.size() )
  {
    if ( Files.size() > 1 )
      Files.sort();

    list< string >::iterator it = Files.begin();
    cout   << "Files:        " << *it++ << endl;
    for ( ; it != Files.end(); ++it)
      cout << "              " << *it << endl;
  }

  if ( m_parser->verbose() > 0 && pkg->hasReadme() )
  {
    cout << "\n--- README ---\n";
    printReadme();
  }
}

void
Pkgman::printReadme()
{
  string arg = m_parser->cmdArgs().front();

  initRepo();

  const auto& pkg = m_repo->getPackage( arg );
  if ( ! pkg )
    return errx( "package '" + arg + "' not found" );

  if ( ! pkg->hasReadme() )
    return errx( "package '" + arg + "' has no README file" );

  if (!printFile(pkg->fullpath("README.md")))
    printFile(pkg->fullpath("README"));
}

void
Pkgman::printPath()
{
  string arg = m_parser->cmdArgs().front();

  initRepo();

  const auto& pkg = m_repo->getPackage( arg );
  if ( ! pkg )
    return errx( "package '" + arg + "' not found" );

  cout << pkg->fullpath() << endl;
}

void
Pkgman::printIsInstalled()
{
  const auto installed = m_state.read();

  for ( const auto& arg: packageNames( m_parser->cmdArgs() ) )
  {
    if ( ! isInstalled( installed, arg ) )
      return errx( "package '" + arg + "' is not installed" );

    cout << "package '" + arg + "' is installed" << endl;
  }
}

void
Pkgman::printCurrentVersion()
{
  const auto installed = m_state.read();
  const string arg = m_parser->cmdArgs().front();
  const auto* package = installed.find_package( arg );

  if ( ! package )
    return errx( "package '" + arg + "' not installed" );

  cout << package->identity().version() << endl;
}

void
Pkgman::printDiff()
{
  const auto installed = m_state.read();
  initRepo();

  PackageNames packages;

  if ( m_parser->cmdArgs().size() )
    expandInstalledPatterns( packageNames( m_parser->cmdArgs() ),
                          installed, packages );
  else
    packages = installedPackageNames( installed );

  if ( packages.empty() )
    return errx( "no packages found" );

  if ( packages.size() < m_parser->cmdArgs().size() )
    return errx( "no matching packages found" );

  if ( m_parser->deps() )
  {
    Transaction depcalc( packages, m_repo, installed, m_parser,
                         m_config, m_locker );

    if ( depcalc.calcDeps() != Transaction::SUCCESS )
      return errx( depcalc.strerror(), depcalc.result() );

    m_missingRepoPackages = depcalc.missing();
    diffCalc( depcalc.deps(), installed );
  }
  else
    diffCalc( packages, installed );

  if ( ! m_parser->full() )
  {
    for ( const auto& name: m_greaterVersionComp )
      cout << name << endl;
    return;
  }

  size_t diff_packages = 0, new_packages = 0;

  if ( m_greaterVersionComp.size() )
  {
    cout <<
    "\n-- Differences between installed packages and packages sources tree\n";

    printFormattedDiffLine( "Package", "Installed", "Available", false );
    cout << endl;

    for ( const auto& name: m_greaterVersionComp )
    {
      isInstalled( installed, name ) ? ++diff_packages : ++new_packages;

      printFormattedDiffLine(
          name,
          installedVersion( installed, name ),
          m_repo->getPackage( name )->version_release(),
          m_locker->isLocked( name ) );
    }
  }

  if ( m_undefinedVersionComp.size() )
  {
    cout << "\n-- Undecidable version differences "
         << "(use --config-append='preferhigher no')\n";

    printFormattedDiffLine( "Package", "Installed", "Available", false );
    cout << endl;

    for ( const auto& name: m_undefinedVersionComp )
      printFormattedDiffLine(
          name,
          installedVersion( installed, name ),
          m_repo->getPackage( name )->version_release(),
          m_locker->isLocked( name ) );
  }

  if ( m_missingRepoPackages.size() )
  {
    cout << "\n-- Packages which was not found in the packages sources tree\n";
    printFormattedDiffLine( "Package", "Installed", "Required by", false );
    cout << endl;

    for ( const auto& [ pkg1, pkg2 ]: m_missingRepoPackages )
      printFormattedDiffLine(
          pkg1, installedVersion( installed, pkg1 ), pkg2, false );
  }

  cout << endl;

  if ( ! diff_packages && ! new_packages )
  {
    cout << "No upgradeable differences found" << endl;
    return;
  }

  cout << "--" << endl;

  if ( diff_packages )
    cout << diff_packages << ( diff_packages > 1 ? " updates" : " update" );

  if ( new_packages )
  {
    if ( diff_packages )
      cout << ", ";

    cout << new_packages << ( new_packages > 1 ? " installs\n" : " install\n" );
  }
}

void
Pkgman::printDep()
{
  const auto installed = m_state.read();
  string arg = m_parser->cmdArgs().front();

  initRepo();

  const auto& pkg = m_repo->getPackage( arg );
  if ( ! pkg )
    return errx( "package '" + arg + "' not found" );

  if ( pkg->dependencies().empty() )
    return;

  if ( m_parser->printTree() )
  {
    if ( m_parser->full() || ! m_parser->recursive() )
      cout << "-- dependencies ([i] = installed)" << endl;
    else
      cout << "-- dependencies ([i] = installed, --> seen before)" << endl;

    cout << ( isInstalled( installed, arg ) ? "[i] " : "[ ] " );
    briefInfo( pkg );
    printDepTree( pkg, 2, installed );
  }
  else
    printDepList( pkg, 0, installed );
}

void
Pkgman::printRevDep()
{
  const auto installed = m_state.read();
  string arg = m_parser->cmdArgs().front();

  initRepo();

  if ( m_parser->printTree() )
  {
    if ( m_parser->full() || ! m_parser->recursive() )
      cout << "-- revdep ([i] = installed)" << endl;
    else
      cout << "-- revdep ([i] = installed, --> seen before)" << endl;

    cout << ( isInstalled( installed, arg ) ? "[i] " : "[ ] " );
    briefInfo( arg, installed );
    printRevDepTree( arg, 2, installed );
  }
  else
    printRevDepList( arg, 0, installed );
}

void
Pkgman::psearch( bool desc )
{
  const auto installed = m_state.read();
  string arg = m_parser->cmdArgs().front();

  initRepo();

  list< Package* > pkglist;
  m_repo->searchMatchingPackages( arg, pkglist, desc );

  if ( pkglist.empty() )
    return errx( "no matching packages found" );

  cout << "-- search ([i] = installed)" << endl;
  for ( const auto& pkg: pkglist )
  {
    cout << ( isInstalled( installed, pkg->name() ) ? "[i] " : "[ ] " );
    briefInfo( pkg );
  }
}

void
Pkgman::fsearch()
{
  const auto installed = m_state.read();
  string arg = "*";
  if ( m_parser->cmdArgs().size() == 1 )
    arg = m_parser->cmdArgs().front();

  initRepo();

  size_t found = 0;

  for ( const auto& pkg: m_repo->packages() )
  {
    list< string > matches;

    bool search = footprintGrep( pkg.second->fullpath( ".footprint" ),
                                 arg, matches, m_parser->full(),
                                 m_useRegex );

    if ( ! search || matches.empty() )
      continue;

    ++found;
    if ( found == 1 )
      cout << "-- fsearch ([i] = installed)" << endl;

    cout << ( isInstalled( installed, pkg.first ) ? "[i] " : "[ ] " );
    briefInfo( pkg.second );

    for ( const string& match: matches )
      cout << "    " << match << endl;
  }

  if ( ! found )
    errx( "Nothing found" );
}

void
Pkgman::install( Transaction::Transaction_t transaction_t )
{
  const PackageNames args = packageNames( m_parser->cmdArgs() );
  auto installed = m_state.read();
  PackageNames sortedList;

  bool update = transaction_t == Transaction::UPDATE;
  for ( const auto& pkg: args )
  {
    bool present = isInstalled( installed, pkg );

    if ( ! update && present )
    {
      if ( ! m_parser->isForced() )
        return errx( "package " + pkg + " is installed", P_INSTALL_ERROR );
    }
    else if ( update && ! present )
      return errx( "package " + pkg + " is not installed", P_INSTALL_ERROR );
  }

  initRepo();

  if ( m_parser->deps() )
  {
    PackageNames deps;

    Transaction depcalc( args, m_repo, installed, m_parser,
                         m_config, m_locker );

    if ( depcalc.calcDeps() != Transaction::SUCCESS )
      return errx( depcalc.strerror(), depcalc.result() );

    for ( const auto& dep: depcalc.deps() )
    {
      if ( m_parser->depSort() &&
           std::find( args.begin(), args.end(), dep ) != args.end() )
        sortedList.push_back( dep );

      if ( ! isInstalled( installed, dep ) )
        deps.push_back( dep );
    }

    if ( deps.size() )
    {
      Transaction tr( deps, m_repo, installed, m_parser,
                      m_config, m_locker );
      executeTransaction( tr, Transaction::INSTALL );

      // External pkgadd changed durable state.  Any following phase must
      // observe a new snapshot rather than the transaction's old view.
      installed = m_state.read();
    }

    if ( transaction_t != Transaction::UPDATE )
      return;
  }

  if ( m_parser->deps() && m_parser->depSort() && sortedList.size() )
  {
    Transaction tran( std::move( sortedList ), m_repo, installed, m_parser,
                      m_config, m_locker );
    executeTransaction( tran, transaction_t );
  }
  else
  {
    Transaction transact( args, m_repo, installed, m_parser,
                          m_config, m_locker );
    executeTransaction( transact, transaction_t );
  }
}

void
Pkgman::remove()
{
  const PackageNames args = packageNames( m_parser->cmdArgs() );
  const auto installed = m_state.read();

  initRepo();

  if ( m_parser->deps() )
  {
    PackageNames removable;
    for ( const auto& pkg: args )
    {
      removable.push_back( pkg );
      getRemovableDeps( pkg, removable, installed );
    }

    sort( removable.begin(), removable.end() );
    removable.erase( unique( removable.begin(), removable.end() ),
                     removable.end() );

    Transaction transaction( std::move( removable ), m_repo, installed,
                             m_parser, m_config, m_locker );
    executeTransaction( transaction, Transaction::REMOVE );
  }
  else
  {
    Transaction transaction( args, m_repo, installed, m_parser,
                             m_config, m_locker );
    executeTransaction( transaction, Transaction::REMOVE );
  }
}

void
Pkgman::sysup()
{
  auto installed = m_state.read();
  initRepo();

  PackageNames* target;
  PackageNames sortedList;
  PackageNames newList;

  diffCalc( installedPackageNames( installed ), installed );

  if ( m_greaterVersionComp.empty() )
    return errx( "System is up to date", P_OK );

  PackageNames different( m_greaterVersionComp.begin(),
                          m_greaterVersionComp.end() );
  Transaction depCalcSort( different, m_repo, installed, m_parser,
                           m_config, m_locker );

  if ( depCalcSort.calcDeps() != Transaction::SUCCESS )
    return errx( depCalcSort.strerror(), depCalcSort.result() );

  if ( m_parser->deps() )
    m_missingRepoPackages = depCalcSort.missing();

  for ( const auto& pkg: depCalcSort.deps() )
  {
    if ( m_parser->depSort() && contains( m_greaterVersionComp, pkg ) )
      sortedList.push_back( pkg );

    if ( m_parser->deps() && ! isInstalled( installed, pkg ) )
      newList.push_back( pkg );
  }

  if ( m_parser->deps() && newList.size() )
  {
    Transaction install( newList, m_repo, installed, m_parser,
                         m_config, m_locker );
    executeTransaction( install, Transaction::INSTALL );

    if ( install.result() != Transaction::SUCCESS && m_parser->group() )
      return;

    cout << endl;
    installed = m_state.read();
  }

  target = m_parser->depSort() ? &sortedList : &different;

  Transaction update( *target, m_repo, installed, m_parser,
                      m_config, m_locker );
  executeTransaction( update, Transaction::UPDATE );
}

void
Pkgman::setLock( bool lock )
{
  const auto installed = m_state.read();

  if ( lock )
    initRepo();

  for ( const auto& arg: packageNames( m_parser->cmdArgs() ) )
  {
    if ( lock )
    {
      if ( isInstalled( installed, arg ) )
      {
        if ( ! m_locker->lock( arg ) )
          errx( "already locked: " + arg );
      }
      else
        return errx( "package '" + arg + "' not found" );
    }
    else if ( ! m_locker->unlock( arg ) )
      return errx( "not locked previously: " + arg );
  }

  if ( ! m_locker->store() )
    errx( "failed to write lock data into " +
          m_parser->root() + SHAREDSTATEDIR"/pkg/locked" );
}

void
Pkgman::ls()
{
  string name = m_parser->cmdArgs().front();

  initRepo();

  const auto& pkg = m_repo->getPackage( name );
  if ( ! pkg )
    return errx( "package '" + name + "' is not found in the packages sources tree" );

  for ( const auto& file: fs::directory_iterator( pkg->fullpath() ) )
  {
    if ( m_parser->printPath() )
      cout << file.path().string() << endl;
    else
      cout << file.path().filename().string() << endl;
  }
}

void
Pkgman::cat()
{
  const auto& args = m_parser->cmdArgs();

  string name = args.front();
  string file = args.size() > 1 ? args.back() : "Pkgfile";

  initRepo();

  const auto& pkg = m_repo->getPackage( name );
  if ( ! pkg )
    return errx( "package '" + name + "' is not found in the packages sources tree" );

  if ( ! printFile( pkg->fullpath( file ) ) )
    errx( "file '" + file + "' not found" );
}

void
Pkgman::edit()
{
  const auto& args = m_parser->cmdArgs();

  string name = args.front();
  string file = args.size() > 1 ? args.back() : "Pkgfile";

  char* editor = getenv( "EDITOR" );
  if ( ! editor ) // XXX check if empty?
    return errx( "environment variable EDITOR not set" );

  initRepo();

  const auto& pkg = m_repo->getPackage( name );
  if ( ! pkg )
    return errx( "package '" + name + "' is not found in the packages sources tree" );

  Process proc( editor, pkg->fullpath( file ) );
  if ( ( m_returnValue = proc.executeShell() ) )
    errx( "error while execution the editor", m_returnValue );
}

SignalHandler::HandlerResult
Pkgman::handleSignal(int signal)
{
  // TODO: second argument could also be true:
  // TODO: kill installtransaction

  // Forward to active subprocess group as a fallback (in case we're
  // not using tty fg pgrp).
  Process::terminateActiveGroup(signal);

  cout << "pkgman: interrupted" << endl;
  if ( m_currentTransaction )
    evaluateResult( *m_currentTransaction, /*interrupted*/ true );
  return EXIT;
}

int
Pkgman::returnValue()
  const
{
  return m_returnValue;
}

///////////////////////////////////////////////////////////////////////
//                          Private methods                          //
///////////////////////////////////////////////////////////////////////

inline void
Pkgman::errx( const string& error_message, int ret )
{
  cerr << "pkgman: " << error_message << ".\n";
  m_returnValue = ret;
}

void
Pkgman::readConfig()
{
  if ( m_config )
    return; // don't initialize twice

  string configFile = SYSCONFDIR"/pkgman.conf";
  if ( m_parser->isAlternateConfigGiven() )
    configFile = m_parser->alternateConfigFile();

  m_config = new ConfigParser( configFile, m_parser );

  // warn instead of errx?
  if ( ! m_parser->noStdConfig() && ! m_config->parse() )
    return errx( "can't read config " + configFile + ": use defaults" );

  for ( const auto& [ line, arg ]: m_parser->configData() )
  {
    m_config->addConfig( line,
                         arg == ArgParser::CONFIG_SET,
                         arg == ArgParser::CONFIG_PREPEND );
  }
}

void
Pkgman::initRepo( bool listDuplicate )
{
  if ( m_repo )
    return;

  m_repo = new Repository( m_useRegex );
  m_repo->initFromFS( m_config->rootList(), listDuplicate );
}

PackageNames
Pkgman::packageNames( const list< char* >& names )
{
  PackageNames result;
  result.reserve( names.size() );
  for ( const auto* name: names )
    result.emplace_back( name );
  return result;
}

PackageNames
Pkgman::installedPackageNames( const pkgstate::snapshot& installed )
{
  PackageNames result;
  result.reserve( installed.size() );
  for ( const auto& package: installed.packages() )
    result.push_back( package.identity().name() );
  return result;
}

bool
Pkgman::isInstalled( const pkgstate::snapshot& installed,
                     const pkgname_t& name )
{
  return installed.find_package( name ) != nullptr;
}

string
Pkgman::installedVersion( const pkgstate::snapshot& installed,
                          const pkgname_t& name )
{
  const auto* package = installed.find_package( name );
  return package ? package->identity().version() : string();
}

void
Pkgman::expandInstalledPatterns( const PackageNames& patterns,
                              const pkgstate::snapshot& installed,
                              PackageNames& target )
  const
{
  for ( const auto& pattern: patterns )
  {
    if ( m_useRegex )
    {
      RegEx re( pattern );
      for ( const auto& package: installed.packages() )
      {
        const auto& name = package.identity().name();
        if ( re.match( name ) &&
             find( target.begin(), target.end(), name ) == target.end() )
          target.push_back( name );
      }
      continue;
    }

    for ( const auto& package: installed.packages() )
    {
      const auto& name = package.identity().name();
      if ( fnmatch( pattern.c_str(), name.c_str(), 0 ) == 0 &&
           find( target.begin(), target.end(), name ) == target.end() )
        target.push_back( name );
    }
  }
}

void
Pkgman::expandWildcardsRepo( const list< char* >&  in,
                             list< pkgname_t >&    target )
{
  for ( const auto& pattern: in )
  {
    list< Package* > pkglist;
    m_repo->getMatchingPackages( pattern, pkglist );

    for ( const auto& pkg: pkglist )
      target.push_back( pkg->name() );
  }
}

void
Pkgman::listDepSorted( PackageNames packages,
                       const pkgstate::snapshot& installed )
{
  sort( packages.begin(), packages.end() );
  packages.erase( unique( packages.begin(), packages.end() ), packages.end() );

  while ( packages.size() )
  {
    string name = packages.front();
    packages.erase( packages.begin() );

    Transaction depcalc( PackageNames{ name }, m_repo, installed,
                         m_parser, m_config, m_locker );
    if ( depcalc.calcDeps() != Transaction::SUCCESS )
      return errx( depcalc.strerror(), depcalc.result() );

    for ( const auto& dep: depcalc.deps() )
    {
      const auto found = find( packages.begin(), packages.end(), dep );
      if ( found == packages.end() )
        continue;

      if ( m_parser->all() )
        cout << ( isInstalled( installed, dep ) ? "[i] " : "[ ] " );

      briefInfo( dep, installed );
      packages.erase( found );
    }

    if ( m_parser->all() )
      cout << ( isInstalled( installed, name ) ? "[i] " : "[ ] " );

    briefInfo( name, installed );
  }
}

bool
Pkgman::printFile( const string& filepath )
{
  if ( ! fs::exists( fs::path( filepath ) ) )
    return false;

  char* pager = getenv( "PAGER" );
  if ( pager )
  {
    Process proc( pager, filepath );
    proc.executeShell();
  }
  else
  {
    ifstream file( filepath );
    if ( ! file.is_open() )
      return false;

    string line;
    while ( getline( file, line ) )
      cout << line << endl;

    file.close();
  }

  return true;
}

COMP_RESULT
Pkgman::compareVersions( const string& v1, const string& v2 )
{
  if ( v1 == v2 )
    return EQUAL;

  if ( m_config->preferHigher() )
    return VersionComparator::compareVersions( v1, v2 );

  if ( v1 != v2 )
    return GREATER;

  return LESS;
}

void
Pkgman::diffCalc( const PackageNames& packages,
                  const pkgstate::snapshot& installed )
{
  for ( const auto& name: packages )
  {
    const auto& pkg = m_repo->getPackage( name );
    if ( ! pkg )
    {
      m_missingRepoPackages.push_back( make_pair( name, "" ) );
      continue;
    }

    const auto result = compareVersions(
        pkg->version_release(), installedVersion( installed, name ) );

    if ( result == GREATER )
    {
      if ( ! m_locker->isLocked( name ) || m_parser->all() )
        m_greaterVersionComp.push_back( name );
    }
    else if ( result == UNDEFINED )
      m_undefinedVersionComp.push_back( name );
  }
}

void
Pkgman::printFormattedDiffLine( const string&  name,
                                const string&  versionInstalled,
                                const string&  versionAvailable,
                                bool           isLocked )
{
  cout.setf( ios::left, ios::adjustfield );
  cout.width( 32 );
  cout.fill( ' ' );
  // There should be at list one space after names longer than 32
  // chars for normal parsing.
  cout << ( ( name.size() > 32 ) ? ( name + ' ' ) : name );

  cout.width( 20 );
  cout.fill( ' ' );
  cout << versionInstalled;

  cout.width( 20 );
  cout.fill( ' ' );
  cout << versionAvailable << ( isLocked ? "locked\n" : "\n" );
}

void
Pkgman::printDepList( const Package* pkg, int level,
                      const pkgstate::snapshot& installed )
{
  if ( pkg->dependencies().empty() )
    return;

  list< string > deps;
  split( pkg->dependencies(), ',', deps );

  static map< pkgname_t, bool > shownMap;
  for ( const auto& dep: deps )
  {
    if ( ! m_parser->all() && ! isInstalled( installed, dep ) )
      continue;

    if ( shownMap[ dep ] )
      continue;

    cout << string( level, ' ' );
    briefInfo( dep, installed );

    if ( m_parser->recursive() )
    {
      const auto& depkg = m_repo->getPackage( dep );
      if ( depkg )
        printDepList( depkg, level + 1, installed );
    }

    if ( ! m_parser->full() )
      shownMap[ dep ] = true;
  }
}

void
Pkgman::printDepTree( const Package* pkg, int level,
                      const pkgstate::snapshot& installed )
{
  if ( pkg->dependencies().empty() )
    return;

  list< string > deps;
  split( pkg->dependencies(), ',', deps );

  static map< pkgname_t, bool > shownMap;
  for ( const auto& dep: deps )
  {
    if ( ! m_parser->all() && ! isInstalled( installed, dep ) )
      continue;

    cout << ( isInstalled( installed, dep ) ? "[i] " : "[ ] " )
         << string( level, ' ' );

    briefInfo( dep, installed, false );

    if ( shownMap[ dep ] )
    {
      cout << " -->\n";
      continue;
    }

    cout << endl;

    if ( m_parser->recursive() )
    {
      const auto& depkg = m_repo->getPackage( dep );
      if ( depkg )
        printDepTree( depkg, level + 2, installed );
    }

    if ( ! m_parser->full() )
      shownMap[ dep ] = true;
  }
}

void
Pkgman::printRevDepList( const pkgname_t& arg, int level,
                         const pkgstate::snapshot& installed )
{
  set< string > revdep;

  for ( const auto& [ name, pkg ]: m_repo->packages() )
  {
    if ( ! m_parser->all() && ! isInstalled( installed, name ) )
      continue;

    if ( pkg && contains( pkg->dependencies(), arg ) )
    {
      list< string > deps;
      split( pkg->dependencies(), ',', deps );

      if ( contains( deps, arg ) )
        revdep.insert( name );
    }
  }

  static map< string, bool > shownMap;

  for ( const auto& dep: revdep )
  {
    if ( shownMap[ dep ] )
      continue;

    cout << string( level, ' ' );
    briefInfo( dep, installed );

    if ( m_parser->recursive() )
      printRevDepList( dep, level + 1, installed );

    if ( ! m_parser->full() )
      shownMap[ dep ] = true;
  }
}

void
Pkgman::printRevDepTree( const pkgname_t& arg, int level,
                         const pkgstate::snapshot& installed )
{
  set< string > revdep;

  for ( const auto& [ name, pkg ]: m_repo->packages() )
  {
    if ( ! m_parser->all() && ! isInstalled( installed, name ) )
      continue;

    if ( pkg && contains( pkg->dependencies(), arg ) )
    {
      list< string > deps;
      split( pkg->dependencies(), ',', deps );

      if ( contains( deps, arg ) )
        revdep.insert( name );
    }
  }

  static map< string, bool > shownMap;

  for ( const auto& dep: revdep )
  {
    cout << ( isInstalled( installed, dep ) ? "[i] " : "[ ] " )
         << string( level, ' ' );

    briefInfo( dep, installed, false );

    if ( shownMap[ dep ] )
    {
      cout << " -->\n";
      continue;
    }

    cout << endl;

    if ( m_parser->recursive() )
      printRevDepTree( dep, level + 2, installed );

    if ( ! m_parser->full() )
      shownMap[ dep ] = true;
  }
}

void
Pkgman::getRemovableDeps( const string& name,
                           PackageNames& removable,
                           const pkgstate::snapshot& installed )
{
  const auto& pkg = m_repo->getPackage( name );
  if ( ! pkg || ! isInstalled( installed, name ) )
    return;

  list< string > deps;
  split( pkg->dependencies(), ',', deps );

  for ( const auto& dep: deps )
  {
    PackageNames revdep;
    for ( const auto& [ candidate_name, candidate_pkg ]: m_repo->packages() )
    {
      if ( ! isInstalled( installed, candidate_name ) )
        continue;

      list< string > candidate_deps;
      split( candidate_pkg->dependencies(), ',', candidate_deps );

      if ( contains( candidate_deps, dep ) )
        revdep.push_back( candidate_name );
    }

    if ( revdep.size() == 1 )
    {
      if ( m_parser->recursive() )
        getRemovableDeps( dep, removable, installed );

      if ( revdep.front() == name )
        removable.push_back( dep );
    }
  }
}

bool
Pkgman::footprintGrep( const string&    filename,
                       const string&    pattern,
                       list< string >&  result,
                       bool             fullPath,
                       bool             useRegex )
{
  ifstream file( filename );
  if ( ! file.is_open() )
    return false;

  string line;
  while ( getline( file, line ) )
  {
    size_t npos = line.find_last_of( "\t" );
    string input = '/' + line.substr( npos + 1 );
    string search_input = input;

    if ( ( npos = input.find( "->" ) ) != std::string::npos )
      search_input.erase( npos - 1 );

    char* name = const_cast< char* >( search_input.data() );

    if ( ! fullPath )
      name = basename( name );

    if ( useRegex )
    {
      RegEx re( pattern );
      if ( re.match( name ) )
        result.push_back( input );
    }
    else
    {
      if ( fnmatch( pattern.c_str(), name, FNM_CASEFOLD ) == 0 )
        result.push_back( input );
    }
  }
  file.close();

  return true;
}

void
Pkgman::executeTransaction( Transaction&                transaction,
                            Transaction::Transaction_t  transactionType )
{
  m_currentTransaction = &transaction;

  switch ( transactionType )
  {
    case Transaction::INSTALL:
    case Transaction::UPDATE:
      transaction.install( transactionType );
      break;
    case Transaction::REMOVE:
      transaction.remove();
      break;
    default:
      return errx( "unknown transaction type" );
  }

  if ( transaction.result() != Transaction::SUCCESS )
    errx( transaction.strerror(), transaction.result() );

  evaluateResult( transaction );

  m_currentTransaction = 0;
}

void
Pkgman::evaluateResult( const Transaction&  transaction,
                        bool                interrupted )
{
  int errors = 0;
  const auto& ignored          = transaction.ignored();
  const auto& missing          = transaction.missing();
  const auto& downloadFailed   = transaction.downloadFailed();
  const auto& buildFailed      = transaction.buildFailed();
  const auto& installFailed    = transaction.installFailed();
  const auto& removeFailed     = transaction.removeFailed();
  const auto& alreadyInstalled = transaction.alreadyInstalled();
  const auto& installed        = transaction.installed();
  const auto& removed          = transaction.removed();

  if ( ignored.size() )
  {
    cout << "\n-- Packages ignored\n";

    for ( const auto& pkg: ignored )
      cout << pkg << endl;
  }

  if ( missing.size() )
  {
    ++errors;
    cout << "\n-- Packages not found\n";

    for ( const auto& [ pkg1, pkg2 ]: missing )
    {
      cout << pkg1;
      if ( pkg2.size() )
        cout << " from " << pkg2;
      cout << endl;
    }
  }

  if ( downloadFailed.size() )
  {
    ++errors;
    cout << "\n-- Packages were download failed\n";

    for ( const auto& pkg: downloadFailed )
      cout << pkg << endl;
  }

  if ( buildFailed.size() )
  {
    ++errors;
    cout << "\n-- Packages where build failed\n";

    for ( const auto& pkg: buildFailed )
      cout << pkg << endl;
  }

  if ( installFailed.size() )
  {
    ++errors;

    if ( transaction.type() == Transaction::UPDATE )
      cout << "\n-- Packages where update failed\n";
    else
      cout << "\n-- Packages where install failed\n";

    for ( const auto& [ pkg, info ]: installFailed )
      cout << pkg << " " << info.strRunScriptsState() << endl;
  }

  if ( removeFailed.size() )
  {
    ++errors;

    cout << "\n-- Packages where removal failed\n";

    for ( const auto& [ pkg, info ]: removeFailed )
      cout << pkg << " " << info.strRunScriptsState() << endl;
  }

  if ( alreadyInstalled.size() )
  {
    cout << "\n-- Packages installed before this run (ignored)\n";

    for ( const auto& pkg: alreadyInstalled )
      cout << pkg << endl;
  }

  if ( installed.size() )
  {
    if ( transaction.type() == Transaction::UPDATE )
      cout << "\n-- Packages updated\n";
    else
      cout << "\n-- Packages installed\n";

    bool atLeastOnePackageHasReadme = false;

    for ( const auto& [ pkg, info ]: installed )
    {
      cout << pkg;

      const auto& pkgobj = m_repo->getPackage( pkg );

      if ( pkgobj->hasReadme() )
      {
        if ( m_config->readmeMode() == ConfigParser::COMPACT_README )
          cout << " (README)";
        atLeastOnePackageHasReadme = true;
      }

      cout << " " << info.strRunScriptsState() << endl;
    }

    // readme's
    if (   atLeastOnePackageHasReadme
        && m_config->readmeMode() == ConfigParser::VERBOSE_README )
    {
      if ( transaction.type() == Transaction::UPDATE )
        cout << "\n-- Updated packages with README files:\n";
      else
        cout << "\n-- Installed packages with README files:\n";

      for ( const auto& [ pkg, info ]: installed )
      {
        if ( m_repo->getPackage( pkg )->hasReadme() )
          cout << pkg << endl;
      }
    }
  }

  if ( removed.size() )
  {
    cout << "\n-- Packages removed\n";
    for ( const auto& [ pkg, info ]: removed )
      cout << pkg << " " << info.strRunScriptsState() << endl;
  }

  if ( m_undefinedVersionComp.size() )
  {
    const auto installedState = m_state.read();
    cout << "\n-- Packages with undecidable version difference "
         << "(use --config-append='preferhigher no')\n";

    for ( const auto& name: m_undefinedVersionComp )
      cout << name
           << " ("
           << installedVersion( installedState, name )
           << " vs "
           << m_repo->getPackage( name )->version()
           << ")\n";
  }

  cout << endl;

  if ( errors == 0 && ! interrupted )
  {
    if      ( transaction.type() == Transaction::UPDATE  )
      cout << "Update" ;
    else if ( transaction.type() == Transaction::INSTALL )
      cout << "Install";
    else if ( transaction.type() == Transaction::REMOVE  )
      cout << "Remove" ;
    cout << " transaction done.\n";
  }
  else
    m_returnValue = P_PARTIAL_INSTALL_ERROR;
}

void
Pkgman::briefInfo( const Package* pkg, bool newline )
{
  cout << ( m_parser->printPath() ? pkg->fullpath() : pkg->name() );

  if ( m_parser->verbose() > 0 )
    cout << " " << pkg->version_release();

  if ( m_parser->verbose() > 1 && pkg->description().size() )
    cout << ": " << pkg->description();

  if ( newline )
    cout << endl;
}

void
Pkgman::briefInfo( const pkgname_t& name,
                   const pkgstate::snapshot& installed,
                   bool newline )
{
  initRepo();

  const auto& pkg = m_repo->getPackage( name );
  if ( pkg )
    return briefInfo( pkg, newline );

  cout << name;
  const auto version = installedVersion( installed, name );
  if ( version.size() && m_parser->verbose() > 0 )
    cout << " " << version;
  cout << " (not found in the packages sources tree)";

  if ( newline )
    cout << endl;
}
