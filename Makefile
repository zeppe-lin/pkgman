include config.mk

BINSRC = $(wildcard src/*.cpp)
MANSRC = $(wildcard man5/*.in man8/*.in)
OBJ = $(BINSRC:.cpp=.o)
MAN = $(MANSRC:.in=)
BIN = prt

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
	install -Dm0755 $(BIN) $(DESTDIR)$(PREFIX)/bin/$(BIN)
	install -Dm0644 conf/prt.conf $(DESTDIR)$(PREFIX)/etc/prt.conf
	for m in man8/*.8; do \
		install -Dm0644 $$m -t $(DESTDIR)$(MANPREFIX)/man8/; \
	done
	for m in man5/*.5; do \
		install -Dm0644 $$m -t $(DESTDIR)$(MANPREFIX)/man5/; \
	done

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)$(PREFIX)/etc/prt.conf
	cd $(DESTDIR)$(MANPREFIX)/ && rm -f $(MAN)

.PHONY: all install uninstall clean
