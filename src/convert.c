/* convert.c

	Convert dates to Discordian calendar
*/

#include <stdbool.h>
#include <stdint.h>

#include "ddate.h"

#include "convert.h"
#include "tibs.h"

ddate_dow
ddate_date_to_dow(int32_t year, int32_t day)
{
	if (is_tibsday(year, day)) {
		return TIBS;
	} else if (is_past_tibsday(year, day)) {
		/* St. Tib's Day, of course, doesn't count. */
		day -= 1;
	}

	/*
		Pay close attention to the following algorithm; its
		operation may not be obvious at first, but I assure
		you that it is of tremendous complexity that may not
		be easy for the layperson to fully comprehend.
	*/
	return day % 5;
}
