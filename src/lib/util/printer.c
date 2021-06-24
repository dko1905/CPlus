#include <util/printer.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#define VX(name, file, prefix) \
void v##name(const char *format, va_list ap) { \
	if (!print_##name) \
		return; \
\
	fprintf(file, "%s: ", prefix); \
	vfprintf(file, format, ap); \
	fprintf(file, "\n"); \
} \
void name(const char *format, ...) { \
	va_list ap; \
\
	va_start(ap, format); \
	v##name(format, ap); \
	va_end(ap); \
}

extern bool print_pdebug;
extern bool print_pinfo;
extern bool print_pwarn;
extern bool print_perr;

noreturn void die(const char *restrict msg, int status) {
	perr("panic: %s", msg);
	exit(status);
}

VX(pdebug, stderr, "DEBUG")
VX(pinfo, stderr, "INFO")
VX(pwarn, stderr, "WARN")
VX(perr, stderr, "ERROR")
