# project metadata
NAME        = pkgman
VERSION     = 6.0.2
HOMEPAGE    = https://github.com/zeppe-lin/pkgman/
BUGTRACKER  = https://github.com/zeppe-lin/pkgman/issues/
DIST        = ${NAME}-${VERSION}

# paths
PREFIX      = /usr
MANPREFIX   = ${PREFIX}/share/man
BASHCOMPDIR = ${PREFIX}/share/bash-completion/completions

# flags
CPPFLAGS    = -DNDEBUG \
	      -DPROJECT_VERSION=\"${VERSION}\" \
	      -DPROJECT_HOMEPAGE=\"${HOMEPAGE}\" \
	      -DPROJECT_BUGTRACKER=\"${BUGTRACKER}\"
CXXFLAGS    = -std=c++17 -Wall -Wextra -Wconversion -Wcast-align \
	      -Wunused -Wshadow -Wold-style-cast
LDFLAGS     = -lstdc++fs

# compiler and linker
CXX         = c++
LD          = ${CXX}
