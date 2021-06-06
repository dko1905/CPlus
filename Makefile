.POSIX:

include config.mk

OBJECTS = main.o

all: options cppp

options:
	@echo cppp build options:
	@echo "CFLAGS  = $(MYCFLAGS)"
	@echo "LDFLAGS = $(MYLDFLAGS)"
	@echo "CC      = $(CC)"

.c.o:
	$(CC) $(MYCFLAGS) -c $<

cppp: $(OBJECTS)
	$(CC) $(OBJECTS) -o cppp $(MYLDFLAGS)

# Remove binary and object files
clean:
	rm -f cppp $(OBJECTS)

# Install cppp
install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f cppp $(DESTDIR)$(PREFIX)/bin
	chmod +x $(DESTDIR)$(PREFIX)/bin/cppp
deinstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/cppp

.PHONY: all options clean install deinstall
