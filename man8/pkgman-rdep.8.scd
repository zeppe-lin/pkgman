pkgman-rdep(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-rdep - print reverse dependencies for a package

# SYNOPSIS

*pkgman* rdep [-v|-vv] [--path] [--all] [--tree] [--recursive [--full]]
_PKGNAME_

# DESCRIPTION

Print reverse dependencies for a package.  It looks in all directories specified
in _/etc/pkgman.conf_(5).

# OPTIONS

[[ *-v*
:< Show version in the listing.
|  *-vv*
:  Show version and description in the listing.
|  *--path*
:  Show package source path in the listing.
|  *--all*
:  List all packages, not only installed.
|  *--tree*
:  Print a nicely indented tree of the package dependencies.
|  *--recursive*
:  Print a recursive list of dependencies.
   In case *--tree* was specified, subtrees already shown are marked with '-->'
   to save some space. If *--tree* option wasn't specified, dependencies
   already shown aren't displayed. In order to show them all, add the
   *--full* switch.
|  *--full*
:  If recursive mode was set, show all dependencies.

; End of file.
