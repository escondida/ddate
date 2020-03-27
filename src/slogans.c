/* slogans.c

	Print the lovable catchphrases all the kids these days are
	saying.
*/

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ddate.h"

char *
get_random_line(FILE *file);

char *
locate_slogans(void);

uint32_t
nlines(FILE *);

char *
get_random_line(FILE *f)
{
	char *s;
	uint32_t len = nlines(f);

	if (len > RAND_MAX) {
		return NULL;
	}
	rewind(f);

	int64_t line = random() % len;

	int32_t c;
	while(line > 0 && ((c = getc(f)) != EOF)) {
		if (c == '\n') {
			line--;
		}
	}

	ssize_t nread;
	size_t n = 0;
	if ((nread = getline(&s, &n, f)) == -1) {
		perror("Error");
		return NULL;
	}

	if (s[nread-1] == '\n') {
		s[nread-1] = '\0';
	}

	return s;
}

char *
locate_slogans()
{
	char *fname = "slogans";
	char *path;

	if (strlen(fname) > PATH_MAX) {
		return NULL;
	}

	path = malloc(strlen(fname) + 1);
	if (!path) {
		fprintf(stderr, "Error: Out of memory; could not save path to slogans\n");
		return NULL;
	}

	snprintf(path, strlen(fname) + 1, "%s", fname);

	return path;
}

uint32_t
nlines(FILE *f)
{
	int32_t c;
	uint32_t lines = 0;

	while ((c = getc(f)) != EOF) {
		if (c == '\n') {
			lines++;
		}
	}

	return lines;
}

char *
sloganeer()
{
	char *path = 0;
	char *slogan = NULL;

	if (!(path = locate_slogans())) {
		fprintf(stderr, "Error: could not find file %s\n", path);
		return 0;
	}

	FILE *slogans = fopen(path, "r");
	if (!slogans) {
		perror("Error");
		goto cleanup;
	}

	if (!(slogan = get_random_line(slogans))) {
		fputs("Error: could not get random slogan\n", stderr);
		goto cleanup;
	}

	if (fclose(slogans) == EOF) {
		perror("Error");
		goto cleanup;
	}

cleanup:
	free(path);
	return slogan;
}
