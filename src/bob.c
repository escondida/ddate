/* bob.c

	Stuff to do with the Subgenius
*/

#include <stdint.h>

#include "bob.h"

int32_t
xday_countdown_days(int32_t day)
{
	return (day <= XDAY) ? XDAY - day : 365 - (day - XDAY);
}

int32_t
xday_countdown_years(int32_t year)
{
	return XYEAR - year;
}
