# LDFLAGS += -lOpenSLES
PREFIX ?= /usr/local
VPATH=$(PWD)/src
CXXFLAGS += \
	-fPIC \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-O3 \
	-fno-omit-frame-pointer \
	-pie \
	-I$(PWD)/include

all: xdg-open xdg-mime

xdg-open: $(PWD)/src/xdg-open.cpp $(PWD)/src/MimeType.cpp
xdg-mime: $(PWD)/src/xdg-mime.cpp $(PWD)/src/MimeType.cpp

clean:
	rm -f xdg-open xdg-mime

install: xdg-open xdg-mime
	install xdg-open $(DESTDIR)$(PREFIX)/bin/xdg-open
	install xdg-mime $(DESTDIR)$(PREFIX)/bin/xdg-mime

uninstall:
	rm -f $(PREFIX)/bin/xdg-open $(PREFIX)/bin/xdg-mime

.PHONY: clean install uninstall
