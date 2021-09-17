pkgman-list(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-list -- print list of available packages

# SYNOPSIS

*pkgman* list [-v|-vv] [--path] [--all] [--depsort] [--regex] [--filter=_PKGNAME_]

# DESCRIPTION

Print list of available packages.  It looks in all directories specified in
_/etc/pkgman.conf_(5).

# OPTIONS

[[ *-v*
:< Show version in the listing.
|  *-vv*
:  Show version and description in the listing.
|  *--path*
:  Show package' source path in the listing.
|  *--all*
:  List all packages, not only installed.
|  *--depsort*
:  List the packages sorted by dependency.
|  *--regex*
:  Interpret *--filter* argument as a regular expression.
|  *--filter=*_PKGNAME_
:  Filter listing packages by _PKGNAME_. The argument may be the string,
   wildcard or regex.

; End of file.
