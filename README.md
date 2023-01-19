ABOUT
-----
This directory contains _pkgman_, a package management tool that
provides additional functionality to basic package management
utilities like [pkgmk][1] and [pkgutils][2].

This _pkgman_ distribution is a fork of CRUX' _prt-get_ tool as of
commit da6bfb0 (Fri Oct 16 2020) with the following differences:
  * The code has been rewritten from "C-like" coding standard to
    high-level C++17 primitives.  This enabled aggressive optimization
    (`-O3` and above).
  * Doxygen documentation has been improved and covers (almost?) all
    code.
  * So-called _prt-cache_ functionality have been removed since there
    is no fundamental advantage while _PageCache_+_NVMe_ are
    available.
  * The man pages have been rewritten in POD format.
  * Build system have been switched from autotools to POSIX make(1p).
  * The command-line interface have been rewritten.
  * The commands output have been unified and improved.
  * Added new commands.

See git log for complete/further differences.

The original sources can be downloaded from:
  1. git://crux.nu/tools/prt-get.git                        (git)
  2. https://crux.nu/gitweb/?p=tools/prt-get.git;a=summary  (web)

REQUIREMENTS
------------
Build time:
  * C++17 compiler (gcc8+)
  * POSIX sh(1p), make(1p) and "mandatory utilities"
  * pod2man(1pm) to build man pages

Runtime:
  * POSIX sh(1p) to execute pre/post scripts
  * [pkgmk][1] to build packages
  * [pkgutils][2] to add/update/remove packages

Tests:
  * podchecker(1pm) to check PODs for errors
  * curl(1) to check URLs for response code

INSTALL
-------
The shell commands `make && make install` should build and install
this package.  See _config.mk_ file for configuration parameters.

The shell command `make check` should start some tests.

LICENSE
-------
_pkgman_ is licensed through the GNU General Public License v2 or
later <https://gnu.org/licenses/gpl.html>.
Read the _COPYING_ file for copying conditions.
Read the _COPYRIGHT_ file for copyright notices.

[1]: https://github.com/zeppe-lin/pkgmk
[2]: https://github.com/zeppe-lin/pkgutils

<!-- vim:sw=2:ts=2:sts=2:et:cc=72:tw=70
End of file. -->
