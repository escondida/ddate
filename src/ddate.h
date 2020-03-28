/* ddate.h

	Public API for formatting Discordian dates
*/

#pragma once

typedef enum ddate_len {
	FULL = 0,
	ABBRVTD,
	ZERO
} ddate_len;

typedef enum ddate_dow {
	SWEETMORN = 0,
	BOOMTIME,
	PUNGENDAY,
	PRICKLEPRICKLE,
	SETTINGORANGE,
	TIBS,
	NOTADAY
} ddate_dow;

typedef enum ddate_holyday {
	ERRORDAY = -3, NORMALDAY = -2, TIBSDY = -1,
	MUNGDAY, CHAOFLUX,
	MOJODAY, DISCOFLUX,
	SYADAY, CONFUFLUX,
	ZARADAY, BUREFLUX,
	MALADAY, AFFLUX
} ddate_holyday;

typedef enum ddate_season {
	ERROR = -1,
	CHAOS,
	DISCORD,
	CONFUSION,
	BUREAUCRACY,
	AFTERMATH
} ddate_season;

struct ddate {
	int32_t yold;
	int32_t day; /* -5 to 365 */
	ddate_season season;
	ddate_holyday holyday;
	ddate_dow wday;
	int8_t sday; /* -5 to 72 */\
	bool tibsyear:1;
	bool tibsday:1;
	bool lingananday:1;
	int32_t pad:21;
};
