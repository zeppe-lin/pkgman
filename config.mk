# project metadata
NAME        = pkgman
VERSION     = 6.0.4

# paths
PREFIX      = /usr
MANPREFIX   = ${PREFIX}/share/man
BASHCOMPDIR = ${PREFIX}/share/bash-completion/completions
VIMFILESDIR = ${PREFIX}/share/vim/vimfiles

# flags
CPPFLAGS    = -DNDEBUG -DVERSION=\"${VERSION}\"
CXXFLAGS    = -std=c++1z -pedantic -Wall -Wextra
LDFLAGS     = -static -lstdc++fs
