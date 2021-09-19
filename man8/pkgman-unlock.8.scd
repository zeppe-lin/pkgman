pkgman-unlock(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-unlock - unlock the package(s)

# SYNOPSIS

*pkgman unlock* _PKGNAME_,...

# DESCRIPTION

Remove lock from these packages.++
They will be available for updating in *pkgman-sysup*(8) operation.

# FILES

\#LOCALSTATEDIR#/lib/pkg/locked
	Database of locked packages.

# SEE ALSO

*pkgman-lock*(8), *pkgman-list-locked*(8)

; vim:cc=80
; End of file.
