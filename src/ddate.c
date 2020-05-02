/* ddate.c .. converts boring normal dates to fun Discordian Date -><-

	 FIVE TONS OF FLAX
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ddate.h"

#include "convert.h"
#include "fmt.h"

void
handle_error(ddate_error error);

void
usage(FILE *f, char *programname);

int
main(int argc, char *argv[])
{
	time_t t;
	struct tm *greg;
	struct ddate dd = {0};
	char *fnord = 0;
	int32_t optind;
	char *todayfmt = "Today is %{%W, the %d%o day of %S %} in the YOLD %Y.%[%n%tCelebrate %H!%]";
	char *otherfmt = "%[%H, %]%{%W, the %d%o of %S%}, YOLD %Y";
	char *progname = argv[0], *p;
	ddate_error err = DDATE_ERROR_NONE;

	/* Do this here instead of in slogans.c in case we add more randomness */
	srandom((uint32_t)time(NULL));

	if ((p = strrchr(progname, '/')) != NULL) {
		progname = p + 1;
	}

	/* do args here */
	for (optind = 1; optind < argc; optind++) {
		switch (argv[optind][0]) {
		case '+':
			fnord = argv[optind] + 1;
			break;
		case '-':
			switch (argv[optind][1]) {
			case 'h':
				usage(stdout, argv[0]);
				goto aftermath;
			case 'v':
				printf(("%s-%s\n"), progname, VERSION);
				goto aftermath;
			default:
				usage(stderr, argv[0]);
				err = DDATE_ERROR_USAGE;
				goto aftermath;
			}
		/*
			Break the loop on the first non-format,
			non-option argument. Yes, unfortunately that
			means that the order of arguments really does
			matter.
		*/
		default:
			goto thud;
		}
	}

thud:
	/* Y[Y[Y[Y]]] M[M] D[D] */
	if (argc - optind == 3) {
		int32_t moe = atoi(argv[optind]);
		int32_t larry = atoi(argv[optind + 1]);
		int32_t curly = atoi(argv[optind + 2]);

		if ((err = ddate_greg_ymd_to_eris(&dd, moe, larry, curly))
				!= DDATE_ERROR_NONE) {
			goto aftermath;
		}

		fnord = fnord ? fnord : otherfmt;
	} else if (argc == optind) {
		t = time(NULL);
		greg = localtime(&t);
		fnord = fnord ? fnord : todayfmt;
		/* tm_year starts counting at 1900 */
		if ((err = ddate_greg_to_eris(&dd, greg->tm_year + 1900, greg->tm_yday))
				!= DDATE_ERROR_NONE) {
			goto aftermath;
		}
	} else {
		usage(stderr, argv[0]);
		err = DDATE_ERROR_USAGE;
		goto aftermath;
	}

	char buf[1024];
	if (!ddate_fmt(buf, sizeof(buf) / sizeof(buf[0]), dd, fnord)) {
		return EXIT_FAILURE;
	}

	puts(buf);

aftermath:
	if (err != DDATE_ERROR_NONE) {
		handle_error(err);
	}

	return (int)err;
}

void
handle_error(ddate_error err)
{
	switch(err) {
	case DDATE_ERROR_NONE:
		break;
	case DDATE_ERROR_HDAY:
		fputs("Error: invalid holyday\n", stderr);
		break;
	case DDATE_ERROR_INVALID_FMT:
	case DDATE_ERROR_INVALID_GREGDAY:
		break;
	case DDATE_ERROR_INVALID_GREGDAY_OF_MONTH:
		fputs("Error: invalid Gregorian day of the month\n", stderr);
		break;
	case DDATE_ERROR_INVALID_GREGMONTH:
		fputs("Error: invalid Gregorian month number\n", stderr);
		break;
	case DDATE_ERROR_INVALID_GREGYEAR:
		fputs("Error: there is no year 0 in the Gregorian calendar\n", stderr);
		break;
	case DDATE_ERROR_ORDINAL:
	case DDATE_ERROR_SDAY:
		break;
	case DDATE_ERROR_SEASON:
		fputs("Error: invalid Erisian season\n", stderr);
		break;
	case DDATE_ERROR_SLOGAN:
	case DDATE_ERROR_SLOGAN_FILE:
	case DDATE_ERROR_TIBS:
		break;
	case DDATE_ERROR_UNKNOWN:
		fputs("An unknown error has occurred, somehow. Hail Eris!\n", stderr);
		break;
	case DDATE_ERROR_USAGE: /* Handled in main so usage() can use argv[0] */
		break;
	case DDATE_ERROR_WDAY:
	case DDATE_ERROR_XDAY:
	case DDATE_ERROR_XYEAR:
		break;
	case DDATE_ERROR_YDAY:
		fputs("Error: invalid day of year\n", stderr);
		break;
	}
}

void
usage(FILE *f, char *prog)
{
	fprintf(f, "Usage: %s [+format] [year month day]\n", prog);
}
