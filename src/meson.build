common_deps = [libpkgcore_dep]
common_incs = [internal_inc]
common_link_args = []

want_static = get_option('link_mode') == 'static'
if want_static
  common_link_args += ['-static']
endif

exe_kwargs = {
  'dependencies': common_deps,
  'include_directories': common_incs,
  'link_args': common_link_args,
  'install': true,
}

pkgman_srcs = files(
  'argparser.cpp',
  'configparser.cpp',
  'datafileparser.cpp',
  'depresolver.cpp',
  'helpers.cpp',
  'locker.cpp',
  'main.cpp',
  'package.cpp',
  'pkgdb.cpp',
  'pkgman.cpp',
  'pkgmksetting.cpp',
  'p_regex.cpp',
  'process.cpp',
  'repository.cpp',
  'signaldispatcher.cpp',
  'transaction.cpp',
  'versioncomparator.cpp',
)

executable('pkgman', pkgman_srcs, kwargs: exe_kwargs, install_dir: get_option('bindir'))
