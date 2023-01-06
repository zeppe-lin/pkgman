.POSIX:

include config.mk

OBJS = $(subst   .cpp,.o,$(wildcard *.cpp))
MAN1 = $(subst .1.pod,.1,$(wildcard *.1.pod))
MAN5 = $(subst .5.pod,.5,$(wildcard *.5.pod))
MAN8 = $(subst .8.pod,.8,$(wildcard *.8.pod))

all: pkgman ${MAN1} ${MAN5} ${MAN8}

%: %.pod
	pod2man --nourls -r ${VERSION} -c ' ' -n $(basename $@) \
		-s $(subst .,,$(suffix $@)) $<  >  $@

.cpp.o:
	${CXX} -c ${CXXFLAGS} ${CPPFLAGS} $<

pkgman: ${OBJS}
	${LD} $^ ${LDFLAGS} -o $@

check:
	@echo "=======> Check PODs for errors"
	@podchecker *.pod
	@grep -Eiho "https?://[^\"\\'> ]+" *.* | xargs -P10 -I{} \
		curl -o /dev/null -sw "%{url} [%{http_code}]\n" '{}'

install: all
	mkdir -p ${DESTDIR}/usr/bin
	mkdir -p ${DESTDIR}/usr/share/man/man1
	mkdir -p ${DESTDIR}/usr/share/man/man5
	mkdir -p ${DESTDIR}/usr/share/man/man8
	cp -f pkgman  ${DESTDIR}/usr/bin/
	chmod +x ${DESTDIR}/usr/bin/pkgman
	cp -f ${MAN1} ${DESTDIR}/usr/share/man/man1/
	cp -f ${MAN5} ${DESTDIR}/usr/share/man/man5/
	cp -f ${MAN8} ${DESTDIR}/usr/share/man/man8/

uninstall:
	rm -f ${DESTDIR}/usr/bin/pkgman
	cd ${DESTDIR}/usr/share/man/man1 && rm -f ${MAN1}
	cd ${DESTDIR}/usr/share/man/man5 && rm -f ${MAN5}
	cd ${DESTDIR}/usr/share/man/man8 && rm -f ${MAN8}

clean:
	rm -f pkgman ${OBJS} ${MAN1} ${MAN5} ${MAN8}

.PHONY: all install uninstall clean
