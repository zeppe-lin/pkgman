.POSIX:

include config.mk

OBJS   = $(subst   .cpp,.o,$(wildcard *.cpp))
MAN1   = $(subst .1.pod,.1,$(wildcard *.1.pod))
MAN5   = $(subst .5.pod,.5,$(wildcard *.5.pod))
MAN8   = $(subst .8.pod,.8,$(wildcard *.8.pod))

all: pkgman ${MAN1} ${MAN5} ${MAN8}

%: %.pod
	pod2man --nourls -r ${VERSION} -c ' ' -n $(basename $@) \
		-s $(subst .,,$(suffix $@)) $< > $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

pkgman: $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

install: all
	mkdir -p ${DESTDIR}${BINDIR}
	mkdir -p ${DESTDIR}${MANDIR}/man1
	mkdir -p ${DESTDIR}${MANDIR}/man5
	mkdir -p ${DESTDIR}${MANDIR}/man8
	cp -f pkgman  ${DESTDIR}${BINDIR}/
	cp -f ${MAN1} ${DESTDIR}${MANDIR}/man1/
	cp -f ${MAN5} ${DESTDIR}${MANDIR}/man5/
	cp -f ${MAN8} ${DESTDIR}${MANDIR}/man8/
	ln -sf pkgman-install.8 ${DESTDIR}${MANDIR}/man8/pkgman-update.8
	ln -sf pkgman-dep.1     ${DESTDIR}${MANDIR}/man1/pkgman-rdep.1

uninstall:
	rm -f ${DESTDIR}${BINDIR}/pkgman
	(cd ${DESTDIR}${MANDIR}/man1 && rm -f ${MAN1} pkgman-rdep.1)
	(cd ${DESTDIR}${MANDIR}/man5 && rm -f ${MAN5})
	(cd ${DESTDIR}${MANDIR}/man8 && rm -f ${MAN8} pkgman-update.8)

clean:
	rm -f pkgman ${OBJS} ${MAN1} ${MAN5} ${MAN8}

.PHONY: all install uninstall clean
