#ifndef DIE_H
#define DIE_H
#include <stdnoreturn.h>

noreturn void die(const char *restrict msg, int status);

#endif
