######################################################################
# ASan and UBSan flags.                                              #
######################################################################

# includes and libs
INCS     =
LIBS     = -lstc++fs -lasan -lubsan

# flags
CPPFLAGS = -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 \
           -DNDEBUG -DVERSION=\"$(VERSION)\"
CXXFLAGS = -std=c++1z -O0 -ggdb3 -fno-omit-frame-pointer \
           -fsanitize=address \
           -fsanitize=leak \
           -fsanitize=undefined \
           -fsanitize-recover=address
LDFLAGS  = $(CXXFLAGS) $(LIBS)
