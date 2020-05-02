/* convert.h

	Convert dates to Discordian calendar
*/

#pragma once

/* ddate_greg_to_eris()
	Take a Gregorian year and day of year and mash them into a
	more pleasingly Eristic form.
*/
bool
ddate_greg_to_eris(struct ddate *dd, int32_t year, int32_t day);

/* ddate_greg_to_eris2()
	Like greg_to_eris, but takes a month and day of month argument
*/
bool
ddate_greg_to_eris2(struct ddate *dd, int32_t year, int32_t month, int32_t day);
