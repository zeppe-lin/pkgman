.POSIX:

include ../config.mk

MAN1 = pkgman.1 pkgman-cat.1 pkgman-current.1 pkgman-dep.1 \
       pkgman-diff.1 pkgman-dsearch.1 pkgman-dumpconfig.1 \
       pkgman-fsearch.1 pkgman-info.1 pkgman-isinst.1 \
       pkgman-list.1 pkgman-list-dup.1 pkgman-list-locked.1 \
       pkgman-list-nodependents.1 pkgman-list-orphans.1 \
       pkgman-ls.1 pkgman-path.1 pkgman-printf.1 \
       pkgman-rdep.1 pkgman-readme.1 pkgman-search.1
MAN5 = pkgman.conf.5
MAN8 = pkgman-edit.8 pkgman-install.8 pkgman-lock.8 \
       pkgman-remove.8 pkgman-sysup.8 pkgman-unlock.8 \
       pkgman-update.8

all: $(MAN1) $(MAN5) $(MAN8)

$(MAN1) $(MAN5) $(MAN8):
	scdoc < $@.scdoc > $@

install: all
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	mkdir -p $(DESTDIR)$(MANPREFIX)/man5
	mkdir -p $(DESTDIR)$(MANPREFIX)/man8
	cp -f $(MAN1) $(DESTDIR)$(MANPREFIX)/man1
	cp -f $(MAN5) $(DESTDIR)$(MANPREFIX)/man5
	cp -f $(MAN8) $(DESTDIR)$(MANPREFIX)/man8
	cd $(DESTDIR)$(MANPREFIX)/man1 && chmod 0644 $(MAN1)
	cd $(DESTDIR)$(MANPREFIX)/man5 && chmod 0644 $(MAN5)
	cd $(DESTDIR)$(MANPREFIX)/man8 && chmod 0644 $(MAN8)

uninstall:
	cd $(DESTDIR)$(MANPREFIX)/man1 && rm -f $(MAN1)
	cd $(DESTDIR)$(MANPREFIX)/man5 && rm -f $(MAN5)
	cd $(DESTDIR)$(MANPREFIX)/man8 && rm -f $(MAN8)

clean:
	rm -f $(MAN1) $(MAN5) $(MAN8)

.PHONY: all install uninstall clean
