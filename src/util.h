/* util.h

	Various functions that could be generally useful
*/

#pragma once

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

