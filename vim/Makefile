.POSIX:

include ../config.mk

all:

install:
	mkdir -p $(DESTDIR)$(VIMFILESDIR)/ftdetect
	mkdir -p $(DESTDIR)$(VIMFILESDIR)/syntax
	cp -f ftdetect/pkgmanconf.vim $(DESTDIR)$(VIMFILESDIR)/ftdetect
	cp -f syntax/pkgmanconf.vim $(DESTDIR)$(VIMFILESDIR)/syntax

uninstall:
	rm -f $(DESTDIR)$(VIMFILESDIR)/ftdetect/pkgmanconf.vim
	rm -f $(DESTDIR)$(VIMFILESDIR)/syntax/pkgmanconf.vim

clean:

.PHONY: all install uninstall clean
