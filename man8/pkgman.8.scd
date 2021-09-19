pkgman(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman - advanced package management tool

# SYNOPSIS

*pkgman* [*--no-std-config*|*--config*=_FILE_] [*--config-set*=_STRING_] [*--config-append*=_STRING_] [*--config-prepend*=_STRING_] [*--root*=_PATH_] _COMMAND_ [_COMMAND-ARGS_]

# DESCRIPTION

*pkgman* is a package management tool that provides additional functionality to
basic package management utilities, like *pkgmk*(8), *pkgadd*(8), *pkgrm*(8) and
*pkginfo*(8).

# OPTIONS

*--no-std-config*
	Ignore _#SYSCONFDIR#/pkgman.conf_.

*--config*=_FILE_
	Use alternative configuration file.

*--config-set*=_STRING_
	Set _STRING_ in configuration, overriding configuration file.

*--config-append*=_STRING_
	Append _STRING_ to configuration.

*--config-prepend*=_STRING
	Prepend _STRING_ to configuration.

*--root*=_PATH_
	Use _PATH_ as root directory.

# COMMANDS

*pkgman* uses so-called commands, which always have to be the first non-option
argument passed.

The commands can be divided into self-explanatory categories:

## INFORMATIONAL

*pkgman-dumpconfig*(8)
	dump the configuration

*pkgman-list*(8)
	print list of available packages

*pkgman-list-dup*(8)
	print list of duplicate packages

*pkgman-list-orphans*(8)
	print list of orphans packages

*pkgman-list-locked*(8)
	print list of locked packages

*pkgman-printf*(8)
	print formatted list of available packages

*pkgman-info*(8)
	print package info

*pkgman-readme*(8)
	print the package's README

*pkgman-path*(8)
	print path to package

*pkgman-isinst*(8)
	print whether packages are installed or not

*pkgman-current*(8)
	print installed package version

## DIFFERENCES / CHECK FOR UPDATES

*pkgman-diff*(8)
	show difference between installed and available packages

## DEPENDENCIES

*pkgman-mdep*(8)
	print the missing dependencies of installed packages

*pkgman-dep*(8)
	print dependencies for a package

*pkgman-rdep*(8)
	print reverse dependencies for a package

## SEARCHING

*pkgman-search*(8)
	search the repositories for package name

*pkgman-dsearch*(8)
	search the repositories for package name and/or description

*pkgman-fsearch*(8)
	search the repositories for a file in package footprint

## INSTALL / UPDATE / REMOVE

*pkgman-install*(8)
	install package(s)

*pkgman-update*(8)
	update installed package(s)

*pkgman-remove*(8)
	remove installed package(s)

## SYSTEM UPDATE

*pkgman-sysup*(8)
	update all outdated packages

*pkgman-lock*(8)
	lock the package(s)

*pkgman-unlock*(8)
	unlock the package(s)

## FILE OPERATIONS

*pkgman-ls*(8)
	print out a listing of the package's directory

*pkgman-cat*(8)
	print out the package's file

*pkgman-edit*(8)
	edit the package's file

# CONFIGURATION

See *pkgman.conf*(5) manual page.

# FILES

\#SYSCONFDIR#/pkgman.conf
	Configuration file.

\#LOCALSTATEDIR#/lib/pkg/db
	Database of currently installed packages.
	Used by *pkgadd*(8), *pkgrm*(8), *pkginfo*(8), and therefore by pkgman.

\#LOCALSTATEDIR#/lib/pkg/locked
	Database of currently locked packages.
	Used in *pkgman-lock*(8), *pkgman-unlock*(8) and *pkgman-list-locked*(8)
	operations.

*!!Caution!!* When the *--root=*_PATH_ option is specified, databases location
changes to _PATH#LOCALSTATEDIR#/lib/pkg/..._.

# SEE ALSO

*pkgman.conf*(5)

; vim:ft=scdoc:cc=80
; End of file.
