# pkgman version
VERSION = 6.0

# paths
PREFIX = /usr/local
BINDIR = ${PREFIX}/bin
MANDIR = ${PREFIX}/share/man

# flags
CXXFLAGS += -Wall -Wextra -Wconversion -Wcast-align -Wunused -Wshadow \
	    -Wcast-align -Wold-style-cast -std=c++17
CPPFLAGS += -DVERSION=\"${VERSION}\" -DNDEBUG
LDFLAGS  += -lstdc++fs
