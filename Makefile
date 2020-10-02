.POSIX:

include config.mk

OBJECTS = main.o

all: options program

options:
	@echo \"Program\" build options:
	@echo "CFLAGS  = $(MYCFLAGS)"
	@echo "LDFLAGS = $(MYLDFLAGS)"
	@echo "CC      = $(CC)"

.c.o:
	$(CC) $(MYCFLAGS) -c $<

program: $(OBJECTS)
	$(CC) $(OBJECTS) -o program $(MYLDFLAGS)

clean:
	rm -f program *.o

.PHONY: all options clean