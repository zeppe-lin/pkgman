ABOUT
-----
This directory contains *pkgman*, a package management tool that
provides additional functionality to basic package management
utilities like [pkgutils][1].

This *pkgman* distribution is a fork of CRUX' *prt-get* tool as of
commit da6bfb0 (Fri Oct 16 2020).  It was almost completely rewritten
from a "C-like" coding standard to high-level c++17 primitives.  This
enabled aggressive optimization (-O3 and above).  So-called
*prt-cache* functionality have been removed since there is no
fundamental advantage in *prt-cache* when PageCache+NVMe are
available.  The man pages have been rewritten in POD format.  Build
system have been switched from autotools to POSIX make(1p).  A
command-line interface also have been rewritten.

The original sources can be downloaded from:
  1. git://crux.nu/tools/prt-get.git                        (git)
  2. https://crux.nu/gitweb/?p=tools/prt-get.git;a=summary  (web)

REQUIREMENTS
------------
Build time:
  * c++17 compiler (gcc8+)
  * POSIX sh(1p), make(1p) and "mandatory utilities"
  * pod2man(1pm) to build man pages

Runtime:
  * POSIX sh(1p) to execute pre/post scripts
  * [pkgmk][2] to build packages
  * [pkgutils][1] to add/update/remove packages

Tests:
  - podchecker(1pm) to check PODs for errors
  - curl(1) to check URLs for response code

INSTALL
-------
The shell commands `make && make install` should build and install
this package.  See *config.mk* file for configuration parameters.

The shell command `make check` should start some tests.

LICENSE
-------
*pkgman* is licensed through the GNU General Public License v2 or
later <https://gnu.org/licenses/gpl.html>.
Read the *COPYING* file for copying conditions.
Read the *COPYRIGHT* file for copyright notices.

<!------------------------------------------------------------------->
[1]: https://github.com/zeppe-lin/pkgutils
[2]: https://github.com/zeppe-lin/pkgmk
<!------------------------------------------------------------------->

<!-- vim:sw=2:ts=2:sts=2:et:cc=72:tw=70
End of file. -->
