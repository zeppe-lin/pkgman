OVERVIEW
========

This repository contains pkgman, a package management tool that
provides additional functionality to basic package management
utilities like pkgmk and pkgutils.

This distribution is a fork of CRUX' prt-get tool as of commit da6bfb0
(Fri Oct 16 2020) with the following differences:
  * the code has been rewritten from "C-like" coding standard to
    high-level C++17 primitives; this enabled aggressive optimization
    (-O3 and above), look at [prt-get][1] vs [pkgman][2]
  * doxygen documentation has been improved and covers (almost?) all
    code
  * so-called prt-cache functionality have been removed since there is
    no fundamental advantage while PageCache+NVMe are available
  * prt.aliases functionality have been removed too
  * manual pages in mdoc(7) format
  * switch from autotools to Makefile
  * rewritten command-line interface
  * unified and improved commands output
  * new commands
  * dependency injection for sysup/update/remove commands
  * quiet mode
  * vim syntax highlight for `pkgman.conf` file
  * etc

See git log for complete/further differences and [COMPLEXITY.md][3]
for source code stats' differences.

[1]: https://git.crux.nu/ports/core/src/branch/3.7/prt-get/README
[2]: https://github.com/zeppe-lin/pkgsrc-core/blob/master/pkgman/Pkgfile#L14
[3]: https://github.com/zeppe-lin/pkgman/blob/master/COMPLEXITY.md

The original sources can be downloaded from:
  * https://git.crux.nu/tools/prt-get.git


REQUIREMENTS
============

Build time
----------
  * C++17 compiler (GCC 8 and later, Clang 5 and later)
  * POSIX sh(1p), make(1p) and "mandatory utilities"

Runtime
-------
  * POSIX sh(1p) to execute pre/post scripts (configurable)
  * pkgmk to build packages (configurable)
  * pkgutils to add/update/remove packages (configurable)


INSTALL
=======

The shell commands `make && make install` should build and install
this package.

For static linking you need to run `make` as the following:

```sh
make LDFLAGS="-static -lstdc++fs"
```

See `config.mk` file for configuration parameters, and
`src/pathnames.h` for absolute filenames that pkgman wants for various
defaults.


DOCUMENTATION
=============

Online documentation
--------------------

Manual pages:
- [pkgman.1](https://zeppe-lin.github.io/pkgman.1.html)
- [pkgman-cat.1](https://zeppe-lin.github.io/pkgman-cat.1.html)
- [pkgman.conf.5](https://zeppe-lin.github.io/pkgman.conf.5.html)
- [pkgman-current.1](https://zeppe-lin.github.io/pkgman-current.1.html)
- [pkgman-dep.1](https://zeppe-lin.github.io/pkgman-dep.1.html)
- [pkgman-diff.1](https://zeppe-lin.github.io/pkgman-diff.1.html)
- [pkgman-dsearch.1](https://zeppe-lin.github.io/pkgman-dsearch.1.html)
- [pkgman-dumpconfig.1](https://zeppe-lin.github.io/pkgman-dumpconfig.1.html)
- [pkgman-edit.8](https://zeppe-lin.github.io/pkgman-edit.8.html)
- [pkgman-fsearch.1](https://zeppe-lin.github.io/pkgman-fsearch.1.html)
- [pkgman-info.1](https://zeppe-lin.github.io/pkgman-info.1.html)
- [pkgman-install.8](https://zeppe-lin.github.io/pkgman-install.8.html)
- [pkgman-isinst.1](https://zeppe-lin.github.io/pkgman-isinst.1.html)
- [pkgman-list.1](https://zeppe-lin.github.io/pkgman-list.1.html)
- [pkgman-list-dup.1](https://zeppe-lin.github.io/pkgman-list-dup.1.html)
- [pkgman-list-locked.1](https://zeppe-lin.github.io/pkgman-list-locked.1.html)
- [pkgman-list-nodependents.1](https://zeppe-lin.github.io/pkgman-list-nodependents.1.html)
- [pkgman-list-orphans.1](https://zeppe-lin.github.io/pkgman-list-orphans.1.html)
- [pkgman-lock.8](https://zeppe-lin.github.io/pkgman-lock.8.html)
- [pkgman-ls.1](https://zeppe-lin.github.io/pkgman-ls.1.html)
- [pkgman-path.1](https://zeppe-lin.github.io/pkgman-path.1.html)
- [pkgman-printf.1](https://zeppe-lin.github.io/pkgman-printf.1.html)
- [pkgman-rdep.1](https://zeppe-lin.github.io/pkgman-rdep.1.html)
- [pkgman-readme.1](https://zeppe-lin.github.io/pkgman-readme.1.html)
- [pkgman-remove.8](https://zeppe-lin.github.io/pkgman-remove.8.html)
- [pkgman-search.1](https://zeppe-lin.github.io/pkgman-search.1.html)
- [pkgman-sysup.8](https://zeppe-lin.github.io/pkgman-sysup.8.html)
- [pkgman-unlock.8](https://zeppe-lin.github.io/pkgman-unlock.8.html)
- [pkgman-update.8](https://zeppe-lin.github.io/pkgman-update.8.html)


LICENSE
=======

pkgman is licensed through the GNU General Public License v2 or later
<https://gnu.org/licenses/gpl.html>.
Read the COPYING file for copying conditions.
Read the COPYRIGHT file for copyright notices.
