/* format.c

	Format Discordian dates
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "ddate.h"
#include "fmt.h"

char *
ddate_fmt(const char *fmt, struct ddate dd)
{
	return 0;
}

char *
ddate_fmt_dayname(ddate_dow day, ddate_len len)
{
	char *daynames[2][6] = {
		{
			"Sweetmorn", "Boomtime", "Pungenday",
			"Prickle-Prickle", "Setting Orange",
			"St. Tib's Day"
		},
		{"SM", "BT", "PD", "PP", "SO", "TIBS"}
	};

	if (day == NOTADAY || len == ZERO) {
		return NULL;
	}

	return daynames[len][day];
}

char *
ddate_fmt_season(ddate_season s, ddate_len len)
{
	char *seasons[2][5] = {
		{
			"Chaos", "Discord", "Confusion",
			"Bureaucracy", "The Aftermath"
		},
		{"Chs", "Dsc", "Cfn", "Bcy", "Afm"}
	};

	if (s == ERROR) {
		return NULL;
	}

	return seasons[len][s];
}

