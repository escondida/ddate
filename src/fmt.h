/* fmt.h
*/

/* ddate_fmt()
	Format a Discordian date (and sundry other nonsense).
	Returns the formatted string, mallocked.
	On failure, returns NULL
*/
char *
ddate_fmt(const char *fmt, struct ddate dd);

/* ddate_fmt_dayname()
	Return the name of the Discordian day of the week, or NULL on
	error.

	Argument: the numerical day of the week
*/
char *
ddate_fmt_dayname(ddate_dow, ddate_len);

/* ddate_fmt_season()
	Return the name of the Discordian season, or NULL on error.
*/
char *
ddate_fmt_season(ddate_season, ddate_len);
