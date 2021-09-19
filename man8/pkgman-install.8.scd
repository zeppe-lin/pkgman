pkgman-install(8) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman-install - install package(s)

# SYNOPSIS

*pkgman install* [*-v*|*-vv*] [*--test*] [*--log*] [*--deps*] [*--group*] [*--margs*=_PKGMK-ARGS_|_PKGMK-ALIASES_] [*--aargs*=_PKGADD-ARGS_|_PKGADD-ALIASES_] [*--ignore*=_PKGNAME_,...] [*--install-scripts*|[*--pre-install*][*--post-install*]] _PKGNAME_,...

# DESCRIPTION

Install all packages in the listed order.++
It looks in all directories specified in _/etc/pkgman.conf_(5).

# OPTIONS

*-v*
	Be more verbose.

*-vv*
	Log to _stdout_ the installation process.

*--test*
	Dry run, don't actually install anything.

*--log*
	Create a transaction log file. It can be set globally for all
	transactions.

	See "writelog" in *pkgman.conf*(5).

*--deps*
	Install all packages in the listed order including their dependencies.

*--group*
	Install all packages in the listed order, but stop if installation of
	one package fails.  This can be used to install packages where
	_PKGNAME1_ depends on _PKGNAME2_.

*--margs*=_PKGMK-ARGS_
	Arguments to be passed to *pkgmk*(8).

*-fr*
	Force rebuild, implies 'pkgmk -f'.
	The same as *--margs=-f*.

*-um*
	Update md5sum, implies 'pkgmk -um'.
	The same as *--margs=-um*.

*-im*
	Ignore md5sum, implies 'pkgmk -im'.
	The same as *--margs=-im*.

*-uf*
	Update footprint, implies 'pkgmk -uf'.
	The same as *--margs=-uf*.

*-if*
	Ignore footprint, implies 'pkgmk -if'.
	The same as *--margs=-if*.

*-ns*
	No stripping, implies 'pkgmk -ns'.
	The same as *--margs=-ns*.

*-kw*
	Keep working directory, implies 'pkgmk -kw'.
	The same as *--margs=-kw*.

*--aargs*=_PKGADD-ARGS_
	Arguments to be passed to *pkgadd*(8).

*-f, -i*
	Force install, implies 'pkgadd -f'.
	The same as *--aargs=-f*.

*--ignore*=_PKGNAME_,...
	Don't install those packages, even if they are listed as dependency.

*--install-scripts*
	Execute pre-install and post-install scripts if they are there.

*--pre-install*
	Execute pre-install script if it is there.

*--post-install*
	Execute post-install script if it is there.

; vim:ft=scdoc:cc=80
; End of file.
