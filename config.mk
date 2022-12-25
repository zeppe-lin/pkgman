# pkgman version
VERSION = 6.0

# flags
CXXFLAGS = -Wall -Wextra -Wconversion -Wcast-align -Wunused \
	    -Wshadow -Wcast-align -Wold-style-cast -std=c++17
CPPFLAGS = -DVERSION=\"${VERSION}\" -DNDEBUG
LDFLAGS  = -lstdc++fs

# compiler and linker
CXX = c++
LD  = ${CXX}
