.POSIX:

include ../../config.mk

FOOTPRINT_ORIG = GNU_DESTDIR_FOOTPRINT.orig
FOOTPRINT_NEW  = GNU_DESTDIR_FOOTPRINT.new
FOOTPRINT_DIFF = GNU_DESTDIR_FOOTPRINT.diff

all:
	@echo "=======> Build and install with GNU make:"
	@cd ../../ && make DESTDIR=../tests/gnu_make/PKG install
	@tree PKG/ > $(FOOTPRINT_NEW)
	@diff --color=always -u $(FOOTPRINT_ORIG) $(FOOTPRINT_NEW) \
		> $(FOOTPRINT_DIFF) || :
	@if test -s $(FOOTPRINT_DIFF); then \
		echo "!!! Unexpected package footprint:"; \
		cat $(FOOTPRINT_DIFF); \
		exit 1; \
	fi
clean:
	rm -rf PKG $(FOOTPRINT_NEW) $(FOOTPRINT_DIFF)

.PHONY: all clean
