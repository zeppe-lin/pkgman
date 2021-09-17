pkgman.conf(5) "version #VERSION#" "Package Manager Manual"

# NAME

pkgman.conf - configuration file for pkgman

# DESCRIPTION

This file, by default #SYSCONFDIR#/pkgman.conf, is read by *pkgman*(8).

# CONFIGURATION

Every line can contain one configuration option.  The format of the lines is
"key value".

The valid keys are:

- pkgsrcdir _PATH_:[:_PKGNAME_,...]

	The location where to look for the packages. The order of the pkgsrcdir
	options is important as if a package is in multiple directories,
	*pkgman*(8) will use the one found first (directories listed first have
	precedence).

	If you want to restrict the packages used from one pkgsrcdir, you can
	append a comma-separated list of packages to be used after the _PATH_,
	using a colon ":" character to separate the two components.

	_PATH_		The directory.

	_PKGNAME_	The final directory in the _PATH_.

	For example:

	```
	pkgsrcdir /usr/src/pkgsrc/core
	pkgsrcdir /usr/src/pkgsrc/contrib:mpv,ffmpeg
	```

- runscripts (yes|no)

	Run pre/post-install and pre/post-remove scripts.  Default: no.

- preferhigher (yes|no)

	If set to yes, *pkgman*(8) will parse version strings and prefer the higher
	one, even if the one found in the pkgsrc repositories is lower.  Will
	influence _diff_, _sysup_, ...  Default: no.

- useregex (yes|no)

	If set to yes, *pkgman*(8) will interpret _EXPR_ and _filter_ patterns
	in *pkgman-list*(8), *pkgman-listinst*(8), *pkgman-printf*(8),
	*pkgman-printf*(8), *pkgman-search*(8), *pkgman-dsearch*(8) and
	*pkgman-fsearch*(8) as regex.  Default: no.

- readme (verbose|compact|disabled)

	If the readme option is set to something different that disabled,
	*pkgman*(8) will notify the user if he/she installs a package with a
	README file.

	- verbose	: print separate information about README files
	- compact	: integrate in result output
	- disabled	: no information about README files

	See *pkgman*(8) and especially the readme command how to read those
	README files.  Default: verbose.

- writelog (enabled|disabled)

	Write the log.  Default: disabled.

- logmode (append|overwrite)

	Write log mode.  Default: overwrite.

- rmlog_on_success (yes|no)

	When set to yes, log files of successful builds will be removed.
	Default: no.

- logfile _FORMAT_

	The string that contains a path to the log file, where all occurrences
	of next characters will be replaced by corresponding items:

	- %n		: package name
	- %p		: package path
	- %v		: package version
	- %r		: package release

	This allows you to have separate log files per package. E.g:
	/var/log/pkgbuild/%n.log

	Default: ''

- makecommand _command_

	Use an alternative command instead of *pkgmk*(8) for the command which is
	used to build the package.  Default: *pkgmk*(8).

- addcommand _command_

	Use an alternative command instead of *pkgadd*(8) for the command which
	is used to install the package.  Default: *pkgadd*(8).

- removecommand _command_

	Use an alternative command instead of *pkgrm*(8) for the command which
	is used to remove the package.  Default: *pkgrm*(8).

- runscriptcommand _command_

	Use an alternative command instead of *sh*(1) for the command which is
	used to run the pre/post-install and pre/post-remove scripts.  Default:
	*sh*(1).

# EXAMPLES

```
#
# /etc/pkgman.conf: configuration file for pkgman(8).
# See pkgman.conf(5) manual page for more information.
#

## use local directory first
#pkgsrcdir /usr/src/pkgsrc/unofficial

# zeppelin official
pkgsrcdir /usr/src/pkgsrc/core
pkgsrcdir /usr/src/pkgsrc/opt
pkgsrcdir /usr/src/pkgsrc/xorg
pkgsrcdir /usr/src/pkgsrc/contrib

# log options
writelog enabled
logmode overwrite
rmlog_on_success yes
logfile /var/log/pkgbuild/%n

# notify the user about README files
readme verbose

# don't prefer higher versions in sysup / diff
preferhigher no

# don't use regex search
useregex no

# run pre/post-install and pre/post-remove scripts
runscripts yes

# End of file.
```

# TIPS

## Build packages as an unprivileged user

- Add new user:

	```
	useradd -m -U -s /bin/false -b /var/lib pkgmk
	```

- Create needed dirs (_PKGMK_SOURCE_DIR_, _PKGMK_PACKAGE_DIR_,++
_PKGMK_WORK_DIR_) and add permissions to write into them:

	```
	mkdir -p /var/lib/pkgmk/{distfiles,packages,work}
	chown pkgmk:pkgmk /var/lib/pkgmk/{distfiles,packages,work}
	```

	*Note:* you need to set up dedicated directories in _/etc/pkgmk.conf_.

- Set up the _makecommand_ in the _/etc/pkgman.conf_:

	```
	makecommand sudo -H -u pkgmk fakeroot pkgmk
	```

## Renice pkgmk's child process

If you keep using your computer while compiling packages you will notice that
your box is much less responsive as usual.  This is caused by having two
"groups" of processes with the same nice priority: your usual running tasks on
one side, and *pkgman*(8) (and its child processes) on the other.  Now, if you
could renice pkgman and its children to higher nice (i.e. lower priority!) value
(default is 0), compiling would inevitably take somewhat longer, but you could
use your workstation without noticing much differences to its usual performance.

Just set up the makecommand in the _/etc/pkgman.conf_ to something like this:

```
makecommand sudo -H -u pkgmk nice -n10 ionice -c2 -n6 pkgmk
```

or like this (to use *fakeroot*(1)):

```
makecommand sudo -H -u pkgmk nice -n10 ionice -c2 -n6 fakeroot pkgmk
```

## Build in RAM

- Find your used id for pkgmk:

	```
	id pkgmk
	```

- Edit _/etc/fstab_ and add this line. Setting your preferred size (size=...) in
M for megabytes or G for gigabytes and the user id of the previous command
(uid=...):

	```
	pkgmk /var/lib/pkgmk/work tmpfs size=SIZE_G,uid=UID,defaults 0 0
	```

- Mount tmpfs:

	```
	mount /var/lib/pkgmk/work
	```

- Edit _/etc/pkgmk.conf_ and set up your tmpfs:

	```
	PKGMK_WORK_DIR="/var/lib/pkgmk/work/$name"
	```

- Also, edit _/etc/pkgmk.conf_ and remove -pipe from all your \*FLAGS=.

# SEE ALSO

*pkgman*(8), *pkgmk*(8), *pkgmk.conf*(5)

; vim:ft=scdoc:cc=80
; End of file.
