LDFLAGS += -lOpenSLES
PREFIX ?= /usr/local
CXXFLAGS += \
	-fPIC \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-O3 \
	-fno-omit-frame-pointer \
	-pie \
	-I/$(PWD)/include

open: open.cpp MimeType.cpp

clean:
	rm -f open

install: open
	install open $(DESTDIR)$(PREFIX)/bin/open

uninstall:
	rm -f $(PREFIX)/bin/open

.PHONY: clean install uninstall
