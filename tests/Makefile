include $(CURDIR)/../config.mk

SUBDIRS = vcomp

all clean:
	$(MAKE) $(SUBDIRS) TARGET=$@

$(SUBDIRS):
	cd $@ && $(MAKE) $(TARGET)

.PHONY: all clean $(SUBDIRS)
