/* tibs.h

	Functions for dealing with that most wonderful of all days,
	St. Tib's Day, called "Leap Day" by the uninitiated.
*/

#pragma once

/* 59: 31 + 29 - 1 (to account for fencepost error) */
#define TIBSDAY 59
#define TIBSY -5

bool
is_past_tibsday(int32_t year, int32_t day);

bool
is_tibsday(int32_t year, int32_t day);

bool
is_tibsyear(int32_t year);
