pkgman-list-locked(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-list-locked - print list of locked packages

# SYNOPSIS

*pkgman list-locked* [*-v*|*-vv*] [*--path*]

# DESCRIPTION

Print list of locked packages. "Locked" are packages that will not update while
performing a system update.

# OPTIONS

*-v*
	Show version in the listing.
*-vv*
	Show version and description in the listing.
*--path*
	Show package' source path in the listing.

# FILES

\#LOCALSTATEDIR#/lib/pkg/locked
	Database of locked packages.

; End of file.
