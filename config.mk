# project metadata
NAME        = pkgman
VERSION     = 6.0.4
DIST        = ${NAME}-${VERSION}

# paths
PREFIX      = /usr
MANPREFIX   = ${PREFIX}/share/man
BASHCOMPDIR = ${PREFIX}/share/bash-completion/completions
VIMFILESDIR = ${PREFIX}/share/vim/vimfiles

# flags
ifneq (${DEBUG}, y)
CPPFLAGS    = -DNDEBUG -DVERSION=\"${VERSION}\"
CXXFLAGS    = -std=c++17 -Wall -Wextra -Wconversion -Wcast-align \
              -Wunused -Wshadow -Wold-style-cast
LDFLAGS     = -lstdc++fs
else
CPPFLAGS    = -DVERSION=\"${VERSION}\"
CXXFLAGS    = -std=c++17 -ggdb3 -fno-omit-frame-pointer -fsanitize=address \
              -fsanitize=leak -fsanitize=undefined -fsanitize-recover=address
LDFLAGS     = -lstdc++fs ${CXXFLAGS} -lasan -lubsan
endif
