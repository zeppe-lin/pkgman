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

argparser_test = executable(
  'argparser-test',
  [
    'argparser_alias_test.cpp',
    '../src/argparser.cpp',
  ],
  include_directories: include_directories('../src'),
  install: false
)

test('argument parser aliases', argparser_test)

bash = find_program('bash', required: false)
if bash.found()
  test(
    'bash completion',
    bash,
    args: [
      files('completion.sh'),
      meson.project_source_root() / 'completion' / 'pkgman.bash',
    ],
  )
endif
