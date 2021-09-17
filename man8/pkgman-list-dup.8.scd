pkgman-list-dup(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-list-dup - print list of duplicate packages

# SYNOPSIS

*pkgman* list-dup [--prefer-higher|--strict-diff] [_FORMAT_]

# DESCRIPTION

Print list of duplicate packages in the pkgsrc repositories.
It looks in all directories specified in _/etc/pkgman.conf_(5).

The _FORMAT_ string by default is '%p1 %v1 %c %p2 %v2\\n'.

# OPTIONS

[[ *--prefer-higher*
:< Prefer higher installed versions over lower ones in the pkgsrc repositories.
|  *--strict-diff*
:  Override *preferhigher* configuration settings.

[[ _FORMAT_
:< The string that contain variables, which are replaced by:
|] %n
:  package name
|] %p1
:  full path (including name) to package' source that has priority
|] %p2
:  full path (including name) to package' source being hidden
|] %v1
:  the version of the package, that has priority
|] %v2
:  the version of the hidden package
|] %c
:  comparison sign of %p1 and %p2: <, >, ==, !=
|] \\n
:  newline
|] \\t
:  tab

No additional format specifier supported.

; End of file.
