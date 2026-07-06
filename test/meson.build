vercomp_sources = [
  '../src/helpers.cpp',
  '../src/versioncomparator.cpp',
]

vercomp = executable(
  'vercomp',
  vercomp_sources,
  cpp_args: ['-DTEST'],
  install: false
)

test('version comparator', vercomp)
