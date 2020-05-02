/* slogans.c

	Print the lovable catchphrases all the kids these days are
	saying.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "slogans.h"
#include "util.h"

/* char *get_random_line(FILE *):
	Returns a mallocked string holding a random slogan.
*/
char *
get_random_line(FILE *);

/* uint32_t nlines(FILE *)
	Returns the number of lines in a file.
*/
uint32_t
nlines(FILE *);

char *
get_random_line(FILE *f)
{
	char *s;
	uint32_t len = nlines(f);

	if (len > RAND_MAX || len <= 0) {
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
ddate_sloganeer()
{
	char *slogan;
	FILE *slogans;

	if (!(slogans = locate_file("slogans"))) {
		fputs("Error: could not find a slogans file\n", stderr);
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
