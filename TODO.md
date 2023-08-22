TODO pkgman
===========

- [ ] enhancement: rework bash_completion.

----------------------------------------------------------------------

- [ ] enhancement: grep 'XXX\|FIXME\|TODO' --exclude-dir=.git  -R .

----------------------------------------------------------------------

- [ ] issue: if package is already built with .footprint mismatch,
      pkgman will install it

----------------------------------------------------------------------

- [ ] issue: skip trying to pkgadd the package if previous pkgmk
execution returns "footprint check failure":
```
=======> ERROR: Footprint mismatch found:
[...]
pkgman: makecommand: [package]: footprint check failure
pkgman: addcommand: [package]: unknown error  <========
```

----------------------------------------------------------------------

- [ ] issue: investigate the following behaviour:
```
# pkgman sysup --depsort --deps --group
pkgman: preparing libcap-ng 0.8.3-1
pkgman: md5sum verification failed.
 
-- Packages where build failed
libcap-ng

pkgman: preparing alsa-lib 1.2.7.1-1
^Cpkgman: interrupted
```

----------------------------------------------------------------------

- [ ] issue: investigate the following behaviour:
```
pkgrm: could not remove /usr/share/icons/Mint-X-Teal/:
Directory not empty
pkgrm: package libsass not installed
pkgrm: could not remove /usr/share/icons/Raleigh/:
Directory not empty

-- Packages where removal failed
libsass

-- Packages removed
sassc
libsass
mint-x-icons
mint-y-icons
mint-themes
gtk-theme-raleigh
```

----------------------------------------------------------------------

- [ ] idea: ansi colors?
The consensus is that the ASCII is better? Just format the output like
the following:
```
=======> This is an example of simple informational message.
=======> WARNING: This is an example of warning message.
=======> ERROR: This is an example of error message.
```

----------------------------------------------------------------------

- [ ] idea: add "ignore" directive to pkgman.conf?

----------------------------------------------------------------------

DONE
====

----------------------------------------------------------------------

- [x] rename config.{cpp,h} -> configparser.{cpp,h}
- [x] rename "Config" class and fix the impacted code
      Done: Thu Mar 16 10:31:04 AM EET 2023

----------------------------------------------------------------------

- [x] issue: pkgman --config-append="runscripts no" doesn't working?
      Done: Fri Aug 18 07:27:40 PM EEST 2023
      Fixed by 4251b0ba4e77804fc2aaca24d251f753cf162d45.
