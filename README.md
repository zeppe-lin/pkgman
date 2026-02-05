OVERVIEW
========

`pkgman` is a package management tool that extends the functionality
of basic utilities such as `pkgmk` and `pkgutils`.

It is a fork of CRUX `prt-get` at commit da6bfb0 (Fri Oct 16 2020)
with the following differences:
  * Code rewritten from "C-like" style to modern C++17 primitives,
    enabling aggressive optimization (`-O3` and above);
    see [prt-get][1] vs [pkgman][2]
  * Expanded doxygen documentation covering nearly all code
  * Removed `prt-cache` (no fundamental advantage with PageCache +
    NVMe available)
  * Removed `prt.aliases`
  * Manual pages in `scdoc(5)` format
  * New manual pages: `pkgman-readme(7)` and `pkgman-runscripts(7)`
  * Switched from autotools to Makefile
  * Rewritten command-line interface
  * Unified and improved command output
  * Added new commands
  * Dependency injection for sysup/update/remove commands
  * Quiet mode
  * Vim syntax highlight for `pkgman.conf`
  * Various other improvements

See the git log for full history and [COMPLEXITY.md][3] for source
code statistics.

[1]: https://git.crux.nu/ports/core/src/branch/3.7/prt-get/README
[2]: https://github.com/zeppe-lin/pkgsrc-core/blob/master/pkgman/Pkgfile#L14
[3]: https://github.com/zeppe-lin/pkgman/blob/master/COMPLEXITY.md

Original sources:
  * https://git.crux.nu/tools/prt-get.git

---

REQUIREMENTS
============

Build-time
----------
  * C++17 compiler (GCC 8+, Clang 5+)
  * POSIX `sh(1p)`, `make(1p)`, and "mandatory utilities"
  * `scdoc(1)` to generate manual pages

Runtime
-------
  * POSIX `sh(1p)` to execute pre/post scripts (configurable)
  * `pkgmk` to build packages (configurable)
  * `pkgutils` to add/update/remove packages (configurable)

---

INSTALLATION
============

To build and install:

```sh
make
make install   # as root
```

For static linking:

```sh
make LDFLAGS="-static -lstdc++fs"
```

Configuration parameters are defined in `config.mk`.  
Default file paths are specified in `src/pathnames.h`.

---

DOCUMENTATION
=============

Manual pages are provided in `/man` and installed under the system
manual hierarchy.

---

LICENSE
=======

`pkgman` is licensed under the
[GNU General Public License v2 or later](https://gnu.org/licenses/gpl.html).

See `COPYING` for license terms and `COPYRIGHT` for notices.
