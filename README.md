ABOUT
-----
This directory contains *pkgman*, a package management tool that
provides additional functionality to basic package management
utilities like [pkgutils](https://github.com/zeppe-lin/pkgutils).

This *pkgman* distribution is a fork of CRUX' *prt-get* tool as of
commit da6bfb0 (Fri Oct 16 2020).  It was almost completely
rewritten from a "C-like" coding standard to high-level c++17
primitives.  This enabled aggressive optimization (-O3 and above).
So-called "prt-cache" functionality have been removed since there
is no need for *prt-cache* when there is Linux's PageCache+NVMe.
The man pages have been rewritten in POD format.  Build system
have been switched from autotools to make(1p).  A command-line
interface also have been rewritten.

The original sources can be downloaded from:
1. git://crux.nu/tools/prt-get.git                          (git)
2. https://crux.nu/gitweb/?p=tools/prt-get.git;a=summary    (web)

REQUIREMENTS
------------
**Build time:**
- c++17 compiler (gcc8+)
- POSIX sh(1p), make(1p) and "mandatory utilities"
- pod2man(1pm) from perl distribution

**Runtime:**
- POSIX sh(1p)
- pkgmk(8)
- pkgadd(8) and pkgrm(8)

**Tests:**
- podchecker(1pm) to check PODs for errors
- httpx(1) to check URLs for non-200 response code

INSTALL
-------
The shell command `make install` should build and install this
package.

The shell command `make check` should start some tests like
checking PODs for errors, checking URLs for non-200 response code,
etc.

LICENSE
-------
*pkgman* is licensed through the GNU General Public License v2 or
later <https://gnu.org/licenses/gpl.html>.
Read the COPYING file for copying conditions.
Read the COPYRIGHT file for copyright notices.


<!-- vim:sw=2:ts=2:sts=2:et:cc=72:tw=70
End of file. -->
