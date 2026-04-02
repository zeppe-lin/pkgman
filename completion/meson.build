if get_option('bashcomp')
  bashcompdir = get_option('bashcompdir')
  if bashcompdir == ''
    bashcompdir = get_option('datadir') / 'bash-completion' / 'completions'
  endif

  install_data('pkgman.bash', install_dir: bashcompdir, rename: 'pkgman')
endif
