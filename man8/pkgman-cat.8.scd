pkgman-cat(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-cat - print out the package's file

# SYNOPSIS

*pkgman* cat _PKGNAME_ [_FILE_]

# DESCRIPTION

Prints out the package file from the pkgsrc directory.
If _FILE_ is not specified, _Pkgfile_ is used.

# ENVIRONMENT

## PAGER
	If the package' source directory has a _FILE_, then *pkgman*
	will try to open it using a *PAGER*. If *PAGER* is unset,
	then *pkgman* prints the _FILE_ to _stdout_.

# SEE ALSO

*pkgman-ls*(8), *pkgman-edit*(8)

; End of file.
