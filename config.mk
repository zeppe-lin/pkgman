# pkgman version
VERSION = 6.0

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -DNDEBUG
CXXFLAGS = -std=c++17 -Wall -Wextra -Wconversion -Wcast-align \
	   -Wunused -Wshadow -Wold-style-cast
LDFLAGS  = -lstdc++fs

# compiler and linker
CXX = c++
LD  = ${CXX}
