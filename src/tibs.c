/* tibs.c

	Functions for dealing with that most wonderful of all days,
	St. Tib's Day, called "Leap Day" by the uninitiated.
*/

#include <stdbool.h>
#include <stdint.h>

#include "tibs.h"

bool
is_past_tibsday(int64_t y, int32_t d)
{
	if (is_tibsyear(y) && d > TIBSDAY) {
		return true;
	} else {
		return false;
	}
}

bool
is_tibsday(int64_t y, int32_t d)
{
	if (is_tibsyear(y) && d == TIBSDAY) {
		return true;
	} else {
		return false;
	}
}

bool
is_tibsyear(int64_t y)
{
	/* https://en.wikipedia.org/wiki/Leap_year#Algorithm */
	if (y % 4 != 0) {
		return false;
	} else if (y % 100 != 0) {
		return true;
	} else if (y % 400 != 0) {
		return false;
	} else {
		return true;
	}
}
