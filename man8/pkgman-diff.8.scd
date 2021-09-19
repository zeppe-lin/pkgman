pkgman-diff(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-diff - show differences between installed and available packages

# SYNOPSIS

*pkgman diff* [*--all*] [*--deps*] [*--full*] [*--regex*] [*--filter*=_NAME_]

# DESCRIPTION

Print an overview of packages that are installed and have different version than
they are in the repository.

In version comparison using *preferhigher* configuration settings.

```
pkgman --config-set='preferhigher yes' diff
```

# OPTIONS

*--all*
	Display locked packages too.

*--deps*
	Calculate the new dependencies for available packages.

*--full*
	Show full table.

*--prefer-higher*=(yes|no)
	Prefer higher installed versions over lower packages.
	Overwriting *preferhigher* configuration expression.

*--regex*
	Interpret *--filter* argument as a regular expression.

*--filter*=_NAME_
	Filter listing packages by name. The filter argument may be the string,
	wildcard or regex.

; vim:ft=scdoc:cc=80
; End of file.
