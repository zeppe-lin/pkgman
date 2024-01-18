include config.mk

OBJS = $(subst .cpp,.o,$(wildcard src/*.cpp))
MAN1 = $(subst   man/,,$(wildcard man/*.1))
MAN5 = $(subst   man/,,$(wildcard man/*.5))
MAN8 = $(subst   man/,,$(wildcard man/*.8))

all: pkgman

pkgman: ${OBJS}
	${CXX} $^ ${LDFLAGS} -o $@

vcomp:
	${CXX} -o $@ -DTEST ${CXXFLAGS} ${CPPFLAGS} \
		src/helpers.cpp src/versioncomparator.cpp

check: vcomp
	@echo "=======> Check version comparator"
	@./vcomp ${VCOMP}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	mkdir -p ${DESTDIR}${MANPREFIX}/man5
	mkdir -p ${DESTDIR}${MANPREFIX}/man8
	cp -f pkgman  ${DESTDIR}${PREFIX}/bin/
	cd man && cp -f ${MAN1} ${DESTDIR}${MANPREFIX}/man1/
	cd man && cp -f ${MAN5} ${DESTDIR}${MANPREFIX}/man5/
	cd man && cp -f ${MAN8} ${DESTDIR}${MANPREFIX}/man8/
	cd ${DESTDIR}${PREFIX}/bin     && chmod 0755 pkgman
	cd ${DESTDIR}${MANPREFIX}/man1 && chmod 0644 ${MAN1}
	cd ${DESTDIR}${MANPREFIX}/man5 && chmod 0644 ${MAN5}
	cd ${DESTDIR}${MANPREFIX}/man8 && chmod 0644 ${MAN8}

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/pkgman
	cd ${DESTDIR}${MANPREFIX}/man1 && rm -f ${MAN1}
	cd ${DESTDIR}${MANPREFIX}/man5 && rm -f ${MAN5}
	cd ${DESTDIR}${MANPREFIX}/man8 && rm -f ${MAN8}

install_bashcomp:
	mkdir -p ${DESTDIR}${BASHCOMPDIR}
	cp -f bash_completion ${DESTDIR}${BASHCOMPDIR}/pkgman

uninstall_bashcomp:
	rm -f ${DESTDIR}${BASHCOMPDIR}/pkgman

install_vimfiles:
	mkdir -p ${DESTDIR}${VIMFILESDIR}/ftdetect
	mkdir -p ${DESTDIR}${VIMFILESDIR}/syntax
	cp -f vim/ftdetect/pkgmanconf.vim ${DESTDIR}${VIMFILESDIR}/ftdetect/
	cp -f vim/syntax/pkgmanconf.vim   ${DESTDIR}${VIMFILESDIR}/syntax/

uninstall_vimfiles:
	rm -f ${DESTDIR}${VIMFILESDIR}/ftdetect/pkgmanconf.vim
	rm -f ${DESTDIR}${VIMFILESDIR}/syntax/pkgmanconf.vim

clean:
	rm -f pkgman vcomp ${OBJS}
	rm -f ${DIST}.tar.gz

dist: clean
	git archive --format=tar.gz -o ${DIST}.tar.gz --prefix=${DIST}/ HEAD

.PHONY: all check install uninstall install_bashcomp uninstall_bashcomp \
	install_vimfiles uninstall_vimfiles clean dist
