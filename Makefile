.POSIX:

include config.mk

OBJECTS = main.o

all: options <program>

options:
	@echo <Program> build options:
	@echo "CFLAGS  = $(MYCFLAGS)"
	@echo "LDFLAGS = $(MYLDFLAGS)"
	@echo "CC      = $(CC)"

.c.o:
	$(CC) $(MYCFLAGS) -c $<

<program>: $(OBJECTS)
	$(CC) $(OBJECTS) -o <program> $(MYLDFLAGS)

# Remove binary and object files
clean:
	rm -f <program> $(OBJECTS)

# Install program
install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f <program> $(DESTDIR)$(PREFIX)/bin
	chmod +x $(DESTDIR)$(PREFIX)/bin/<program>
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/<program>

.PHONY: all options clean install uninstall
