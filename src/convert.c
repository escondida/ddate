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

/* doy_to_dow()
	Return the member of the enum ddate_dow corresponding to the
	day of the week for the given day of the year.
*/
ddate_dow
doy_to_dow(int16_t day);

/* get_holyday()
	Return the holy day for the given date.
	Takes an Erisian season and day of the season for arguments.
*/
ddate_holyday
get_holyday(ddate_season s, int32_t day);

/* g2e4real
	Do the actual work of setting up a ddate struct from a
	Gregorian year and day of year.
*/
ddate_error
g2e4real(struct ddate *dd, int64_t year, int16_t day);

void
handle_tibs(struct ddate *dd, int64_t year, int16_t day);

/* ddate_year_to_dyear(): Take a Gregorian year and make it into an Erisian one. */
ddate_error
year2yold(int64_t year, int64_t *yold, bool *bs);

ddate_dow
doy_to_dow(int16_t day)
{
	/*
		Pay close attention to the following algorithm; its
		operation may not be obvious at first, but I assure
		you that it is of tremendous complexity that may not
		be easy for the layperson to fully comprehend.
	*/
	return day % 5;
}

ddate_error
ddate_greg_to_eris(struct ddate *dd, int32_t year, int32_t day)
{
	if (day < 0 || (day > (is_tibsyear(year) ? 365 : 364))) {
		return DDATE_ERROR_YDAY;
	}

	return g2e4real(dd, year, (int16_t)day);
}

ddate_error
ddate_greg_ymd_to_eris(struct ddate *dd, int64_t year, int64_t month, int64_t day)
{
	int32_t cal[2][12] = {
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};
	int8_t tibs = is_tibsyear(year) ? 1 : 0;

	if (year == 0) {
		return DDATE_ERROR_INVALID_GREGYEAR;
	}

	if (month > 12 || month < 1) {
		return DDATE_ERROR_INVALID_GREGMONTH;
	}

	if (day < 1 || day > cal[tibs][month-1]) {
		return DDATE_ERROR_INVALID_GREGDAY;
	}

	/* Only add months that have already happened */
	month--;
	while (month > 0) {
		day += cal[tibs][--month];
	}

	if (day <= 0 || (tibs == 0 && day > 365) || (tibs == 1 && day > 366)) {
		/* How did we get here? */
		return DDATE_ERROR_YDAY;
	}

	return g2e4real(dd, year, (int16_t)day-1);
}

ddate_holyday
get_holyday(ddate_season s, int32_t d)
{
	ddate_holyday h[5][2] = {
		{MUNGDAY, CHAOFLUX},
		{MOJODAY, DISCOFLUX},
		{SYADAY, CONFUFLUX},
		{ZARADAY, BUREFLUX},
		{MALADAY, AFFLUX}
	};

	if (s == ERROR) {
		return ERRORDAY;
	}

	if (d == 4) {
		return h[s][0];
	} else if (d == 49) {
		return h[s][1];
	} else {
		return NORMALDAY;
	}
}

ddate_error
g2e4real(struct ddate *dd, int64_t year, int16_t day)
{
	ddate_error err;
	bool bs;

	dd->season = ERROR;
	dd->lingananday = false;

	if ((err = year2yold(year, &dd->yold, &bs)) != DDATE_ERROR_NONE) {
		return err;
	}
	dd->bs = bs;

	handle_tibs(dd, year, day);

	if (dd->yday != TIBSY) {
		dd->wday = doy_to_dow(dd->yday);
		dd->season = dd->yday / SEASON_LEN;
		dd->sday = dd->yday % SEASON_LEN;
		dd->holyday = get_holyday(dd->season, dd->sday);
	}

	if (dd->season == ERROR) {
		return DDATE_ERROR_SEASON;
	}

	if (dd->holyday == ERRORDAY) {
		return DDATE_ERROR_HDAY;
	}

	return DDATE_ERROR_NONE;
}

void
handle_tibs(struct ddate *dd, int64_t year, int16_t day)
{
	dd->tibsyear = is_tibsyear(year);

	if (is_tibsday(year, day)) {
		dd->yday = TIBSY;
		dd->wday = TIBS;
		dd->season = CHAOS;
		dd->sday = TIBSY;
		dd->holyday = TIBSDY;
		dd->tibsday = true;
	} else {
		dd->tibsday = false;
		if (dd->tibsyear && is_past_tibsday(year, day)) {
			dd->yday = day - 1;
			dd->tibsday = false;
		} else {
			dd->yday = day;
		}
	}
}

ddate_error
year2yold(int64_t year, int64_t *yold, bool *bs)
{
	/* PD:00034: YOLD 3136 = 1970 C.E. */
	int64_t offset = 1166;

	if (year > INT64_MAX - offset) {
		return DDATE_ERROR_BIGYEAR;
	}

	*bs = false;

	if (year == 0) { /* There is no year 0 in the Gregorian calendar */
		/* How did we get here? This should have already been checked. */
		return DDATE_ERROR_INVALID_GREGYEAR;
	} else if (year < -1166) {
		/*
			AFAIK, nobody until now has decided how to
			represent pre-Snub dates. This is uncharted
			territory. If you disagree, well...schism is
			always an option.

			My arbitrary decision: POEE Calendar also
			skips year 0 (leave 0 to Void; see PD:00056),
			dates prior to the Snub are considered BS
			(Before Snub)
		*/
		*bs = true;
	} else if (year < 0) { /* -1166 < date < 0 */
		offset++;
	}

	*yold = year + offset;

	return DDATE_ERROR_NONE;
}
