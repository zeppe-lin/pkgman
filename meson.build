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

libpkgstate_dep = dependency(
  'libpkgstate',
  version: '>=0.2.0',
  required: true,
  static: want_static,
  fallback: ['libpkgstate', 'libpkgstate_dep'],
)

prefix = get_option('prefix')

buildmeta = {
  'PACKAGE_NAME': meson.project_name(),
  'PACKAGE_VERSION': meson.project_version(),
  'PACKAGE_STRING': meson.project_name() + ' ' + meson.project_version(),
  'PACKAGE_BUGREPORT': 'https://github.com/zeppe-lin/pkgman/issues',
  'PACKAGE_URL': 'https://github.com/zeppe-lin/pkgman',
  'SYSCONFDIR': get_option('sysconfdir'),
  'SHAREDSTATEDIR': get_option('sharedstatedir'),
  'BINDIR': prefix / get_option('bindir'),
  'SBINDIR': prefix / get_option('sbindir'),
  'LIBPKGSTATE_VERSION': libpkgstate_dep.version(),
}

subdir('src')
subdir('man')
subdir('completion')
subdir('vim')
subdir('test')
