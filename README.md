pkgman - advanced package manager
=================================
pkgman is a package management tool that provides additional
functionality to basic package management utilities like
[pkgutils](https://github.com/zeppe-lin/pkgutils).

This pkgman distribution is a fork of CRUX' prt-get tool as of commit
da6bfb0 (Fri Oct 16 2020).  It was almost completely rewritten from a
"C-like" coding standard to high-level c++17 primitives.  This enabled
aggressive optimization (`-O3`++).  So-called "prt-cache"
functionality have been removed since there is no need for "prt-cache"
when there is Linux' "PageCache" + NVMe.  The man pages have been
rewritten in POD (Plain Old Documentation).  A command-line interface
as well as build files were rewritten too.

The original sources can be downloaded from:

  1.
    git clone git://crix.nu/tools/prt-get.git
    git checkout da6bfb0

  2. https://crux.nu/gitweb/?p=tools/prt-get.git;a=summary


Dependencies
------------
Build time:
- c++17 compiler (gcc8+, -std=c++17)
- make(1p) and POSIX utilities like mkdir(1p), cp(1p), ln(1p), rm(1p),
  sed(1p)
- podchecker(1pm) and pod2man(1pm) from perl distribution

Runtime:
- pkgmk(1) from pkgmk distribution
- pkgadd(1) and pkgrm(1) from pkgutils distribution
- sh(1p)


Install
-------
The shell commands `make; make install` should build and install this
package.  See `config.mk` file for configuration parameters.


License and Copyright
---------------------
pkgman is licensed through the GNU General Public License v2 or later
<https://gnu.org/licenses/gpl.html>.
Read the COPYING file for copying conditions.
Read the COPYRIGHT file for copyright notices.


<!-- vim:ft=markdown:sw=2:ts=2:sts=2:et:cc=72:tw=70
End of file. -->
