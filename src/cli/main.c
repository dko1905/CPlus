#include <stdio.h>
#include <stdbool.h>

#include <util/printer.h>

bool print_pdebug = true;
bool print_pinfo = true;
bool print_pwarn = true;
bool print_perr = true;

int main(void) {
	pinfo("Hello, World!");

	return 0;
}
