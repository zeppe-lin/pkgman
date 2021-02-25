include config.mk

BINSRC = $(wildcard src/*.cpp)
MANSRC = $(wildcard man5/*.in man8/*.in)
OBJ = $(BINSRC:.cpp=.o)
MAN = $(MANSRC:.in=)
BIN = pkgman

all: $(BIN) $(MAN)


%: %.in
	sed \
		-e "s@#VERSION#@$(VERSION)@g" \
		-e "s@#LOCALSTATEDIR#@$(LOCALSTATEDIR)@g" \
		-e "s@#SYSCONFDIR#@$(SYSCONFDIR)@g" \
		$< > $@

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

$(OBJ):	config.mk
$(BIN): $(OBJ)
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f $(BIN) $(MAN) $(OBJ)

install: all
	install -m 0755 -D $(BIN) $(DESTDIR)$(PREFIX)/sbin/$(BIN)
	install -m 0644 -D conf/pkgman.conf $(DESTDIR)$(SYSCONFDIR)/pkgman.conf
	for m in man8/*.8; do \
		install -m 0644 -D -t $(DESTDIR)$(MANPREFIX)/man8 $$m; done
	for m in man5/*.5; do \
		install -m 0644 -D -t $(DESTDIR)$(MANPREFIX)/man5 $$m; done

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)$(SYSCONFDIR)/pkgman.conf
	cd $(DESTDIR)$(MANPREFIX)/ && rm -f $(MAN)

.PHONY: all install uninstall clean
