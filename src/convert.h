/* convert.h

	Convert dates to Discordian calendar
*/

#pragma once

/* ddate_greg_to_eris()
	Take a Gregorian year and day of year and mash them into a
	more pleasingly Eristic form.
*/
ddate_error
ddate_greg_to_eris(struct ddate *dd, int32_t year, int32_t day);

/* ddate_greg_ymd_to_eris()
	Like greg_to_eris, but takes a month and day of month argument
*/
ddate_error
ddate_greg_ymd_to_eris(struct ddate *dd, int32_t year, int32_t month, int32_t day);
