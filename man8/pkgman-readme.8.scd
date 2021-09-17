pkgman-readme(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-readme - print the package's README

# SYNOPSIS

*pkgman* readme _PKGNAME_

# DESCRIPTION

Print the package's README file if it exists.

# ENVIRONMENT

## PAGER
	If the package source has a README file, then *pkgman* will try to open
	it using a *PAGER*.  If *PAGER* is unset, then *pkgman* prints the
	README to _stdout_.

# SEE ALSO

*pkgman-readme*(5)

; End of file.
