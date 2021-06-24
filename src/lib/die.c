#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <die.h>

noreturn void die(const char *restrict msg, int status) {
	fprintf(stderr, "%s: %s\n", msg, strerror(errno));
	exit(status);
}
