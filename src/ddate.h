/* ddate.h

	Public API for formatting Discordian dates

	If you're going to do anything...well, "useful" is a strong
	word, but you know what I mean...with this, you probably want
	to include fmt.h as well: ddate_fmt() is probably the function
	you want.

	If you want to get really weird, convert.h will get you
	ddate_greg_to_eris (for when you have the year and day of
	year--as, for instance, from localtime(3)) and
	ddate_greg_ymd_to_eris (for when you have the year, month, and
	day)
*/

#pragma once

#define SEASON_LEN 73

typedef enum ddate_error {
	DDATE_ERROR_NONE = 0,
	DDATE_ERROR_UNKNOWN,
	DDATE_ERROR_HDAY,
	DDATE_ERROR_INVALID_FMT,
	DDATE_ERROR_INVALID_GREGDAY,
	DDATE_ERROR_INVALID_GREGDAY_OF_MONTH,
	DDATE_ERROR_INVALID_GREGMONTH,
	DDATE_ERROR_INVALID_GREGYEAR,
	DDATE_ERROR_ORDINAL,
	DDATE_ERROR_SDAY,
	DDATE_ERROR_SEASON,
	DDATE_ERROR_SLOGAN,
	DDATE_ERROR_SLOGAN_FILE,
	DDATE_ERROR_TIBS,
	DDATE_ERROR_WDAY,
	DDATE_ERROR_XDAY,
	DDATE_ERROR_XYEAR,
	DDATE_ERROR_YDAY
} ddate_error;

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
