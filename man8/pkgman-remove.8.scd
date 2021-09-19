pkgman-remove(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-remove - remove installed package(s)

# SYNOPSIS

*pkgman remove* [*-v*|*-vv*] [*--test*] [*--log*] [*--deps*] [*--group*] [*--rargs*=_PKGRM-ARGS_|_PKGRM-ALIASES_] [*--ignore*=_PKGNAME_,...] [*--remove-scripts*|[*--pre-remove*][*--post-remove*]] _PKGNAME_,...

# DESCRIPTION

Remove all installed packages in the listed order.

It looks in all directories specified in _/etc/pkgman.conf_(5) to find the
package source before removing the installed package. If the package source
can't be found an error will be displayed. See *pkgrm*(8) if needs to remove an
installed but orphaned package.

# OPTIONS

*-v*
	Be more verbose.

*-vv*
	Log to _stdout_ the removing process.

*--test*
	Dry run, don't actually remove anything.

*--log*
	Create a transaction log file. It can be set globally for all
	transactions.

	See "writelog" in *pkgman.conf*(5).

*--deps*
	Remove all packages in the listed order including their dependencies.

*--group*
	Remove all packages in the listed order, but stop if removing of one
	package fails. This can be used to remove packages where _PKGNAME1_
	depends on _PKGNAME2_.

*--rargs*=_PKGRM-ARGS_
	Arguments to be passed to *pkgrm*(8).

*--ignore*=_PKGNAME_,...
	Don't remove those packages, even if they are listed as dependency.

*--remove-scripts*
	Execute pre-remove and post-remove scripts if they are there.

*--pre-remove*
	Execute pre-remove script if it is there.

*--post-remove*
	Execute post-remove script if it is there.

; vim:ft=scdoc:cc=80
; End of file.
