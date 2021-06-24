#include <util/reader.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include <util/printer.h>

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

size_t next_block(char **restrict s, size_t *restrict capp, FILE *restrict stream) {
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

size_t next_block_wcs(wchar_t **restrict wcs, size_t *restrict capp, FILE *restrict stream) {
	size_t freep = 0, cap = *capp, len = 0;
	wint_t c = 0;

	/* Setup wcs buffer. */
	if (*wcs == NULL || cap < 1) {
		cap = BUFFER_START_SIZE * sizeof(wchar_t);
		*wcs = malloc(cap);
		if (*wcs == NULL)
			die("Failed to allocate memory for wcs", 2);
	}

	/* Skip all spaces. */
	while ((c = fgetwc(stream)) != WEOF) {
		if (is_allowed_char(c)) break;
	}

	/* Read charectores into string. */
	while (c != WEOF) {
		if (is_allowed_char(c)) {
			(*wcs)[freep++] = c;
			/* If filled up. */
			if (((freep + 1) * sizeof(wchar_t)) > cap) {
				cap *= 2;
				*wcs = realloc(*wcs, cap);
				if (*wcs == NULL)
					die("Failed to reallocate memory for s", 2);
			}

			++len;
			c = fgetwc(stream);
		} else {
			ungetwc(c, stream);
			break;
		}
	}
	(*wcs)[freep] = '\0';
	*capp = cap;

	if (c == WEOF) return (size_t) - 1;

	return len;
}
