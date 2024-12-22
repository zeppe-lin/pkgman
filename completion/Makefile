.POSIX:

include ../config.mk

all:

install:
	mkdir -p $(DESTDIR)$(BASHCOMPDIR)
	cp -f bash_completion $(DESTDIR)$(BASHCOMPDIR)/pkgman

uninstall:
	rm -f $(DESTDIR)$(BASHCOMPDIR)/pkgman

clean:

.PHONY: all install uninstall clean
