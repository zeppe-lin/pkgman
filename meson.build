project(
  'pkgman',
  'cpp',
  version: '6.2',
  license: 'GPL-2.0-or-later',
  meson_version: '>=1.2.0',
  default_options: [
    'cpp_std=c++17',
    'warning_level=3',
    'b_ndebug=if-release',
  ],
)

want_static = get_option('link_mode') == 'static'

libpkgcore_dep = dependency(
  'libpkgcore',
  required: true,
  static: want_static,
  fallback: ['libpkgcore', 'libpkgcore_dep'],
)

prefix = get_option('prefix')
conf = configuration_data()
conf.set('PACKAGE_NAME', meson.project_name())
conf.set('PACKAGE_VERSION', meson.project_version())
conf.set('PACKAGE_STRING', meson.project_name() + ' ' + meson.project_version())
conf.set('PACKAGE_BUGREPORT', 'https://github.com/zeppe-lin/pkgman/issues')
conf.set('PACKAGE_URL', 'https://github.com/zeppe-lin/pkgman')
conf.set('SYSCONFDIR', get_option('sysconfdir'))
conf.set('SHAREDSTATEDIR', get_option('sharedstatedir'))
conf.set('BINDIR', prefix / get_option('bindir'))
conf.set('SBINDIR', prefix / get_option('sbindir'))
conf.set('LIBPKGCORE_VERSION', libpkgcore_dep.version())

subdir('src')
subdir('man')
subdir('completion')
subdir('vim')
subdir('test')
