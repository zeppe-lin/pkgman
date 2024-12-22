.POSIX:
.SUFFIXES: .cpp .o

include ../config.mk
#include ../extra/flags-extra.mk
#include ../extra/flags-sanitizer.mk

OBJS = argparser.o configparser.o datafileparser.o depresolver.o \
       helpers.o locker.o main.o package.o pkgdb.o pkgman.o \
       pkgmksetting.o p_regex.o process.o repository.o \
       signaldispatcher.o transaction.o versioncomparator.o
BIN1 = pkgman

all: $(BIN1)

.cpp.o:
	$(CXX) -c $< $(CPPFLAGS) $(CXXFLAGS)

$(BIN1): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(BIN1) $(DESTDIR)$(PREFIX)/bin
	chmod 0755 $(DESTDIR)$(PREFIX)/bin/$(BIN1)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN1)

clean:
	rm -f $(OBJS) $(BIN1)

.PHONY: all install uninstall clean
