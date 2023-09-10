# project metadata
NAME        = pkgman
VERSION     = 6.0.2
DIST        = ${NAME}-${VERSION}

# paths
PREFIX      = /usr
MANPREFIX   = ${PREFIX}/share/man
BASHCOMPDIR = ${PREFIX}/share/bash-completion/completions

# flags
CPPFLAGS    = -DNDEBUG -DVERSION=\"${VERSION}\"
CXXFLAGS    = -std=c++17 -Wall -Wextra -Wconversion -Wcast-align \
	      -Wunused -Wshadow -Wold-style-cast
LDFLAGS     = -lstdc++fs
