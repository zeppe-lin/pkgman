pkgman-fsearch(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-fsearch - search the pkgsrc repositories for a file in package footprint

# SYNOPSIS

*pkgman* *fsearch* [*-v*|*-vv*] [*--path*] [*--regex*] [*--full*] _EXPR_

# DESCRIPTION

Search the pkgsrc repositories for _EXPR_ in packages' .footprint files.++
It looks in all directories specified in _/etc/pkgman.conf_.

# OPTIONS

*-v*
	Show version in the listing.

*-vv*
	Show version and description in the listing.

*--path*
	Show package source path in the listing.

*--regex*
	Interpret _EXPR_ as a regular expression.

*--full*
	Full path search (e.g. pkgman fsearch --full /usr/bin/gcc).

_EXPR_
	String, shell-like wildcard pattern, or regular expression.

; vim:ft=scdoc:cc=80
; End of file.
