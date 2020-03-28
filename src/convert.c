/* convert.c

	Convert dates to Discordian calendar
	See PD:00034 for all of your questions.
	ALL of them.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "ddate.h"

#include "convert.h"
#include "tibs.h"

#define SEASON_LEN 73

/* doy_to_dow()
	Return the member of the enum ddate_dow corresponding to the
	day of the week for the given day of the year.
*/
ddate_dow
doy_to_dow(int32_t day);

/* g2e4real
	Do the actual work of setting up a ddate struct from a
	Gregorian year and day of year.
*/
void
g2e4real(struct ddate *dd, int32_t year, int32_t day);

void
handle_tibs(struct ddate *dd, int32_t year, int32_t day);

/* ddate_year_to_dyear(): Take a Gregorian year and make it into an Erisian one. */
int32_t
year_to_dyear(int32_t y);

ddate_dow
doy_to_dow(int32_t day)
{
	/*
		Pay close attention to the following algorithm; its
		operation may not be obvious at first, but I assure
		you that it is of tremendous complexity that may not
		be easy for the layperson to fully comprehend.
	*/
	return day % 5;
}

bool
ddate_greg_to_eris(struct ddate *dd, int32_t year, int32_t day)
{
	if (day < 0 || (day > (is_tibsyear(year) ? 365 : 364))) {
		fprintf(stderr, "Error: invalid day of the year: %d\n", day);
		return false;
	}

	g2e4real(dd, year, day);

	return true;
}

bool
ddate_greg_to_eris2(struct ddate *dd, int32_t year, int32_t month, int32_t day)
{
	int32_t cal[2][12] = {
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};
	int8_t tibs = is_tibsyear(year) ? 1 : 0;

	if (year == 0) {
		fputs("Error: there is no year 0 in the Gregorian calendar", stderr);
		return false;
	}

	if (month > 12 || month < 1) {
		fprintf(stderr, "Error: invalid month number: %d\n", month);
		return false;
	}

	if (day < 1 || day > cal[tibs][month-1]) {
		fprintf(stderr, "Error: invalid day of the month: %d\n", day);
		return false;
	}

	while (--month > 0) {
		day += cal[tibs][month];
	}

	g2e4real(dd, year, day);

	return true;
}

void
g2e4real(struct ddate *dd, int32_t year, int32_t day)
{
	dd->yold = year_to_dyear(year);

	/* Date given starts counting at 1, the math starts counting at 0 */
	day--;

	handle_tibs(dd, year, day);

	if (dd->day != TIBSY) {
		dd->wday = doy_to_dow(dd->day);
		dd->season = dd->day / SEASON_LEN;
		dd->sday = dd->day % SEASON_LEN;
	}
}

void
handle_tibs(struct ddate *dd, int32_t year, int32_t day)
{
	dd->tibsyear = is_tibsyear(year);

	if (is_tibsday(year, day)) {
		dd->day = TIBSY;
		dd->wday = TIBS;
		dd->season = CHAOS;
		dd->sday = TIBSY;
		dd->tibsday = true;
	} else {
		dd->tibsday = false;
		if (dd->tibsyear && is_past_tibsday(year, day)) {
			dd->day = day - 1;
			dd->tibsday = false;
		} else {
			dd->day = day;
		}
	}
}

int32_t
year_to_dyear(int32_t y)
{
	return y + (3136 - 1970);
}
