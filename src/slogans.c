/* slogans.c

	Print the lovable catchphrases all the kids these days are
	saying.

	Public functions: ddate.h:/sloganeer(void)
*/

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "ddate.h"

/* char *get_random_line(FILE *):
	Returns a mallocked string holding a random slogan.
*/
char *
get_random_line(FILE *);

/* FILE *locate_file(char *fname):
	Try to find a readable file by the name of fname in the
	following locations, in order of preference:
		./
		$LOCALDIR/data/ddate/
		$XDG_DATA_HOME/ddate/
		$HOME/.local/share/ddate/
	Returns a handle to the file to use, or NULL on error.
*/
FILE *
locate_file(char *fname);

/* uint32_t nlines(FILE *)
	Returns the number of lines in a file.
*/
uint32_t
nlines(FILE *);

/* FILE *try_read_file
	Attempt to read the file at rpath + midpath + fname.
	If successful, returns a handle to the file.
	If unsuccessful, returns NULL.
*/
FILE *
try_read_file(char *rpath, char *midpath, char *fname);

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

FILE *
locate_file(char *fname)
{
	FILE *f;

	f = try_read_file(".", "/", fname);

	if (!f && getenv("LOCALDIR")) {
		f = try_read_file(getenv("LOCALDIR"), "/data/ddate/", fname);
	}

	if (!f && getenv("XDG_DATA_HOME")) {
		f = try_read_file(getenv("XDG_DATA_HOME"), "/ddate/", fname);
	}

	if (!f && getenv("HOME")) {
		f = try_read_file(getenv("HOME"), "/.local/share/ddate/", fname);
	}

	return f;
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
	char *slogan;
	FILE *slogans;

	if (!(slogans = locate_file("slogans"))) {
		fputs("Error: could not find a slogans file %s\n", stderr);
		return NULL;
	}

	if (!(slogan = get_random_line(slogans))) {
		fputs("Error: could not get random slogan\n", stderr);
	}

	if (fclose(slogans) == EOF) {
		perror("Error");
	}

	return slogan;
}

FILE *
try_read_file(char *rpath, char *midpath, char *fname)
{
	char *fullpath;
	FILE *f;

	fullpath = calloc(PATH_MAX, sizeof(char));
	if (!fullpath) {
		fprintf(stderr, "Error: Out of memory; could not save path to slogans\n");
		return NULL;
	}

	if (1 + strlen(rpath) + strlen(midpath) + strlen(fname) > PATH_MAX) {
		fprintf(stderr, "Error: File path exceeds PATH_MAX: %s%s%s", rpath, midpath, fname);
		return NULL;
	}

	snprintf(fullpath, PATH_MAX, "%s%s%s", rpath, midpath, fname);

	/* fopen() returns NULL on failure, which is what we return on failure */
	f = fopen(fullpath, "r");

	free(fullpath);
	return f;
}
