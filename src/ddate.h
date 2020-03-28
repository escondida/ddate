/* ddate.h

	Public API for formatting Discordian dates
*/

#pragma once

typedef enum ddate_dow_len {
	FULL = 0,
	ABBRVTD,
	ZERO
} ddate_dow_len;

typedef enum ddate_dow {
	SWEETMORN = 0,
	BOOMTIME,
	PUNGENDAY,
	PRICKLEPRICKLE,
	SETTINGORANGE,
	TIBS,
	NOTADAY
} ddate_dow;

typedef enum ddate_season {
	CHAOS = 0,
	DISCORD,
	CONFUSION,
	BUREAUCRACY,
	AFTERMATH,
	ERROR
} ddate_season;

struct ddate {
	int32_t yold;
	int32_t day;
	ddate_dow wday;
	ddate_season season;
	uint8_t sday; /* 0-73 */
	bool tibsyear:1;
	bool tibsday:1;
};
