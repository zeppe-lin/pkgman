if get_option('bashcomp')
  bashcompdir = get_option('bashcompdir')
  if bashcompdir == ''
    bashcompdir = get_option('datadir') / 'bash-completion' / 'completions'
  endif

  install_data('bash_completion',
    install_dir: bashcompdir,
    rename: 'pkgman')
endif
