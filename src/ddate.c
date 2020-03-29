/* ddate.c .. converts boring normal dates to fun Discordian Date -><-
	 written  the 65th day of The Aftermath in the Year of Our Lady of
	 Discord 3157 by Druel the Chaotic aka Jeremy Johnson aka
	 mpython@gnu.ai.mit.edu
		28 Sever St Apt #3
		Worcester MA 01609

	 and I'm not responsible if this program messes anything up (except your
	 mind, I'm responsible for that)

	 (k) YOLD 3161 and all time before and after.
	 Reprint, reuse, and recycle what you wish.
	 This program is in the public domain.  Distribute freely.  Or not.

	 Majorly hacked, extended and bogotified/debogotified on
	 Sweetmorn, Bureaucracy 42, 3161 YOLD, by Lee H:. O:. Smith, KYTP,
	 aka Andrew Bulhak, aka acb@dev.null.org

	 Slightly hackled and crackled by a sweet firey stove on
	 Boomtime, the 53rd day of Bureaucracy in the YOLD 3179,
	 by Chaplain Nyan the Wiser, aka Dan Dart, aka ntw@dandart.co.uk

	 and I'm not responsible if this program messes anything up (except your
	 mind, I'm responsible for that) (and that goes for me as well --lhos)

	 Version history:
	 Bureflux 3161:      First release of enhanced ddate with format strings
	 59 Bcy, 3161:       PRAISE_BOB and KILL_BOB options split, other minor
					   changes.
	 53 Bcy, 3179:       Fixed gregorian date conversions less than YOLD 1167

	 1999-02-22 Arkadiusz Miskiewicz <misiek@pld.ORG.PL>
	 - added Native Language Support

	 2000-03-17 Burt Holzman <holzman+ddate@gmail.com>
	 - added range checks for dates

	 2014-06-07 William Woodruff <william@tuffbizz.com>
	 - removed gettext dependent locale code

	 15th of Confusion, 3180:
	 - call out adherents of the wrong fruit

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
				return EXIT_SUCCESS;
			case 'v':
				printf(("%s\n"), progname);
				return EXIT_SUCCESS;
			default:
				usage(stderr, argv[0]);
				return EXIT_FAILURE;
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

		if (!ddate_greg_to_eris2(&dd, moe, larry, curly)) {
			fputs("Invalid date\n", stderr);
			return EXIT_FAILURE;
		}

		fnord = fnord ? fnord : otherfmt;
	} else if (argc == optind) {
		t = time(NULL);
		greg = localtime(&t);
		fnord = fnord ? fnord : todayfmt;
		/* tm_year starts counting at 1900 */
		if (!ddate_greg_to_eris(&dd, greg->tm_year + 1900, greg->tm_yday)) {
			fputs("Invalid date\n", stderr);
			return EXIT_FAILURE;
		}
	} else {
		usage(stderr, argv[0]);
		return EXIT_FAILURE;
	}

	char buf[1024];
	if (!ddate_fmt(buf, sizeof(buf), dd, fnord)) {
		return EXIT_FAILURE;
	}

	puts(buf);
	return EXIT_SUCCESS;
}

void
usage(FILE *f, char *prog)
{
	fprintf(f, "Usage: %s [+format] [year month day]\n", prog);
}
