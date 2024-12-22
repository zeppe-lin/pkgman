.POSIX:

include ../../config.mk

BIN  = vcomp
SRCS = ../../src/helpers.cpp ../../src/versioncomparator.cpp

all: $(BIN) check

$(BIN):
	$(CXX) -o $@ -DTEST $(CXXFLAGS) $(CPPFLAGS) $(SRCS)

check: $(BIN)
	@echo "=======> Check version comparator:"
	./$(BIN)

clean:
	rm -f $(BIN)

.PHONY: all check clean
