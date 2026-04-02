vercomp_srcs = [
  '../src/helpers.cpp',
  '../src/versioncomparator.cpp',
]

exe = executable('vcomp', vercomp_srcs, cpp_args: ['-DTEST'], install: false)

test('version comparator', exe)
