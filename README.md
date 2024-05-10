OVERVIEW
========

This directory contains pkgman, a package management tool that provides
additional functionality to basic package management utilities like pkgmk and
pkgutils.

This distribution is a fork of CRUX' prt-get tool as of commit da6bfb0 (Fri Oct
16 2020) with the following differences:
  * the code has been rewritten from "C-like" coding standard to high-level
    C++17 primitives; this enabled aggressive optimization (-O3 and above),
    look at [prt-get][1] vs [pkgman][2]
  * doxygen documentation has been improved and covers (almost?) all code
  * so-called prt-cache functionality have been removed since there is no
    fundamental advantage while PageCache+NVMe are available
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

See git log for complete/further differences and [COMPLEXITY.md][3] for source
code stats' differences.

[1]: https://crux.nu/ports/crux-3.7/core/prt-get/README
[2]: https://github.com/zeppe-lin/pkgsrc-core/blob/master/pkgman/Pkgfile#L14
[3]: https://github.com/zeppe-lin/pkgman/blob/master/COMPLEXITY.md

The original sources can be downloaded from:
  1. git://crux.nu/tools/prt-get.git                        (git)
  2. https://crux.nu/gitweb/?p=tools/prt-get.git;a=summary  (web)


REQUIREMENTS
============

Build time
----------
  * C++17 compiler (GCC 8 and later, Clang 5 and later)
  * POSIX sh(1p) and "mandatory utilities"
  * GNU make(1)

Runtime
-------
  * POSIX sh(1p) to execute pre/post scripts
  * pkgmk to build packages
  * pkgutils to add/update/remove packages


INSTALL
=======

The shell commands `make && make install` should build and install this
package.  The command `make install_bashcomp` should install bash completion
script, and the command `make install_vimfiles` should install vim syntax
highlight for `pkgman.conf` file.

See `config.mk` file for configuration parameters, and `src/pathnames.h` for
absolute filenames that pkgman wants for various defaults.


LICENSE
=======

pkgman is licensed through the GNU General Public License v2 or later
<https://gnu.org/licenses/gpl.html>.
Read the COPYING file for copying conditions.
Read the COPYRIGHT file for copyright notices.
