/* fmt.h

	The actual formatting takes place here. You most likely want
	ddate_fmt, unless you're writing a formatter of your own.
*/

/* ddate_fmt()
	Format a Discordian date (and sundry other nonsense).
	The formatted string is saved to *buf.
*/
ddate_error
ddate_fmt(char *buf, size_t bufsize, struct ddate dd, const char *fmt);

/* ddate_fmt_dayname()
	Return the name of the Discordian day of the week, or NULL on
	error.

	Arguments: the numerical day of the week, length of desired
	result
*/
char *
ddate_fmt_dayname(ddate_dow, ddate_len);

/* ddate_fmt_era
	If bs (before snub) is false, return "BS".
	Otherwise, return "YOLD".
*/
char *
ddate_fmt_era(bool bs, ddate_len len);

/* ddate_fmt_holyday()
	Return the name of the Discordian holyday, or NULL on
	error.

	Arguments: day of the season, whether the user is a member of
	the Laughing Christ sect and therefore considers Apostle
	Patamunzo Lingananda to be the patron of the Season of Discord
	rather than Apostle Dr. Van van Mojo (PD:00039).
*/
char *
ddate_fmt_holyday(ddate_holyday hday, bool laughingchrist);

/* ddate_fmt_season()
	Return the name of the Discordian season, or NULL on error.


*/
char *
ddate_fmt_season(ddate_season, ddate_len);
