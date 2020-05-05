/* ddate.c .. converts boring normal dates to fun Discordian Date -><-

	 FIVE TONS OF FLAX
*/

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ddate.h"

#include "convert.h"
#include "fmt.h"

ddate_error
arg2int64(char *s, int64_t *i);

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
	char *todayfmt = "Today is %{%W, the %d%o day of %S %} in the %z %Y.%[%n%tCelebrate %H!%]";
	char *otherfmt = "%[%H, %]%{%W, the %d%o of %S%}, %z %Y";
	char *bsfmt = "%[%H, %]%{%W, the %d%o of %S%}, %Y %z";
	char *bstodayfmt = "Your time machine or temporal accident has landed you on %{%W, the %d%o day of %S %} in the %Y %z.%[%n%tCelebrate %H!%]";
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
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				goto thud;
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
		int64_t moe, larry, curly;

		if ((err = arg2int64(argv[optind], &moe)) != DDATE_ERROR_NONE) {
			goto aftermath;
		}

		if ((err = arg2int64(argv[optind+1], &larry)) != DDATE_ERROR_NONE) {
			goto aftermath;
		}

		if ((err = arg2int64(argv[optind+2], &curly)) != DDATE_ERROR_NONE) {
			goto aftermath;
		}

		if ((err = ddate_greg_ymd_to_eris(&dd, moe, larry, curly))
				!= DDATE_ERROR_NONE) {
			goto aftermath;
		}

		if (!fnord) {
			fnord = dd.bs ? bsfmt : otherfmt;
		}
	} else if (argc == optind) {
		t = time(NULL);
		greg = localtime(&t);

		/* tm_year starts counting at 1900 */
		if ((err = ddate_greg_to_eris(&dd, greg->tm_year + 1900, greg->tm_yday))
				!= DDATE_ERROR_NONE) {
			goto aftermath;
		}

		if (!fnord) {
			/* We have to account for time travel, *obviously* */
			fnord = dd.bs ? bstodayfmt : todayfmt;
		}
	} else {
		usage(stderr, argv[0]);
		err = DDATE_ERROR_USAGE;
		goto aftermath;
	}

	char buf[1024];
	if ((err = ddate_fmt(buf, sizeof(buf) / sizeof(buf[0]), dd, fnord))
			!= DDATE_ERROR_NONE) {
		goto aftermath;
	}

	puts(buf);

aftermath:
	if (err != DDATE_ERROR_NONE) {
		handle_error(err);
	}

	return (int)err;
}

ddate_error
arg2int64(char *s, int64_t *i)
{
	errno = 0;
	*i = strtol(s, NULL, 10);

	if ((errno == ERANGE && (*i == LONG_MAX || *i == LONG_MIN))
			|| (errno != 0 && *i == 0)) {
		perror("strtol");
		return DDATE_ERROR_INVALID_DATE;
	}

	return DDATE_ERROR_NONE;
}

void
handle_error(ddate_error err)
{
	switch(err) {
	case DDATE_ERROR_NONE: /* How did we get here? */
		break;
	case DDATE_ERROR_BIGYEAR:
		fputs("Error: ddate only works until the last syllable of recorded time\n"
				"(Or at least until INT64_MAX - (Gregorian - Discordian ))\n",
				stderr);
		break;
	case DDATE_ERROR_ERA:
		fputs("Error: invalid era\n", stderr);
		break;
	case DDATE_ERROR_HDAY:
		fputs("Error: invalid holyday\n", stderr);
		break;
	case DDATE_ERROR_INVALID_DATE:
		fputs("Error: invalid date\n", stderr);
		break;
	case DDATE_ERROR_INVALID_FMT:
		fputs("Error: invalid format string. Please refer to the manual or your pineal gland.\n", stderr);
		break;
	case DDATE_ERROR_INVALID_GREGDAY:
		fputs("Error: invalid Gregorian day of the month\n", stderr);
		break;
	case DDATE_ERROR_INVALID_GREGMONTH:
		fputs("Error: invalid Gregorian month number\n", stderr);
		break;
	case DDATE_ERROR_INVALID_GREGYEAR:
		fputs("Error: there is no year 0 in the Gregorian calendar\n", stderr);
		break;
	case DDATE_ERROR_ORDINAL:
		fputs("This whole system is out of ordinal!\n", stderr);
		break;
	case DDATE_ERROR_SDAY:
		fputs("Error: invalid day of the Erisian season\n", stderr);
		break;
	case DDATE_ERROR_SEASON:
		fputs("Error: invalid Erisian season\n", stderr);
		break;
	case DDATE_ERROR_SLOGAN:
		fputs("Error printing slogan\n", stderr);
		break;
	case DDATE_ERROR_TIBS:
		fputs("Error formatting festive St. Tib's Day message\n", stderr);
		break;
	case DDATE_ERROR_UNKNOWN:
		fputs("An unknown error has occurred, somehow. Hail Eris!\n", stderr);
		break;
	case DDATE_ERROR_USAGE: /* Handled in main so usage() can use argv[0] */
		break;
	case DDATE_ERROR_WDAY:
		fputs("Error: invalid day of the Erisian week\n", stderr);
		break;
	case DDATE_ERROR_XDAY:
		fputs("Error: Xist interference prevents calculating days to the pre-anniversary of X-Day\n", stderr);
		break;
	case DDATE_ERROR_XYEAR:
		fputs("Error: Xist interference prevents calculating years to X-Day\n", stderr);
		break;
	case DDATE_ERROR_YDAY:
		fputs("Error: invalid day of year\n", stderr);
		break;
	case DDATE_ERROR_YEAR:
		fputs("Error: year way too far in the future to worry about right now\n", stderr);
		break;
	}
}

void
usage(FILE *f, char *prog)
{
	fprintf(f, "Usage: %s [+format] [year month day]\n", prog);
}
