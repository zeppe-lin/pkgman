TODO pkgman
===========


Unknown milestone
-----------------

- [ ] Enhancement: rework bash_completion.

---

- [ ] Enhancement: clean up the code a bit.

    grep 'XXX\|FIXME\|TODO' --exclude-dir=.git  -R .

---

- [ ] Enhancement: --group option misleading.  May be call it better
  --fail-on-error? and add --continue-on-error which will be default?
  Or one option width different arguments, like:

    --on-error=fail|continue?

  The --group option should remain for a while, but deprecation note
  should be added in case we rename it.

---

- [ ] Enhancement: --test option misleading.  May be call it better
  --dry-run?  All considerations for --group option is applicable
  there.

---

Is these two issues about the same wtf?

- [ ] Issue: if package is already built with .footprint mismatch,
  pkgman will install it.  It's bad? Good? Wtf is it? *^_^*

- [ ] Issue: skip trying to pkgadd the package if previous pkgmk
execution returns "footprint check failure":

    =======> ERROR: Footprint mismatch found:
    [...]
    pkgman: makecommand: [package]: footprint check failure
    pkgman: addcommand: [package]: unknown error  <========

---

- [ ] Issue: investigate the following wtf behaviour:

    pkgrm: could not remove /usr/share/icons/Mint-X-Teal/:
    Directory not empty
    pkgrm: package libsass not installed
    pkgrm: could not remove /usr/share/icons/Raleigh/:
    Directory not empty

    -- Packages where removal failed
    libsass

    -- Packages removed
    sassc
    libsass
    mint-x-icons
    mint-y-icons
    mint-themes
    gtk-theme-raleigh

---

- [ ] (bad) Idea: add ansi colors?  The consensus is that the ASCII is
  better? Just format the output like the following:

    =======> This is an example of simple informational message.
    =======> WARNING: This is an example of warning message.
    =======> ERROR: This is an example of error message.

---

- [ ] (normal) Idea: add "ignore" directive to pkgman.conf to ignore
  specified packages like --ignore option do?  Pitfails?

---

Caution: This issue description feels like a drunk person's train of
thought.

- [ ] sysup --deps --depsort case #1:
    e.g. we have mpv (installed), which depends on libplacebo
    (uninstalled), which depends on py3-markupsafe (installed, but
    need upgraded).  The sysup --deps --depsort fail, 'cause pkgman
    tries to install libplacebo without updateing the deps:
    py3-markupsafe is broken, it needs to be updated first, the built
    and installed libplacebo, and only then to update mpv.


DONE
----

----------------------------------------------------------------------

- [x] Refactor: rename config.{cpp,h} -> configparser.{cpp,h}
- [x] Refactor: rename "Config" class and fix the impacted code
      Done: Thu Mar 16 10:31:04 AM EET 2023

----------------------------------------------------------------------

- [x] Issue: pkgman --config-append="runscripts no" doesn't working?
  Done: Fri Aug 18 07:27:40 PM EEST 2023
  Fixed by 4251b0ba4e77804fc2aaca24d251f753cf162d45.

-----------------------------------------------------------------------

- [x] Issue: respect --group option for sysup operation

    # pkgman sysup --depsort --deps --group
    pkgman: preparing libcap-ng 0.8.3-1
    pkgman: md5sum verification failed.

    -- Packages where build failed
    libcap-ng

    pkgman: preparing alsa-lib 1.2.7.1-1
    ^Cpkgman: interrupted

  Done: Tue Oct  1 04:46:49 PM EEST 2024
  Fixed by db54ab223bf615c5b4b2c84951bb55dd31d7b3ca
