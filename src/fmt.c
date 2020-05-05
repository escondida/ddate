/* format.c

	Format Discordian dates
*/

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bob.h"
#include "ddate.h"
#include "fmt.h"
#include "slogans.h"

/* INT64_MIN = −9223372036854775807 = 22 chars, + 1 for \0 */
#define MAX_DIGITS 23

/* The day of the season will always be 1-2 digits (+ \0) */
#define MAX_SDAY_DIGITS 3

/* The day of the year will always be 1-3 digits (+ \0) */
#define MAX_YDAY_DIGITS 4

bool
find_tibs(const char *fmt, int32_t *tibstart, uint32_t *tibend);

/* fmt_ordinal
	1->"st", 2->"nd", 3->"rd", etc.
*/
char *
fmt_ordinal(int32_t n);

/* Based pretty closely on the original */
ddate_error
ddate_fmt(char *buf, size_t bufsize, struct ddate dd, const char *fmt)
{
	char *bufptr = buf;
	int32_t tibstart = -1;
	uint32_t tibend = 0;
	size_t len = strlen(fmt);
	size_t written = 0;
	/* For bracketing off conditional sections */
	bool print = true;
	/* Count digits written in the loop below */
	int32_t digits = 0;

	if (dd.tibsday) {
		if (!find_tibs(fmt, &tibstart, &tibend)) {
			return DDATE_ERROR_TIBS;
		}
	}

	for (uint32_t i = 0; i < len; i++) {
		if (written >= bufsize) {
			break;
		}

		/* i == tibstart if & only if it's St. Tib's Day; otherwise it remains -1 */
		if ((int32_t)i == tibstart) {
			char *tibmsg = "St. Tib's Day";
			strncpy(bufptr, tibmsg, bufsize - written);
			bufptr += strlen(tibmsg);
			written += strlen(tibmsg);
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
			written++;
		} else {
			bool sloganeered = false;
			char *tmp = 0, n2s[MAX_DIGITS];
			int64_t xyear = 0;
			int16_t xday = 0;

			switch (fmt[++i]) {
			/* Numbers and ordinal suffixes */
			case 'd':
				digits = snprintf(n2s, MAX_SDAY_DIGITS, "%d", dd.sday+1);
				if (digits > MAX_SDAY_DIGITS || digits < 1) {
					return DDATE_ERROR_SDAY;
				}
				tmp = n2s;
				break;
			case 'D':
				digits = snprintf(n2s, MAX_YDAY_DIGITS, "%d", dd.yday+1);
				if (digits > MAX_YDAY_DIGITS || digits < 1) {
					return DDATE_ERROR_YDAY;
				}
				tmp = n2s;
				break;
			case 'o':
				if (!(tmp = fmt_ordinal(dd.sday+1))) {
					return DDATE_ERROR_ORDINAL;
				}
				break;
			case 'Y':
				digits = snprintf(n2s, MAX_DIGITS, "%ld", dd.yold);
				if (digits > MAX_DIGITS || digits < 1) {
					return DDATE_ERROR_YEAR;
				}
				tmp = n2s;
				break;

			/* Holydays */
			case 'H':
				if (!(tmp = ddate_fmt_holyday(dd.holyday, false))) {
					return DDATE_ERROR_HDAY;
				}
				break;
			case 'h':
				if (!(tmp = ddate_fmt_holyday(dd.holyday, true))) {
					return DDATE_ERROR_HDAY;
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
					return DDATE_ERROR_SEASON;
				}
				break;
			case 's':
				if (!(tmp = ddate_fmt_season(dd.season, ABBRVTD))) {
					return DDATE_ERROR_SEASON;
				}
				break;

			/* Days of the week */
			case 'W':
				if (!(tmp = ddate_fmt_dayname(dd.wday, FULL))) {
					return DDATE_ERROR_WDAY;
				}
				break;
			case 'w':
				if (!(tmp = ddate_fmt_dayname(dd.wday, ABBRVTD))) {
					return DDATE_ERROR_WDAY;
				}
				break;

			/* Ranting and raving */
			case 'e':
				sloganeered = true;
				if (!(tmp = ddate_sloganeer())) {
					return DDATE_ERROR_SLOGAN;
				}
				break;
			case 'X':
				xyear = xday_countdown_years(dd.yold);
				digits = snprintf(n2s, MAX_DIGITS, "%ld", xyear);
				if (digits > MAX_DIGITS || digits < 1) {
					return DDATE_ERROR_XYEAR;
				}
				tmp = n2s;
				break;
			case 'x':
				xday = xday_countdown_days(dd.yday, dd.tibsyear);
				digits = snprintf(n2s, MAX_YDAY_DIGITS, "%d", xday);
				if (digits > MAX_YDAY_DIGITS || digits < 1) {
					return DDATE_ERROR_XDAY;
				}
				tmp = n2s;
				break;

			/* Special characters */
			case 'n':
				*bufptr++ = '\n';
				written++;
				break;
			case 't':
				*bufptr++ = '\t';
				written++;
				break;
			case '%':
				*bufptr++ = '%';
				written++;
				break;
			case '{':
			case '}':
				break;

			default:
				return DDATE_ERROR_INVALID_FMT;
			}

			if (tmp) {
				strncpy(bufptr, tmp, bufsize - 1 - written);
				bufptr += strlen(tmp);
				written += strlen(tmp);
			}

			if (sloganeered) {
				free(tmp);
			}
		}
	}

	if (written < bufsize) {
		*bufptr = '\0';
	} else {
		buf[bufsize-1] = '\0';
	}

	return DDATE_ERROR_NONE;
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
				if (*tibstart < 0) {
					*tibstart = i;
				}
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
	if (n < 1 || n > SEASON_LEN) {
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
