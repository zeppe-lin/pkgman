if get_option('vimfiles')
  vimfilesdir = get_option('vimfilesdir')
  if vimfilesdir == ''
    vimfilesdir = get_option('datadir') / 'vim' / 'vimfiles'
  endif

  install_data('ftdetect.vim',
    install_dir: vimfilesdir / 'ftdetect',
    rename: 'pkgmanconf.vim')

  install_data('syntax.vim',
    install_dir: vimfilesdir / 'syntax',
    rename: 'pkgmanconf.vim')
endif
