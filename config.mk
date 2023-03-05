# pkgman version
VERSION = 6.0

# paths
PREFIX = /usr
MANPREFIX = ${PREFIX}/share/man

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -DNDEBUG
CXXFLAGS = -std=c++17 -Wall -Wextra -Wconversion -Wcast-align \
	   -Wunused -Wshadow -Wold-style-cast
LDFLAGS  = -lstdc++fs

# compiler and linker
CXX = c++
LD  = ${CXX}
