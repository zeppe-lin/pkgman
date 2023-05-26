OVERVIEW
--------
This directory contains pkgman, a package management tool that
provides additional functionality to basic package management
utilities like pkgmk and pkgutils.

This distribution is a fork of CRUX' prt-get tool as of commit da6bfb0
(Fri Oct 16 2020) with the following differences:

- the code has been rewritten from "C-like" coding standard to
  high-level C++17 primitives; this enabled aggressive optimization
  (-O3 and above), look at
  https://crux.nu/ports/crux-3.7/core/prt-get/README vs
  https://github.com/zeppe-lin/pkgsrc/blob/master/core/pkgman/Pkgfile#L14
- doxygen documentation has been improved and covers (almost?) all
  code
- so-called prt-cache functionality have been removed since there is
  no fundamental advantage while PageCache+NVMe are available
- prt.aliases functionality have been removed too
- the man pages have been rewritten in POD format
- build system have been switched from autotools to POSIX make(1p)
- the command-line interface have been rewritten
- the commands output have been unified and improved
- added new commands
- added dependency injection for sysup/update/remove commands
- quiet mode
- and so on, and so on...

See git log for complete/further differences and
[COMPLEXITY](README.md#complexity) section for source code stats'
differences.

The original sources can be downloaded from:
1. git://crux.nu/tools/prt-get.git                        (git)
2. https://crux.nu/gitweb/?p=tools/prt-get.git;a=summary  (web)

COMPLEXITY
----------
In general, the number of changes and their impact reflects well
[scc](https://github.com/boyter/scc) report:

**prt-get**:
```
────────────────────────────────────────────────────────────────────────
Language          Files     Lines   Blanks  Comments     Code Complexity
────────────────────────────────────────────────────────────────────────
Autoconf              6      2299      276        94     1929        409
C Header             17      1241      259       278      704          5
C++                  18      6055      853       722     4480       1181
License               1       279       39         0      240          0
Shell                 5      7285      588       814     5883        717
m4                    1       984       99       380      505          0
────────────────────────────────────────────────────────────────────────
Total                48     18143     2114      2288    13741       2312
────────────────────────────────────────────────────────────────────────
Estimated Cost to Develop (organic) $423,169
Estimated Schedule Effort (organic) 9.92 months
Estimated People Required (organic) 3.79
────────────────────────────────────────────────────────────────────────
Processed 545036 bytes, 0.545 megabytes (SI)
────────────────────────────────────────────────────────────────────────
```

**pkgman**:
```
────────────────────────────────────────────────────────────────────────
Language          Files     Lines   Blanks  Comments     Code Complexity
────────────────────────────────────────────────────────────────────────
C Header             17      2679      491      1327      861         33
C++                  17      5014      984       285     3745        911
License               2       287       39         0      248          0
Makefile              2        75       16         4       55          7
Markdown              1        62       11         0       51          0
YAML                  2        65       13         2       50          0
────────────────────────────────────────────────────────────────────────
Total                41      8182     1554      1618     5010        951
────────────────────────────────────────────────────────────────────────
Estimated Cost to Develop (organic) $146,698
Estimated Schedule Effort (organic) 6.63 months
Estimated People Required (organic) 1.97
────────────────────────────────────────────────────────────────────────
Processed 220697 bytes, 0.221 megabytes (SI)
────────────────────────────────────────────────────────────────────────
```


REQUIREMENTS
------------
**Build time**:
- C++17 compiler (GCC 8 and later, Clang 5 and later)
- POSIX sh(1p), make(1p) and "mandatory utilities"
- pod2man(1pm) to build man pages

**Runtime**:
- POSIX sh(1p) to execute pre/post scripts
- pkgmk to build packages
- pkgutils to add/update/remove packages


INSTALL
-------
The shell commands `make && make install` should build and install
this package.  See `config.mk` file for configuration parameters,
and `pathnames.h` for absolute filenames that pkgman wants for
various defaults.


LICENSE
-------
pkgman is licensed through the GNU General Public License v2 or later
<https://gnu.org/licenses/gpl.html>.
Read the COPYING file for copying conditions.
Read the COPYRIGHT file for copyright notices.
