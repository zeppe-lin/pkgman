//! \file pkgman.h
//! \brief Pkgman main commands definition.
//! \copyright See COPYING for license terms and COPYRIGHT for notices.

#pragma once

#include <iostream>
#include <list>
#include <string>
#include <utility>

#include <libpkgstate/libpkgstate.h>

class ArgParser;
class ConfigParser;
class Repository;

#include "transaction.h"
#include "locker.h"
#include "signaldispatcher.h"
#include "versioncomparator.h"

using namespace std;


//! \class  Pkgman
//! \brief  Pkgman's main class, controlling all the commands
class Pkgman : public SignalHandler
{

public:


  //! \brief   Create a Pkgman object
  //!
  //! \param   parser  the argument parser to be used
  Pkgman( const ArgParser* parser, pkgstate::store& state );


  //! Destroy a Pkgman object
  ~Pkgman();


  //! Pkgman return states
  enum PReturnStates
  {
    P_GENERAL_ERROR          = -1,  //!< Error
    P_OK                     =  0,  //!< Success
    P_ARG_ERROR,                    //!< Command-line argument error
    P_INSTALL_ERROR,                //!< Installation fails
    P_PARTIAL_INSTALL_ERROR,        //!< Group installation fails
    P_DEPENDENCY_CALC_ERROR,        //!< Dependency calculation fails
  };


  /////////////////////////////////////////////////////////////////////
  //                     Informational commands                      //
  /////////////////////////////////////////////////////////////////////


  //! Dump the configuration
  void dumpConfig() const;


  //! Print list of available packages
  void listPackages();


  //! Print list of duplicate packages in the repository
  void listShadowed();


  //! Print list of independent packages
  void listNodependents();


  //! Print list of orphaned packages
  void listOrphans();


  //! Print list of locked packages
  void listLocked();


  //! Print formatted list of packages available in the repository
  void printf();


  //! Print info for a package
  void printInfo();


  //! Print package source's readme file
  void printReadme();


  //! Print path to package source
  void printPath();


  //! Print whether a package is installed or not
  void printIsInstalled();


  //! Print installed package version
  void printCurrentVersion();


  /////////////////////////////////////////////////////////////////////
  //                 Differences / Check for updates                 //
  /////////////////////////////////////////////////////////////////////


  //! Print an overview of packages that are installed and have
  //! a different version than they are in the repository
  void printDiff();


  /////////////////////////////////////////////////////////////////////
  //                          Dependencies                           //
  /////////////////////////////////////////////////////////////////////


  //! Print dependencies for a package
  void printDep();


  //! Print reverse dependencies for a package
  void printRevDep();


  /////////////////////////////////////////////////////////////////////
  //                            Searching                            //
  /////////////////////////////////////////////////////////////////////


  //! \brief   Search repository packages for a certain pattern
  //!          in their names and/or description
  //!
  //! \param   desc  search also in packages' descriptions
  //!
  //! \see     Repository::searchMatchingPackages()
  void psearch( bool desc=false );


  //! \brief   Search repository for a certain pattern as a filename
  //!          in their footprint
  //!
  //! \see     Repository::getMatchingPackages()
  void fsearch();


  /////////////////////////////////////////////////////////////////////
  //                    Install / Update / Remove                    //
  /////////////////////////////////////////////////////////////////////


  //! \brief   Install or Update the package(s)
  //!
  //! \param   transactionType the transaction type (install or update)
  //!
  //! \see     Transaction::Transaction_t
  void install( Transaction::Transaction_t transaction_t );


  //! Remove the package(s)
  void remove();


  /////////////////////////////////////////////////////////////////////
  //                          System update                          //
  /////////////////////////////////////////////////////////////////////


  //! Update all outdated packages
  void sysup();


  //! \brief   (Un)Lock the packages and don't update them in the
  //!          \sysup operation
  //! \param   lock  lock if \a true, unlock otherwise
  void setLock( bool lock );


  /////////////////////////////////////////////////////////////////////
  //                         File operations                         //
  /////////////////////////////////////////////////////////////////////


  //! Print a listing of the package source's directory
  void ls();


  //! Print the package source's file
  void cat();


  //! Edit the package source's file
  void edit();


  /////////////////////////////////////////////////////////////////////


  //! \brief   Interrupt handler
  //!
  //! \param   signal  unused
  //!
  //! \return  \a HandlerResult of catched signal
  SignalHandler::HandlerResult handleSignal( int signal );


  //! \a Pkgman return code
  int returnValue() const;


protected:


  //! Name of the default configuration file
  static const string CONF_FILE;


  //! Argument parser
  const ArgParser* m_parser;


  //! Repository of all available packages
  Repository* m_repo;


  //! Configuration file parser
  ConfigParser* m_config;


  //! Package locker
  Locker* m_locker;


  //! Current executing transaction
  Transaction* m_currentTransaction;


  //! Return code
  int m_returnValue;


  //! Durable installed package state.
  pkgstate::store& m_state;


  //! Whether \a ConfigParser::useRegex() or \a ArgParser::useRegex() is set
  bool m_useRegex;


  //! Packages that have a greater version in the packages sources tree
  list< string > m_greaterVersionComp;


  //! Packages with undecidable version differences
  list< string > m_undefinedVersionComp;


  //! Packages that are installed but not found in the packages sources tree
  list< pair< string, string > > m_missingRepoPackages;


  //! \brief   Helper function to display an error message
  //!          and set \a Pkgman return value
  //!
  //! \param   error_message  the error message
  //! \param   ret            the Pkgman's return value
  inline void errx( const string& error_message,
                    int           ret = P_GENERAL_ERROR );


  //! Read the config file
  void readConfig();


  //! \brief   Initialize repository
  //!
  //! \param   listDuplicate  whether duplicates should registered
  //!
  //! \see     Repository::initFromFS()
  void initRepo( bool listDuplicate = false );


  //! Convert command arguments into owned package names.
  static PackageNames packageNames( const list< char* >& names );


  //! Return installed package names in snapshot order.
  static PackageNames installedPackageNames(
      const pkgstate::snapshot& installed );


  //! Return whether a package is present in a snapshot.
  static bool isInstalled( const pkgstate::snapshot& installed,
                           const pkgname_t& name );


  //! Return a package version from a snapshot, or an empty string.
  static string installedVersion( const pkgstate::snapshot& installed,
                                  const pkgname_t& name );


  //! Add installed packages matching command patterns.
  void expandInstalledPatterns( const PackageNames& patterns,
                             const pkgstate::snapshot& installed,
                             PackageNames& target ) const;


  //! \brief   Get the packages that match the pattern
  //!          from the whole repository
  //!
  //! \param   in      the list of patterns
  //! \param   target  save into \a target the matching packages
  //!
  //! \see     Repository::getMatchingPackages()
  void expandWildcardsRepo( const list< char* >&  in,
                            list< string >&       target );


  //! \brief   List the packages sorted by dependency,
  //!          without injecting missing ones
  //!
  //! \param   packages  the packages to be sorted and listed
  void listDepSorted( PackageNames packages,
                      const pkgstate::snapshot& installed );


  //! \brief   Print a file to stdout or open it with PAGER
  //!          if available
  //!
  //! \param   filepath  the file path
  //!
  //! \return  \a false if file doesn't exists or it can't be opened,
  //!          \a true otherwise
  static bool printFile( const string& filepath );


  //! \brief   Compare two versions
  //!
  //! \param   v1  first string
  //! \param   v2  second string
  //!
  //! \return  a \a COMP_RESULT value indicating the comparison result
  VersionComparator::COMP_RESULT compareVersions( const string& v1,
                                                  const string& v2 );


  //! \brief   Calculate the version difference between selected,
  //!          installed packages and the packages available in the
  //!          package sources tree
  //!
  //! \param   packages  the packages that need to check
  //!                    for differences
  //!
  //! \note    The calculation results are stored in the following
  //!          \a Pkgman members:
  //!          - \a m_missingRepoPackages
  //!          - \a m_greaterVersionComp
  //!          - \a m_undefinedVersionComp
  void diffCalc( const PackageNames& packages,
                 const pkgstate::snapshot& installed );


  //! \brief   Print the formatted line of the package
  //!          that has a difference
  //!
  //! \param   name              the package name
  //! \param   versionInstalled  package's installed version
  //! \param   versionAvailable  package's available version
  //!                            in the packages sources tree
  //! \param   isLocked          whether the package is in the lock list
  static void printFormattedDiffLine( const string&  name,
                                      const string&  versionInstalled,
                                      const string&  versionAvailable,
                                      bool           isLocked );


  //! \brief   Print the list of package dependencies
  //!
  //! \param   pkg  the package
  //! \param   level  the identation level
  void printDepList( const Package* pkg, int level,
                     const pkgstate::snapshot& installed );


  //! \brief   Print the tree of package dependencies
  //!
  //! \param   pkg    the package
  //! \param   level  the identation level
  void printDepTree( const Package* pkg, int level,
                     const pkgstate::snapshot& installed );


  //! \brief   Print the list of package reverse dependencies
  //!
  //! \param   arg  the package name
  //! \param   level  the identation level
  void printRevDepList( const pkgname_t& arg, int level,
                        const pkgstate::snapshot& installed );


  //! \brief   Print the tree of package reverse dependencies
  //!
  //! \param   arg    the package name
  //! \param   level  the identation level
  void printRevDepTree( const pkgname_t& arg, int level,
                        const pkgstate::snapshot& installed );


  //! \brief   Get the package's removable (unneeded) dependencies
  //!
  //! \note    The dependency is considered unnecessary if
  //!          only the removed package depends on it.
  //!
  //! \param   pkg        the package name
  //! \param   removable  the list where to save the removable
  //!                     dependencies
  void getRemovableDeps( const string& pkg,
                          PackageNames& removable,
                          const pkgstate::snapshot& installed );


  //! \brief   Search for \a pattern in \a filename
  //!          (used in fsearch command to search files in .footprints)
  //!
  //! \param   filename  file path
  //! \param   pattern   the pattern to search for
  //! \param   result    the list where to save the matched lines
  //! \param   fullPath  if true then don't strip the directories
  //!                    from the file names before matching
  //! \param   useRegex  interpret \a pattern as regular expression
  //!
  //! \return  \a false if file can't be opened, \a true otherwise
  bool footprintGrep( const string&    filename,
                      const string&    pattern,
                      list< string >&  result,
                      bool             fullPath,
                      bool             useRegex );


  //! \brief   Execute the transaction and evaluate the result
  //!
  //! \param   transaction      the transaction to execute
  //! \param   transactionType  the transaction type
  void executeTransaction( Transaction&                transaction,
                           Transaction::Transaction_t  transactionType );


  //! \brief   Helper method to print the result of \a Transaction
  //!
  //! \param   transaction  the transaction to evaluate
  //! \param   interrupted  whether the transaction was interrupted
  void evaluateResult( const Transaction&  transaction,
                       bool                interrupted=false );


  //! Print available package information.
  void briefInfo( const Package* pkg, bool newline=true );


  //! Print a package name, falling back to installed state when absent
  //! from the package sources tree.
  void briefInfo( const pkgname_t& name,
                  const pkgstate::snapshot& installed,
                  bool newline=true );


}; // class Pkgman
