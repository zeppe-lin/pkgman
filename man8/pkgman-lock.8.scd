pkgman-lock(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-lock - lock the package(s)

# SYNOPSIS

*pkgman* lock _PKGNAME_,...

# DESCRIPTION

Lock the packages and don't update the in a *pkgman-sysup*(8) operation.

# FILES

#LOCALSTATEDIR#/lib/pkg/locked
	Database of locked packages.

# SEE ALSO

*pkgman-unlock*(8), *pkgman-list-locked*(8)

; End of file.
