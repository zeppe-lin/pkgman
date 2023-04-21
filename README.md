OVERVIEW
--------
This directory contains pkgman, a package management tool that provides
additional functionality to basic package management utilities like pkgmk and
pkgutils.

This distribution is a fork of CRUX' prt-get tool as of commit da6bfb0 (Fri Oct
16 2020) with the following differences:

- the code has been rewritten from "C-like" coding standard to high-level C++17
  primitives; this enabled aggressive optimization (-O3 and above), look at
  https://crux.nu/ports/crux-3.7/core/prt-get/README vs
  https://github.com/zeppe-lin/pkgsrc/blob/master/core/pkgman/Pkgfile#L14
- doxygen documentation has been improved and covers (almost?) all code
- so-called prt-cache functionality have been removed since there is no
  fundamental advantage while PageCache+NVMe are available
- prt.aliases functionality have been removed too
- the man pages have been rewritten in POD format
- build system have been switched from autotools to POSIX make(1p)
- the command-line interface have been rewritten
- the commands output have been unified and improved
- added new commands
- added dependency injection for sysup/update/remove commands
- quiet mode
- and so on, and so on...

See git log for complete/further differences.

The original sources can be downloaded from:
1. git://crux.nu/tools/prt-get.git                        (git)
2. https://crux.nu/gitweb/?p=tools/prt-get.git;a=summary  (web)


REQUIREMENTS
------------
**Build time**:
- c++17 compiler (gcc 8 and later, clang 5 and later)
- POSIX sh(1p), make(1p) and "mandatory utilities"
- pod2man(1pm) to build man pages

**Runtime**:
- POSIX sh(1p) to execute pre/post scripts
- pkgmk to build packages
- pkgutils to add/update/remove packages


INSTALL
-------
The shell commands `make && make install` should build and install this
package.  See `config.mk` file for configuration parameters.


LICENSE
-------
pkgman is licensed through the GNU General Public License v2 or later
<https://gnu.org/licenses/gpl.html>.
Read the COPYING file for copying conditions.
Read the COPYRIGHT file for copyright notices.
