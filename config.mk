# This file is a part of pkgman.
# See COPYING and COPYRIGHT files for corresponding information.

# pkgman version
VERSION = 6.0

# paths
PREFIX = /usr/local
BINDIR = ${PREFIX}/bin
MANDIR = ${PREFIX}/share/man

# flags
CXXFLAGS = -Wall -Wextra -Wconversion -Wcast-align -Wunused \
	    -Wshadow -Wcast-align -Wold-style-cast -std=c++17
CPPFLAGS = -DVERSION=\"${VERSION}\" -DNDEBUG
LDFLAGS  = -lstdc++fs

# compiler and linker
CXX = g++
LD  = ${CXX}

# vim:cc=72:tw=70
# End of file.
