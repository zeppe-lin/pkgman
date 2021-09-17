pkgman-info(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-info - print package source info

# SYNOPSIS

*pkgman* info [-v|-vv] _PKGNAME_

# DESCRIPTION

Print available info for a source package.

# OPTIONS

[[ *-v*, *-vv*
:< Print also the README file if exists.


# ENVIRONMENT

## PAGER
	If the package' source directory has a README file, and the *-v* or
	*-vv* option is set to print it, then *pkgman* will try to open it using
	a *PAGER*. If *PAGER* is unset, then *pkgman* prints the README to
	_stdout_.

# EXIT STATUS

0 if success, 255 otherwise.

# SEE ALSO

*pkgman-readme*(8), *pkgman-readme*(5)

; End of file.
