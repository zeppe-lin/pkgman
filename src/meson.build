common_deps = [libpkgcore_dep, libpkgstate_dep]
common_incs = []
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

sources = files(
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

conf_h = configuration_data()

foreach k, v: buildmeta
  conf_h.set_quoted(k, v)
endforeach

configure_file(
  output: 'pkgman-config.h',
  configuration: conf_h,
)

executable(
  'pkgman',
  sources,
  kwargs: exe_kwargs,
  install_dir: get_option('bindir')
)
