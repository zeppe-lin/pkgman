.POSIX:

include config.mk

OBJS = $(subst   .cpp,.o,$(wildcard *.cpp))
MAN1 = $(subst .1.pod,.1,$(wildcard *.1.pod))
MAN5 = $(subst .5.pod,.5,$(wildcard *.5.pod))
MAN8 = $(subst .8.pod,.8,$(wildcard *.8.pod))

all: pkgman ${MAN1} ${MAN5} ${MAN8}

%: %.pod
	pod2man --nourls -r "pkgman ${VERSION}" -c ' ' -n $(basename $@) \
		-s $(subst .,,$(suffix $@)) $<  >  $@

.cpp.o:
	${CXX} -c ${CXXFLAGS} ${CPPFLAGS} $<

pkgman: ${OBJS}
	${LD} $^ ${LDFLAGS} -o $@

versioncomparator:
	${CXX} -o $@ -DTEST helpers.cpp versioncomparator.cpp

check: versioncomparator
	@echo "=======> Check version comparator"
	@./versioncomparator

install-dirs:
	mkdir -p ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	mkdir -p ${DESTDIR}${MANPREFIX}/man5
	mkdir -p ${DESTDIR}${MANPREFIX}/man8

install: all install-dirs
	cp -f pkgman  ${DESTDIR}${PREFIX}/bin/
	chmod 0755    ${DESTDIR}${PREFIX}/bin/pkgman
	cp -f ${MAN1} ${DESTDIR}${MANPREFIX}/man1/
	cp -f ${MAN5} ${DESTDIR}${MANPREFIX}/man5/
	cp -f ${MAN8} ${DESTDIR}${MANPREFIX}/man8/

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/pkgman
	cd ${DESTDIR}${MANPREFIX}/man1 && rm -f ${MAN1}
	cd ${DESTDIR}${MANPREFIX}/man5 && rm -f ${MAN5}
	cd ${DESTDIR}${MANPREFIX}/man8 && rm -f ${MAN8}

clean:
	rm -f pkgman versioncomparator ${OBJS} ${MAN1} ${MAN5} ${MAN8}

.PHONY: all check install-dirs install uninstall clean
