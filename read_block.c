#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "read_block.h"
#include "die.h"

#define BUFFER_START_SIZE 8

static inline bool is_allowed_char(unsigned char c) {
	if ( c >= 128 ||
		(c >= '0' && c <= '9') ||
		(c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		(c == '.' || c == '_'))
		return true;
	else
		return false;
}

size_t read_block(char **restrict s, size_t *restrict capp, FILE *restrict stream) {
	size_t freep = 0, cap = *capp, len = 0;
	int c = 0;

	/* Setup `s` buffer. */
	if (*s == NULL || cap < 1) {
		cap = BUFFER_START_SIZE;
		*s = malloc(cap);
		if (*s == NULL)
			die("Failed to allocate memory for s", 2);
	}

	/* Skip all spaces. */
	while ((c = fgetc(stream)) != EOF) {
		if (is_allowed_char(c)) break;
	}
	/* Read charectors and save them. */
	while (c != EOF) {
		if (is_allowed_char(c)) {
			(*s)[freep++] = c;
			/* if no available place */
			if ((freep + 1) > cap ) {
				cap *= 2;
				*s = realloc(*s, cap);
				if (*s == NULL)
					die("Failed to reallocate memory for s", 2);
			}
		} else {
			ungetc(c, stream);
			break;
		}

		++len;
		c = fgetc(stream);
	}
	(*s)[freep] = '\0';
	*capp = cap;

	if (c == EOF) return (size_t)-1;

	return len;
}
