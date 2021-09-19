pkgman-printf(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-printf - print formatted list of available packages

# SYNOPSIS

*pkgman printf* [*--sort*=_FORMAT_] [*--regex*] [*--filter*=_PKGNAME_] _FORMAT_

# DESCRIPTION

Print formatted list of available packages.++
It looks in all directories specified in _/etc/pkgman.conf_(5).

# OPTIONS

*--sort=*_FORMAT_
	The _FORMAT_ string (see below) that is used to sort the output.

*--regex*
	Interpret *--filter* argument as regex.

*--filter*=_PKGNAME_
	Filter listing packages by _PKGNAME_. The argument may be the string,
	wildcard or regex.

_FORMAT_
	The string that contains variables, which are replaced by text.

	See _FORMAT SPECIFIERS_.

# FORMAT SPECIFIERS

[[ *Specifier*
:< *Replacement*
|  %n
:  name
|  %p
:  path
|  %v
:  version
|  %r
:  release
|  %d
:  description
|  %e
:  dependencies
|  %u
:  url
|  %P
:  packager
|  %M
:  maintainer
|  %R
:  has readme (yes/no)
|  %A
:  has pre-install script (yes/no)
|  %B
:  has post-install script (yes/no)
|  %C
:  has pre-remove script (yes/no)
|  %D
:  has post-remove script (yes/no)
|  %l
:  is locked (yes/no)
|  %i
:  is installed (yes/no/diff)
   'diff' means the package is installed and different package version is
   available in the pkgsrc repositories.

No additional specifiers supported.

; vim:ft=scdoc:cc=80
; End of file.
