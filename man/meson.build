if get_option('manpages')
  scdoc = find_program('scdoc')
  curdir = meson.current_source_dir()

  foreach section, pages: {
    '1': [
      'pkgman',
      'pkgman-cat',
      'pkgman-current',
      'pkgman-dep',
      'pkgman-diff',
      'pkgman-dsearch',
      'pkgman-dumpconfig',
      'pkgman-fsearch',
      'pkgman-info',
      'pkgman-isinst',
      'pkgman-list',
      'pkgman-list-dup',
      'pkgman-list-locked',
      'pkgman-list-nodependents',
      'pkgman-list-orphans',
      'pkgman-ls',
      'pkgman-path',
      'pkgman-printf',
      'pkgman-rdep',
      'pkgman-readme',
      'pkgman-search',
    ],
    '5': [
      'pkgman.conf',
    ],
    '7': [
      #'collections',
      'packaging',
      'pkgman-readme',
      'pkgman-runscripts',
    ],
    '8': [
      'pkgman-edit',
      'pkgman-install',
      'pkgman-lock',
      'pkgman-remove',
      'pkgman-sysup',
      'pkgman-unlock',
      'pkgman-update',
    ],
  }
    foreach page: pages
      name = page + '.' + section

      generated = configure_file(
        input: curdir / (name + '.scd.in'),
        output: name + '.scd',
        configuration: conf,
      )

      custom_target(
        name,
        input: generated,
        output: name,
        command: [scdoc],
        feed: true,
        capture: true,
        install: true,
        install_dir: get_option('mandir') / ('man' + section),
        install_tag: 'man',
      )
    endforeach
  endforeach
endif
