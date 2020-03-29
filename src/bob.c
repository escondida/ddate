/* bob.c

	Stuff to do with the Subgenius
*/

#include <stdint.h>

#include "bob.h"

/* X-Day: the 40th day of the Season of Confusion, YOLD 9827 */
#define XYEAR 9827
/* -1, since in the code day 0 == 1st Chaos */
#define XDAY 185

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
