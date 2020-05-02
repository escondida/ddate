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
				printf(("%s-%s\n"), progname, VERSION);
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
	if (!ddate_fmt(buf, sizeof(buf) / sizeof(buf[0]), dd, fnord)) {
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
