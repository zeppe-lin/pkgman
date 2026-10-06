# NEWS

## pkgman 6.3 — Unreleased

This release modernizes pkgman's build and package-database foundations while
preserving its command-line model.  It also tightens subprocess/job-control
behavior and rebuilds Bash completion around the documented command grammar.

### Process execution and transactions

* Run build and package-management subprocesses in dedicated process groups and
  hand them the controlling terminal while they are active, so terminal signals
  reach wrapper chains such as `sudo`, `nice`, `ionice`, and `fakeroot`.
* Restore normal interactive job-control behavior for stopped jobs, including
  `Ctrl+Z`, `fg`, and `bg`.
* Decode `waitpid(2)` status correctly, retry interrupted waits, and preserve
  real subprocess exit codes instead of accidentally treating failures as
  success.
* Make logging reads non-blocking and drain them safely without deadlocking a
  still-running child.
* Separate build-step and package-step results inside transactions so pkgmk,
  pkgadd, and pkgrm failures retain their actual status and the final
  transaction result reflects collected failures.

### Package database

* Replace pkgman's private installed-package database reader with
  `libpkgcore(7)` and use its package metadata as the canonical installed-state
  representation.
* Fix dependency-sorted package listings when repository packages are not
  installed.
* Preserve the established `diff --deps` behavior for dependencies that exist
  in repositories but are not installed.
* Add regression coverage proving that `--root` reaches the alternate package
  database and lock state rather than consulting the host root.

### Command line and completion

* Accept the documented `-i` pkgadd-force shorthand for install, update, and
  sysup while retaining the historical `-fi` spelling for compatibility.
* Rebuild Bash completion from the public manual-page grammar instead of a
  positional monolithic case statement.
* Complete generic `--root=` and `--config=` values, preserving alternate-root
  context in package-state queries.
* Support repeated comma-separated package completion for `--ignore=` and
  documented pass-through flags for `--margs=` and `--aargs=`.
* Complete package source files for commands such as `cat` and `edit`, and use
  the appropriate available, installed, outdated, or locked package population
  for each command.
* Add an isolated Bash completion regression suite, including Bash word-break
  handling for attached option values.

### Build and integration

* Replace the legacy Makefile build with Meson and Ninja.
* Add shared/static dependency selection through the `link_mode` option.
* Add `libpkgcore` as an explicit dependency and report its linked version in
  `pkgman --version` output.
* Generate package metadata and installation paths from Meson configuration and
  substitute the same build-time paths into generated manual pages.
* Restructure CI around Meson builds covering glibc and musl with shared and
  static link modes.
* Flatten and install the Bash completion and Vim support through Meson.

### Documentation and maintenance

* Expand and reorganize the package README authoring guidance in
  `pkgman-readme(7)` and refine `pkgman-runscripts(7)`.
* Add configured package/version footers, canonical bug-report links, and
  build-time path substitution throughout the manual pages.
* Remove obsolete Makefile-only tests, build fragments, complexity statistics,
  and completed TODO tracking in favor of the issue tracker.
* Clean up compiler warnings without changing the established runtime behavior
  of shell-command expansion or command-argument ownership.

### Compatibility

The source build now requires Meson, Ninja, and libpkgcore.  Existing pkgman
command syntax is otherwise preserved; compatibility aliases retained by the
parser are not newly advertised by completion.

## pkgman 6.2 — 2025-12-12

A documentation-focused release with broad cleanup across manual pages,
configuration references, completion scripts, and build tooling.

### Manual pages

* Refine the pkgman manual set for formatting, readability, terminology, and
  consistent scdoc markup.
* Standardize SYNOPSIS sections and fix rendering issues.
* Expand descriptions, file formats, exit-status documentation, examples, and
  cross-references.
* Add `packaging(7)`, `pkgman-readme(7)`, and `pkgman-runscripts(7)`.
* Fix URLs, references, escaping, and layouts for small terminals.

### Core functionality

* Add `--dependents` to `list-orphans`.
* Fix incorrect or unused command-line option handling.
* Clean up pkgman internals and improve source documentation.

### Bash completion

* Complete the new `--dependents` option.
* Fix package listing and `--filter=` completion.
* Apply general style and correctness improvements.

### Build and repository

* Remove obsolete lint targets from the top-level and auxiliary Makefiles.
* Migrate all manual pages from mdoc to scdoc.
* Update CI workflows and release integration.
* Add a release target and simplify recursive Makefile handling.
* Clean up README, TODO, ignore rules, and copyright metadata.
