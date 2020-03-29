/* bob.h

	Stuff to do with the Subgenius
*/


/* Code for counting down to X-Day, X-Day being Cfn 40, 3164
 *
 * After `X-Day' passed without incident, the CoSG declared that it had
 * got the year upside down --- X-Day is actually in 8661 AD rather than
 * 1998 AD.
 *
 * Thus, the True X-Day is Cfn 40, 9827.
 *
 * Note that if the world fails to end on X-Day, someone in the year
 * 9827 will need to update this code.
*/

/* xday_countdown_days()
	Note that this is the number of days between the calendar date
	of the given day and xday, *NOT* taking years into account. To
	get the years 'til then, use xday_countdown_years.
*/
int32_t
xday_countdown_days(int32_t day);

int32_t
xday_countdown_years(int32_t year);
