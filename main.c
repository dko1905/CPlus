#define _POSIX_C_SOURCE 200112L // POSIX.1-2001

#include <stdio.h>

#ifndef VERSION
#define VERSION "Something went wrong with VERSION"
#endif

int main(int argc, char *argv[]){
	(void)argc; (void)argv;

	printf("Version: "VERSION"\nHello world!\n");

	return 0;
}