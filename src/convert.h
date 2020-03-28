/* convert.h

	Convert dates to Discordian calendar
*/

#pragma once

/* ddate_date_to_dow()
	Return the member of the enum ddate_dow corresponding to the
	day of the week.

	Argument: the day of year and the year (so that we can check
	for St. Tib's Day)
*/
ddate_dow
ddate_date_to_dow(int32_t day, int32_t year);
