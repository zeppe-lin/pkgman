if get_option('vimfiles')
  vimfilesdir = get_option('vimfilesdir')
  if vimfilesdir == ''
    vimfilesdir = get_option('datadir') / 'vim' / 'vimfiles'
  endif

  install_data('ftdetect/pkgmanconf.vim',
    install_dir: vimfilesdir / 'ftdetect',
  )
  install_data('syntax/pkgmanconf.vim',
    install_dir: vimfilesdir / 'syntax',
  )
endif
