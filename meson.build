project(
  'pkgman',
  'cpp',
  version: '6.2',
  meson_version: '>=1.2.0',
  default_options: [
    'cpp_std=c++17',
    'warning_level=3',
    'b_ndebug=if-release',
  ],
)

conf = configuration_data()
conf.set_quoted('PACKAGE_NAME', meson.project_name())
conf.set_quoted('PACKAGE_VERSION', meson.project_version())

configure_file(
  output: 'pkgman-config.h',
  configuration: conf,
)

config_inc = include_directories('.')

subdir('src')
subdir('man')
subdir('completion')
subdir('vim')
subdir('tests')
