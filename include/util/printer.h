#ifndef PRINTER_H
#define PRINTER_H
#include <stdnoreturn.h>

noreturn void die(const char *restrict msg, int status);

#ifdef __STDARG_H
void vpdebug(const char *restrict format, va_list ap);
void vpinfo(const char *restrict format, va_list ap);
void vpwarn(const char *restrict format, va_list ap);
void vperr(const char *restrict format, va_list ap);
#endif

void pdebug(const char *restrict format, ...);
void pinfo(const char *restrict format, ...);
void pwarn(const char *restrict format, ...);
void perr(const char *restrict format, ...);

#endif
