# pkgman version
VERSION = 6.0

# paths
PREFIX = /usr/local
MANDIR = ${PREFIX}/share/man
LOCALSTATEDIR = /var
SYSCONFDIR = /etc

# flags
CXXFLAGS += -Wall -Wextra -Wconversion -Wcast-align -Wunused -Wshadow \
	    -Wcast-align -Wold-style-cast -std=c++17
CPPFLAGS += -DVERSION=\"${VERSION}\" -DLOCALSTATEDIR=\"${LOCALSTATEDIR}\" \
	    -DSYSCONFDIR=\"${SYSCONFDIR}\" -DPREFIX=\"${PREFIX}\" \
	    -DNDEBUG
LDFLAGS  += -lstdc++fs
