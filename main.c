#define _GNU_SOURCE /* We use getline. */
// #define _POSIX_C_SOURCE 200112L // POSIX.1-2001

#include <stdio.h>
#include <wchar.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef VERSION
#define VERSION "Something went wrong with VERSION"
#endif

#include "commands.h"

/* Steps:
 * 1. Read file per line, if @ is found, read command, and split args into list
*/

int main(int argc, char *argv[]) {
	(void)argc; (void)argv;

	printf("Version: "VERSION"\n");

	return 0;
}

struct parse_run_command;

typedef int (*parse_run_func_t)(struct parse_run_command *command);

struct parse_run_command {
	char cmd[CMD_MAXLEN];
	uint8_t cmd_arg_count;
	parse_run_func_t func;
};

void rec(
	FILE *volatile in,
	const struct parse_run_command *volatile commands
) {
	
}

int parse_run(
	FILE *volatile in,
	const struct parse_run_command *volatile commands
) {
	wint_t c = 0;
	bool in_command = false;


	while ((c = getwc(in)) != WEOF) {
		if (c == '@') {

		}
	}

	return 0;
}
