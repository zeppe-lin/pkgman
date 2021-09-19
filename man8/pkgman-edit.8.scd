pkgman-edit(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-edit - edit the package' file

# SYNOPSIS

*pkgman edit* _PKGNAME_ [_FILE_]

# DESCRIPTION

Edit the package' _FILE_ from the package source directory using the editor
specified in the $EDITOR environment variable.

If _FILE_ is not specified, _Pkgfile_ is used.

# ENVIRONMENT

## EDITOR

If the package' source directory has a _FILE_, then *pkgman* will try to open
it using a *EDITOR*.

If *PAGER* is unset, then *pkgman* will fail.

# SEE ALSO

*pkgman-ls*(8), *pkgman-cat*(8)

; vim:ft=scdoc:cc=80
; End of file.
