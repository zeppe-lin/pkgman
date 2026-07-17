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

state_integration = find_program('state-integration.sh')

test(
  'installed state integration',
  state_integration,
  args: [pkgman_exe],
)
