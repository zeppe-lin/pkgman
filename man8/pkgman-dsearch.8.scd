pkgman-dsearch(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-dsearch - search the pkgsrc repositories for package name/description

# SYNOPSIS

*pkgman dsearch* [*-v*|*-vv*] [*--path*] [*--regex*] _EXPR_

# DESCRIPTION

Search the pkgsrc repositories for _EXPR_ in packages' name and/or description.++
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

_EXPR_
	String or regular expression.

; vim:ft=scdoc:cc=80
; End of file.
