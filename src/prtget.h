////////////////////////////////////////////////////////////////////////
// FILE:        prtget.h
// AUTHOR:      Johannes Winkelmann, jw@tks6.net
// COPYRIGHT:   (c) 2002 by Johannes Winkelmann
// ---------------------------------------------------------------------
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
////////////////////////////////////////////////////////////////////////

#ifndef _PRTGET_H_
#define _PRTGET_H_

class Repository;
class ArgParser;
class Configuration;

#include <list>
#include <utility>
#include <string>
using namespace std;

#include "pkgdb.h"
#include "signaldispatcher.h"
#include "locker.h"
#include "installtransaction.h"
#include "versioncomparator.h"

/*!
  \class PrtGet
  \brief Main class

  This is prt-get's main class, controlling all the commands
*/
class PrtGet
    : public SignalHandler
{
public:

    enum PGReturnStates {
        PG_GENERAL_ERROR = -1,
        PG_OK = 0,
        PG_ARG_ERROR,
        PG_INSTALL_ERROR,
        PG_PARTIAL_INSTALL_ERROR
    };

    PrtGet( const ArgParser* parser );
    ~PrtGet();

    void printVersion();
    void printUsage();

    void listPackages();
    void listShadowed();
    void listInstalled();

    void searchPackages( bool searchDesc=false );
    void printInfo();
    void isInstalled();
    void readme();

    void install( bool update=false,
                  bool group=false,
                  bool dependencies=false );
    void sysup();
    void current();
    void printDepends( bool simpleListing=false );
    void printDependTree();
    void printDependent();
    void printDiff();
    void printQuickDiff();
    void listOrphans();

    void createCache();

    void printPath();
    void printf();

    void cat();
    void ls();
    void edit();

    void remove();

    void setLock( bool lock );
    void listLocked();

    void fsearch();

    void dumpConfig();

    int returnValue() const;

    SignalHandler::HandlerResult handleSignal( int signal );

protected:

    void printDepsLevel(int indent, const Package* package);

    void printDependent(const std::string& dep, int level);

    void executeTransaction( InstallTransaction& transaction,
                             bool update, bool group );
    void evaluateResult( InstallTransaction& transaction,
                      bool update,
                      bool interrupted=false );
    void reportPrePost(const InstallTransaction::InstallInfo& info);

    void readConfig();
    void initRepo( bool listDuplicate=false );

    void expandWildcardsPkgDB( const list<char*>& in,
                               map<string, string>& target );
    void expandWildcardsRepo( const list<char*>& in,
                              list<string>& target );

    void warnPackageNotFound(InstallTransaction& transaction);

    static void printFormattedDiffLine(const string& name,
                                       const string& version1,
                                       const string& version2,
                                       bool locked);


    Repository* m_repo;
    PkgDB* m_pkgDB;
    Configuration* m_config;
    InstallTransaction* m_currentTransaction;

    Locker m_locker;

    const ArgParser* m_parser;
    string m_appName;
    string m_cacheFile;

    int m_returnValue;

    bool m_useRegex;

    /*! Name of default configuration file */
    static const string CONF_FILE;

    /*! Name of default cache file */
    static const string DEFAULT_CACHE_FILE;


    void assertMinArgCount(size_t count);
    void assertMaxArgCount(size_t count);
    void assertExactArgCount(size_t count);
    void argCountFailure(size_t count, const string& specifier);

    VersionComparator::COMP_RESULT
    compareVersions( const string& v1, const string& v2 );
    static bool printFile(const string& file);

    // map package from ports tree to version currently installed
    std::list< std::pair<const Package*, std::string> > m_undefinedVersionComp;
};

#endif /* _PRTGET_H_ */
