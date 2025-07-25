OVERVIEW
========

This repository contains `pkgman`, a package management tool that
provides additional functionality to basic package management
utilities like `pkgmk` and `pkgutils`.

This distribution is a fork of CRUX' `prt-get` tool as of commit
da6bfb0 (Fri Oct 16 2020) with the following differences:
  * the code has been rewritten from "C-like" coding standard to
    high-level C++17 primitives; this enabled aggressive optimization
    (`-O3` and above), look at [prt-get][1] vs [pkgman][2]
  * doxygen documentation has been improved and covers (almost?) all
    code
  * so-called `prt-cache` functionality have been removed since there
    is no fundamental advantage while PageCache+NVMe are available
  * `prt.aliases` functionality have been removed too
  * manual pages in `scdoc(5)` format
  * new manual pages: `pkgman-readme(7)` & `pkgman-runscripts(7)`
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
  * POSIX `sh(1p)`, `make(1p)` and "mandatory utilities"
  * `scdoc(1)` to build manual pages

Runtime
-------
  * POSIX `sh(1p)` to execute pre/post scripts (configurable)
  * `pkgmk` to build packages (configurable)
  * `pkgutils` to add/update/remove packages (configurable)


INSTALL
=======

To build and install this package:

    make && make install

For static linking you need to run `make(1p)` as the following:

    make LDFLAGS="-static -lstdc++fs"

See `config.mk` file for configuration parameters, and
`src/pathnames.h` for absolute filenames that `pkgman` wants for
various defaults.


DOCUMENTATION
=============

See `/man` directory for manual pages.


LICENSE
=======

`pkgman` is licensed through the GNU General Public License v2 or
later <https://gnu.org/licenses/gpl.html>.
Read the COPYING file for copying conditions.
Read the COPYRIGHT file for copyright notices.
