# Makefile.lint is the automated checking of pkgman project
# for various programmatic and stylistic errors.

all: deadlinks podchecker shellcheck cppcheck flawfinder longlines

deadlinks:
	@echo "=======> Check for dead links"
	@grep -EIihor "https?://[^\"\\'> ]+" --exclude-dir=.git*  \
		| xargs -P10 -r -I{} curl -I -o/dev/null          \
		 -sw "[%{http_code}] %{url}\n" '{}'               \
		| grep -v '^\[200\]'                              \
		| sort -u

podchecker:
	@echo "=======> Check PODs for syntax errors"
	@podchecker pod/*.pod >/dev/null

shellcheck:
	@echo "=======> Check shell scripts for syntax errors"
	@grep -m1 -Irl '^#\s*!/bin/sh' --exclude-dir=.git* \
		| xargs -L10 -r shellcheck -s sh

cppcheck:
	@echo "=======> Static C/C++ code analysis"
	@cppcheck --quiet --enable=all --check-level=exhaustive \
		--suppress=missingIncludeSystem src

flawfinder:
	@echo "=======> Check for potential security flaws"
	@flawfinder --quiet -D src

longlines:
	@echo "=======> Check for long lines (> ${MAXLINE})"
	@! grep -PIrn '^.{81,}$$' --exclude-dir=.git*

.PHONY: all deadlinks podchecker shellcheck cppcheck flawfinder longlines
