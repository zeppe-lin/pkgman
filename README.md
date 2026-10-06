OVERVIEW
========

**pkgman** is a package management tool that extends the functionality
of basic utilities such as [**pkgmk**][pkgmk] and
[**pkgutils**][pkgutils].

Originally forked from CRUX [**prt-get**][prt-get] at commit da6bfb0
(Fri Oct 16 2020), **pkgman** has since been modernized and developed
independently.

See the git log for full history.

---

REQUIREMENTS
============

Build-time
----------
  * C++17 compiler (GCC 8+, Clang 5+)
  * Meson
  * Ninja
  * [libpkgcore][libpkgcore]
  * `scdoc(1)` to generate manual pages
    (enabled by default via the `manpages` option)
  * `pkg-config(1)` for dependency discovery

Runtime
-------
  * POSIX `sh(1p)` to execute pre/post scripts (configurable)
  * `pkgmk` to build packages (configurable)
  * `pkgutils` to add/update/remove packages (configurable)

---

INSTALLATION
============

General
-------

Configure and build with Meson:

```sh
meson setup build \
    --prefix=/usr \
    --sysconfdir=/etc \
    --buildtype=release \
    --wrap-mode=nodownload \

ninja -C build
```

Install:

```sh
DESTDIR="$PKG" ninja -C build install
```

Common options:

```sh
meson setup build ... \
    -D link_mode=static \
    -D manpages=true \
    -D bashcomp=true \
    -D vimfiles=true \
```

Use `meson configure build` to inspect available options.

Link Mode
---------

`pkgman` can be built against shared or static external dependencies
(e.g., [libpkgcore][libpkgcore]).

Shared:

```sh
meson setup build ... -D link_mode=shared
meson compile -C build
```

Static:

```sh
meson setup build ... -D link_mode=static
meson compile -C build
```

For generic static packaging, keep LTO disabled unless the whole
toolchain is prepared for static LTO archives.

> [!IMPORTANT]
> Static builds disable PIE (position-independent executables),
> since PIE requires a dynamic loader and is incompatible with
> `-static`.

---

DOCUMENTATION
=============

Manual pages are provided in `/man` and installed under the system
manual hierarchy.

Release notes are maintained in [NEWS.md](NEWS.md).

---

LICENSE
=======

`pkgman` is licensed under the
[GNU General Public License v2 or later][gpllicense].

See [COPYING](COPYING) for license terms and
[COPYRIGHT](COPYRIGHT) for notices.

---

[pkgmk]: https://github.com/zeppe-lin/pkgmk
[pkgutils]: https://github.com/zeppe-lin/pkgutils
[libpkgcore]: https://github.com/zeppe-lin/libpkgcore
[prt-get]: https://git.crux.nu/tools/prt-get.git
[gpllicense]: https://gnu.org/licenses/gpl.html
