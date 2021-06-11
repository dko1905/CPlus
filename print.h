#ifndef PRINT_H
#define PRINT_H
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

extern bool print_pdebug;
extern bool print_pinfo;
extern bool print_pwarn;
extern bool print_perr;

#define VX(name, file, prefix) \
static inline void v##name(const char *format, va_list ap) { \
	if (!print_##name) \
		return; \
\
	fprintf(file, "%s: ", prefix); \
	vfprintf(file, format, ap); \
	fprintf(file, "\n"); \
} \
static inline void name(const char *format, ...) { \
	va_list ap; \
\
	va_start(ap, format); \
	v##name(format, ap); \
	va_end(ap); \
}

VX(pdebug, stderr, "DEBUG")
VX(pinfo, stderr, "INFO")
VX(pwarn, stderr, "WARN")
VX(perr, stderr, "ERROR")

#endif
