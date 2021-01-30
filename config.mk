VERSION       = 6.0

PREFIX        ?= /usr/local
MANPREFIX     ?= ${PREFIX}/share/man
LOCALSTATEDIR ?= /var
SYSCONFDIR    ?= /etc

CXXFLAGS += -Wall -Wextra -Wconversion -Wcast-align -Wunused -Wshadow \
	    -Wcast-align -Wold-style-cast -std=c++17

CPPFLAGS += -DVERSION=\"${VERSION}\" \
	    -DLOCALSTATEDIR=\"${LOCALSTATEDIR}\" \
	    -DSYSCONFDIR=\"${SYSCONFDIR}\"

LDFLAGS   = -lstdc++fs
