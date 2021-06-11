#define _POSIX_C_SOURCE 200112L // POSIX.1-2001

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "read_block.h"

#ifndef VERSION
#define VERSION "Something went wrong with VERSION"
#endif

int main(int argc, char *argv[]){
	(void)argc; (void)argv;
	/* Switch to default locale, which often is UTF8. */
	setlocale(LC_ALL, "");

	return 0;
}