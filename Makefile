include config.mk

OBJS = $(subst .cpp,.o,$(wildcard *.cpp))
MAN1 = $(wildcard *.1)
MAN5 = $(wildcard *.5)
MAN8 = $(wildcard *.8)

all: pkgman

pkgman: ${OBJS}
	${CXX} $^ ${LDFLAGS} -o $@

vcomp:
	${CXX} -o $@ -DTEST ${CXXFLAGS} ${CPPFLAGS} \
		helpers.cpp versioncomparator.cpp

check: vcomp
	@echo "=======> Check version comparator"
	@./vcomp ${VCOMP}

install: all
	mkdir -p      ${DESTDIR}${PREFIX}/bin
	mkdir -p      ${DESTDIR}${MANPREFIX}/man1
	mkdir -p      ${DESTDIR}${MANPREFIX}/man5
	mkdir -p      ${DESTDIR}${MANPREFIX}/man8
	cp -f pkgman  ${DESTDIR}${PREFIX}/bin/
	for F in ${MAN1}; do sed "s/@VERSION@/${VERSION}/" $$F > \
		${DESTDIR}${MANPREFIX}/man1/$$F; done
	for F in ${MAN5}; do sed "s/@VERSION@/${VERSION}/" $$F > \
		${DESTDIR}${MANPREFIX}/man5/$$F; done
	for F in ${MAN8}; do sed "s/@VERSION@/${VERSION}/" $$F > \
		${DESTDIR}${MANPREFIX}/man8/$$F; done
	cd ${DESTDIR}${PREFIX}/bin     && chmod 0755 pkgman
	cd ${DESTDIR}${MANPREFIX}/man1 && chmod 0644 ${MAN1}
	cd ${DESTDIR}${MANPREFIX}/man5 && chmod 0644 ${MAN5}
	cd ${DESTDIR}${MANPREFIX}/man8 && chmod 0644 ${MAN8}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/pkgman
	cd ${DESTDIR}${MANPREFIX}/man1 && rm -f ${MAN1}
	cd ${DESTDIR}${MANPREFIX}/man5 && rm -f ${MAN5}
	cd ${DESTDIR}${MANPREFIX}/man8 && rm -f ${MAN8}

install-bashcomp:
	mkdir -p ${DESTDIR}${BASHCOMPDIR}
	cp -f bash_completion ${DESTDIR}${BASHCOMPDIR}/pkgman

uninstall-bashcomp:
	rm -f ${DESTDIR}${BASHCOMPDIR}/pkgman

clean:
	rm -f pkgman vcomp ${OBJS}
	rm -f ${DIST}.tar.gz

dist: clean
	git archive --format=tar.gz -o ${DIST}.tar.gz --prefix=${DIST}/ HEAD

.PHONY: all check install install-bashcomp uninstall uninstall-bashcomp clean dist
