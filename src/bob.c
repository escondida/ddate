/* bob.c

	Stuff to do with the Subgenius
*/

#include <stdbool.h>
#include <stdint.h>

#include "bob.h"

int16_t
xday_countdown_days(int16_t day, bool tibsyear)
{
	int16_t x = tibsyear ? 366 : 365;

	return (day <= XDAY) ? XDAY - day : x - (day - XDAY);
}

int64_t
xday_countdown_years(int64_t year)
{
	return XYEAR - year;
}
