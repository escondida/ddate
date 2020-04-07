/* util.c

	Various functions that could be generally useful
*/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

/* FILE *try_read_file
	Attempt to read the file at rpath + midpath + fname.
	If successful, returns a handle to the file.
	If unsuccessful, returns NULL.
*/
FILE *
try_read_file(char *rpath, char *midpath, char *fname);

FILE *
locate_file(char *fname)
{
	FILE *f;
	char *where;

	f = try_read_file(".", "/", fname);

	if (!f && (where = getenv("LOCALDIR"))) {
		f = try_read_file(getenv("LOCALDIR"), "/data/ddate/", fname);
	}

	if (!f && (where = getenv("HOME"))) {
		f = try_read_file(where, "/local/data/ddate/", fname);
	}

	if (!f && (where = getenv("XDG_DATA_HOME"))) {
		f = try_read_file(where, "/ddate/", fname);
	}

	if (!f && (where = getenv("HOME"))) {
		f = try_read_file(getenv("HOME"), "/.local/share/ddate/", fname);
	}

	if (!f) {
		f = try_read_file(DATADIR, "/ddate/", fname);
	}

	return f;
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
