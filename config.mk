# project metadata
NAME        = pkgman
VERSION     = 6.0.3
DIST        = ${NAME}-${VERSION}

# paths
PREFIX      = /usr
MANPREFIX   = ${PREFIX}/share/man
BASHCOMPDIR = ${PREFIX}/share/bash-completion/completions

# flags
CPPFLAGS    = -DNDEBUG -DVERSION=\"${VERSION}\"
ifeq (${DEBUG}, 1)
CXXFLAGS    = -std=c++17 -ggdb3 -fno-omit-frame-pointer -fsanitize=address \
              -fsanitize=leak -fsanitize=undefined -fsanitize-recover=address
LDFLAGS     = -lstdc++fs ${CXXFLAGS} -lasan -lubsan
else
CXXFLAGS    = -std=c++17 -Wall -Wextra -Wconversion -Wcast-align \
	      -Wunused -Wshadow -Wold-style-cast
LDFLAGS     = -lstdc++fs
endif
