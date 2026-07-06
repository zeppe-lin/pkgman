vercomp_sources = [
  '../src/helpers.cpp',
  '../src/versioncomparator.cpp',
]

exe = executable(
  'vcomp',
  vercomp_sources,
  cpp_args: ['-DTEST'],
  install: false
)

test('version comparator', exe)
