/* format.c

	Format Discordian dates
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bob.h"
#include "ddate.h"
#include "fmt.h"
#include "slogans.h"

bool
find_tibs(const char *fmt, int32_t *tibstart, uint32_t *tibend);

/* fmt_ordinal
	1->"st", 2->"nd", 3->"rd", etc.
*/
char *
fmt_ordinal(int32_t n);

/* Based pretty closely on the original */
bool
ddate_fmt(char *buf, size_t bufsize, struct ddate dd, const char *fmt)
{
	char *bufptr = buf;
	int32_t tibstart = -1;
	uint32_t tibend = 0;
	size_t len = strlen(fmt);
	/* For bracketing off conditional sections */
	bool print = true;

	if (dd.tibsday) {
		if (!find_tibs(fmt, &tibstart, &tibend)) {
			fputs("Error formatting festive St. Tib's Day message\n", stderr);
			return false;
		}
	}

	for (uint32_t i = 0; i < len; i++) {
		/* i == tibstart if & only if it's St. Tib's Day; otherwise it remains -1 */
		if ((int32_t)i == tibstart) {
			char *tibmsg = "St. Tib's Day";
			strncpy(bufptr, tibmsg, bufsize - i);
			bufptr += strlen(tibmsg);
			i = tibend;
		}

		if (!print) {
			if (fmt[i] == '%') {
				if (fmt[++i] == ']') {
					print = true;
				}
			}
		} else if (fmt[i] != '%') {
			*bufptr++ = fmt[i];
		} else {
			bool sloganeered = false;
			/* See ../ddata/wisdom/yearlength */
			char *tmp = 0, n2s[16];
			int8_t max_date = 15;
			int32_t xyear = 0, xday = 0;

			switch (fmt[++i]) {
			/* Numbers and ordinal suffixes */
			case 'd':
				if (snprintf(n2s, max_date+1, "%d", dd.sday+1) > max_date+1) {
					fprintf(stderr, "%d...is a pretty large value for the day of the season\n", dd.sday+1);
					return false;
				}
				tmp = n2s;
				break;
			case 'D':
				if (snprintf(n2s, max_date+1, "%d", dd.day+1) > max_date+1) {
					fprintf(stderr, "%d...is a pretty large value for the day of the year\n", dd.day+1);
					return false;
				}
				tmp = n2s;
				break;
			case 'o':
				if (!(tmp = fmt_ordinal(dd.sday+1))) {
					fputs("This whole system is out of ordinal!\n", stderr);
					return false;
				}
				break;
			case 'Y':
				if (snprintf(n2s, max_date+1, "%d", dd.yold) > max_date+1) {
					fprintf(stderr, "%d...is a pretty large value for the year\n", dd.yold);
					return false;
				}
				tmp = n2s;
				break;

			/* Holydays */
			case 'H':
				if (!(tmp = ddate_fmt_holyday(dd.holyday, dd.lingananday))) {
					fprintf(stderr, "That's an odd number for a Holyday, %d\n", dd.holyday);
					return false;
				}
				break;
			case '[':
				if (dd.holyday < 0) {
					print = false;
				}
				break;
			case ']':
				break;

			/* Seasons */
			case 'S':
				if (!(tmp = ddate_fmt_season(dd.season, FULL))) {
					fprintf(stderr, "This is downright unseasonable\n", dd.season);
					return false;
				}
				break;
			case 's':
				if (!(tmp = ddate_fmt_season(dd.season, ABBRVTD))) {
					fprintf(stderr, "This is downright unseasonable\n", dd.season);
					return false;
				}
				break;

			/* Days of the week */
			case 'W':
				if (!(tmp = ddate_fmt_dayname(dd.wday, FULL))) {
					fprintf(stderr, "What day is it, boy? WHAT DAY IS IT? %d\n", dd.wday);
					return false;
				}
				break;
			case 'w':
				if (!(tmp = ddate_fmt_dayname(dd.wday, ABBRVTD))) {
					fprintf(stderr, "What day is it, boy? WHAT DAY IS IT? %d\n", dd.wday);
					return false;
				}
				break;

			/* Ranting and raving */
			case 'e':
				sloganeered = true;
				tmp = sloganeer();
				break;
			case 'X':
				xyear = xday_countdown_years(dd.yold);
				if (snprintf(n2s, max_date+1, "%d", xyear) > max_date+1) {
					fprintf(stderr, "It's a *really* long time 'til X-Day: %d\n", xyear);
					return false;
				}
				tmp = n2s;
				break;
			case 'x':
				xday = xday_countdown_days(dd.day);
				if (snprintf(n2s, max_date+1, "%d", xday) > max_date+1) {
					fprintf(stderr, "It's a *really* long time 'til X-Day: %d\n", xday);
					return false;
				}
				tmp = n2s;
				break;

			/* Special characters */
			case 'n':
				*bufptr++ = '\n';
				break;
			case 't':
				*bufptr++ = '\t';
				break;
			case '%':
				*bufptr++ = '%';
				break;
			case '{':
			case '}':
				break;

			default:
				fprintf(stderr, "Error: unknown format specifier \"%%%c\"\n", fmt[i]);
					return false;
			}

			if (tmp) {
				strncpy(bufptr, tmp, bufsize - i);
				bufptr += strlen(tmp);
			}

			if (sloganeered) {
				free(tmp);
			}
		}
	}

	*bufptr = '\0';
	return true;
}

char *
ddate_fmt_dayname(ddate_dow day, ddate_len len)
{
	char *daynames[2][6] = {
		{
			"Sweetmorn", "Boomtime", "Pungenday",
			"Prickle-Prickle", "Setting Orange",
			"St. Tib's Day"
		},
		{"SM", "BT", "PD", "PP", "SO", "TIBS"}
	};

	if (day == NOTADAY || len == ZERO) {
		return NULL;
	}

	return daynames[len][day];
}

char *
ddate_fmt_holyday(ddate_holyday hd, bool lc)
{
	char *holydaynames[10] = {
		"Mungday", "Chaoflux",
		"Mojoday", "Discoflux",
		"Syaday", "Confuflux",
		"Zaraday", "Bureflux",
		"Maladay", "Afflux"
	};
	if (lc) {
		holydaynames[2] = "Lingananday";
	}

	if (hd < 0) { /* ERRORDAY, NORMALDAY, TIBSDAY */
		return NULL;
	}

	return holydaynames[hd];
}

char *
ddate_fmt_season(ddate_season s, ddate_len len)
{
	char *seasons[2][5] = {
		{
			"Chaos", "Discord", "Confusion",
			"Bureaucracy", "The Aftermath"
		},
		{"Chs", "Dsc", "Cfn", "Bcy", "Afm"}
	};

	if (s == ERROR) {
		return NULL;
	}

	return seasons[len][s];
}

bool
find_tibs(const char *fmt, int32_t *tibstart, uint32_t *tibend)
{
	size_t len = strlen(fmt);

	uint32_t j = 1;
	for (int32_t i = 0; (size_t)i < len; i++, j++) {
		if (fmt[i] == '%') {
			switch (fmt[j]) {
			case 'A':
			case 'a':
			case 'd':
			case 'e':
			case 'o':
				*tibend = j;
				if (*tibstart < 0) {
					*tibstart = i;
				}
				break;
			case '{':
				*tibstart = i;
				break;
			case '}':
				if (*tibstart >= 0) {
					*tibend = j+1;
				} else {
					return false;
				}
				break;
			}
		}
	}

	return (*tibstart >= 0) ? true : false;
}

char *
fmt_ordinal(int32_t n)
{
	/* This is *only* for formatting dates */
	if (n < 1 || n > 73) {
		return NULL;
	}

	if (n > 10 && n < 20) {
		return "th";
	}

	switch (n % 10) {
	case 1:
		return "st";
	case 2:
		return "nd";
	case 3:
		return "rd";
	default:
		return "th";
	}
}
