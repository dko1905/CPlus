#include <stdbool.h>
#include <wchar.h>
#define _POSIX_C_SOURCE 200112L // POSIX.1-2001

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "read_block.h"
#include "print.h"
#include "hashmap.h"

#ifndef VERSION
#define VERSION "Something went wrong with VERSION"
#endif

/* Typedefs */
struct import_item {
	wchar_t *short_name;
	wchar_t *long_name;
};

/* Function declerations */
static int parse_and_print(FILE *input, FILE* output);
static int handle_command(struct hashmap *import_hm, wchar_t **command_p,
                          size_t *command_cap, FILE *input, FILE* output);

static int import_item_compare(const void *a, const void *b, void *udata);
static uint64_t import_item_hash(const void *item, uint64_t seed0, uint64_t seed1);
static bool import_item_free_iter(const void *item, void *udata);

/* Globals */
bool print_pdebug = false;
bool print_pinfo = true;
bool print_pwarn = true;
bool print_perr = true;

/* Function definitions */
int main(int argc, char *argv[]){
	(void)argc; (void)argv;
	/* Switch to default locale, which often is UTF8. */
	setlocale(LC_ALL, "");

	int st = parse_and_print(stdin, stdout);
	pdebug("Parse return is: %i", st);

	return 0;
}

static int parse_and_print(FILE *input, FILE* output) {
	wint_t c = 0;
	/* states:
	 * 0 - normal
	 * 1 - start comment
	 * 2 - c comment
	 * 3 - cpp comment
	 * 4 - stop comment (c only)
	 * 5 - skip char (only comments) (c & cpp)
	 * 10 - start command
	 * 11 - command stop (just like skip char)
	 * 20 - string skip
	 * 21 - string " start
	 * 22 - string ' start
	 */
	unsigned int state = 0;
	/* Import hashmap. */
	struct hashmap *import_hm = hashmap_new(sizeof(struct import_item), 0,
	                                        0, 0, import_item_hash,
	                                        import_item_compare, NULL);
	/* Command. */
	wchar_t *command = NULL;
	size_t command_cap = 0;

	/* Main loop */
	while ((c = fgetwc(input)) != WEOF) {
		/* Handle comments */
		if (state == 5 || state == 11 || state == 20) {
			state = 0;
		}
		if (state == 0) {
			if (c == L'/') {
				state = 1;
			} else if (c == L'@') {
				state = 10;
			}
		} else if (state == 1) {
			if (c == L'*') {
				state = 2;
			} else if (c == L'/') {
				state = 3;
			} else {
				state = 0;
			}
		} else if (state == 2) {
			if (c == L'*') {
				state = 4;
			}
		} else if (state == 3) {
			if (c == L'\n') {
				state = 0;
			}
		} else if (state == 4) {
			if (c == L'/') {
				state = 5;
			} else {
				state = 2;
			}
		}
		/* Handle strings/quotes */
		if (state == 0) {
			if (c == L'"') {
				state = 21;
			} else if (c == L'\'') {
				state = 22;
			}
		} else if (state == 21) {
			if (c == L'"') {
				state = 20;
			}
		} else if (state == 23) {
			if (c == L'\'') {
				state = 20;
			}
		}
		/* Handle commands */
		if (state == 10) {
			pdebug("top of handle_c: %ls, %zu", command, command_cap);
			handle_command(import_hm, &command, &command_cap, stdin, stdout);
			pdebug("bot of handle_c: %ls, %zu", command, command_cap);
			state = 11;
		}

		/* Finally print. */
		if (state <= 5 || (state >= 20 && state <= 22))
			fprintf(output, "%lc", c);
	}

	hashmap_scan(import_hm, import_item_free_iter, NULL);
	free(command);
	hashmap_free(import_hm);

	return 0;
}

static int handle_command(struct hashmap *import_hm, wchar_t **command_p,
                          size_t *command_capp, FILE *input, FILE* output) {
	(void)output;
	size_t read_wchars = 0, command_cap = 0;
	wchar_t *command = *command_p;

	command_cap = *command_capp;

	read_wchars = read_block_wcs(&command, &command_cap, input);
	if (read_wchars == (size_t) - 1) return 1;

	if (wcscmp(command, L"import") == 0) {
		pdebug("import command detected!");

		struct import_item *ii = NULL;
		wchar_t *fullname = NULL, *name = NULL;
		size_t fullname_cap = 0, name_cap = 0;
		size_t fullname_len = 0, name_len = 0;

		fullname_len = read_block_wcs(&fullname, &fullname_cap, input);
		if (fullname_len == (size_t) - 1) return 1;
		name_len = read_block_wcs(&name, &name_cap, input);
		if (name_len == (size_t) - 1) return 1;
		name_len = read_block_wcs(&name, &name_cap, input);
		if (name_len == (size_t) - 1) return 1;

		for (size_t n = 0; n < fullname_len; ++n) {
			if (fullname[n] == L'.')
				fullname[n] = L'_';
		}

		pdebug("    %ls : %ls", fullname, name);

		ii = hashmap_get(import_hm, &(struct import_item){.short_name = name});
		if (ii != NULL) {
			pwarn("%ls has already been imported as %ls", fullname, ii->short_name);
		}

		hashmap_set(import_hm, &(struct import_item){.long_name = fullname, .short_name = name});
		fprintf(output, "typedef %ls %ls;", fullname, name);

		ii = hashmap_get(import_hm, &(struct import_item){.short_name = name});
		pdebug("ii is %ls : %ls", ii->short_name, ii->long_name);

	} else if (wcscmp(command, L"new") == 0) {
		pdebug("new command detected!");

		wchar_t *typename = NULL, *fullname = NULL;
		size_t typename_cap = 0;
		size_t typename_len = 0;

		typename_len = read_block_wcs(&typename, &typename_cap, input);
		if (typename_len == (size_t) - 1) return 1;

		struct import_item *ii;

		ii = hashmap_get(import_hm, &(struct import_item){.short_name = typename});
		if (ii == NULL) {
			pwarn("%ls is not defined", typename);
			return 1;
		}
		fullname = ii->long_name;

		pdebug("got %ls from %ls", fullname, typename);

		fprintf(output, "%ls_init", fullname);

		free(typename);

	} else if (wcscmp(command, L"free") == 0) {
		pdebug("free command detected!");

		struct import_item *ii;
		wchar_t *varname = NULL, *typename = NULL, *fullname = NULL;
		size_t varname_cap = 0, typename_cap = 0;
		size_t varname_len = 0, typename_len = 0;

		varname_len = read_block_wcs(&varname, &varname_cap, input);
		if (varname_len == (size_t) - 1) return 1;
		typename_len = read_block_wcs(&typename, &typename_cap, input);
		if (typename_len == (size_t) - 1) return 1;
		typename_len = read_block_wcs(&typename, &typename_cap, input);
		if (typename_len == (size_t) - 1) return 1;

		ii = hashmap_get(import_hm, &(struct import_item){.short_name = typename});
		if (ii == NULL) {
			pwarn("%ls is not defined", typename);
			return 1;
		}
		fullname = ii->long_name;

		pdebug("varname: %ls    typename: %ls", varname, typename);

		fprintf(output, "%ls_free(&%ls)", fullname, varname);

		free(varname);
		free(typename);

	} else {
		pdebug("unknown command: %ls", command);
	}

	*command_p = command;
	*command_capp = command_cap;

	return 0;
}

static int import_item_compare(const void *a, const void *b, void *udata) {
	(void)udata;
	const struct import_item *iia = a;
	const struct import_item *iib = b;
	return wcscmp(iia->short_name, iib->short_name);
}

static bool import_item_free_iter(const void *item, void *udata) {
	(void)udata;
	const struct import_item *ii = item;

	if (ii->long_name != NULL)
		free(ii->long_name);
	if (ii->short_name != NULL)
		free(ii->short_name);

	return true;
}

static uint64_t import_item_hash(const void *item, uint64_t seed0, uint64_t seed1) {
	const struct import_item *ii = item;
	return hashmap_sip(ii->short_name, wcslen(ii->short_name), seed0, seed1);
}

