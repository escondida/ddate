# Perpetual date converter from Gregorian to POEE calendar

Official Discordian Document Numbering System
item ddate:00001

## Sacred Document of the Frogs (old Erisian poem)

	73 Days hath
	Chaos, Discord, Confusion, Bureaucracy, and
	                                  Aftermath

## Okay, haha, seriously, what?

This is a program which converts Gregorian dates into the Erisian calendar,
as described in the [Principia Discordia: 00034][pd00034].
It's very useful.
Stop looking at me like that.

## About this version

It all started when I wanted to fix the infamous
[Saint Tib's Day Bug][bo0tsbug00013]
in the standalone version of ddate.
It was easy enough to do,
but then I also wanted to fiddle with some other things,
and I discovered that since the code was mostly in two big functions,
it was difficult to do that in a reasonable way.
So I did what any sensible person would do:
I rewrote pretty much the whole damn thing.
It didn't hurt that I was stuck inside
waiting for the collapse of civilization, either.

Nevertheless, the project would not have been possible
without the original authors' hard work,
which is by no means to be discounted.
See also the [version history][

### Incompatibilities

- You can no longer crash the program by reversing the brackets
in the format string on St. Tib's Day.

- The formatting strings have changed;
please refer to the manual for more information.

- Older versions hardcoded a list of slogans into the format function.
This version looks for slogans
in a file called "slogans" in the following places,
in order of preference:
the current working directory,
$LOCALDIR/data/ddate,
$XDG_DATA_HOME/ddate,
$HOME/.local/share/ddate,
$DATADIR/ddate
(DATADIR = $PREFIX/share by default)

- Other versions let you choose at compile-time
between providing the date arguments
as baffling, American-style dates (month day year)
and as normal dates (day month year).
This version gives you the only true choice,
which is no choice:
it will only accept the format year month day.

## Building

You will need mk, from plan9port, to build this.

	./configure [-c clang] [--prefix /usr]
	mk
	mk [DESTDIR=./pkg] install

## Helping out

See HACKING.
Or write it if it still hasn't been written.

If you'd like to add to this program but aren't into programming,
consider adding new slogans to ddata/slogans.
If you introduce an entirely new class of slogans,
please make a new file to contain them and add it to mkfile.in.

Note that the slogans should be relatively short,
and *must* be on a single line.
If you have some long-form text to contribute,
consider adding it to ddata/wisdom instead
and--well, who knows.
Maybe someone will read it.
Maybe someone will make a random wisdom file printer.

Finally, of course, if you find a bug, please submit it to
[the bugtracker][esbug].

## The Calendar

To see the whole year at a glance,
please refer directly to the [Principia][pd00034].
Note that only one calendar is necessary:
every year begins with Sweetmorn, 1st Chaos,
and ends with Setting Orange, 73rd Aftermath.

### The Year

Ol' Greg's 2020 CE is YOLD 3186.
Years of Our Lady Discord are dated from [the original snub][00017]

### Seasons & Saints

You can find a [fuller explanation][pd00039] in the [Principia][pd00040].

1. Chaos: Patron Apostle Hung Mung
2. Discord: Patron Apostle Dr. Van Van Mojo,
or was it Patamunzo Lingananda?
3. Confusion: Patron Apostle Sri Syadasti
4. Bureaucracy: Patron Apostle Zarathud
5. The Aftermath: Patron Apostle the Elder Malaclypse

### Days of the Week

The days of the week take their names from the five basic elements:
Sweet, Boom, Pungent, Prickle, and Orange.

1. Sweetmorn
2. Boomtime
3. Pungenday
4. Prickle-Prickle
5. Setting Orange

### Holydays

#### Apostle Holydays

Each Apostle Holyday occurs on the 5th day of their season.

1. Mungday
2. Mojoday (Lingananday?)
3. Syaday
4. Zaraday
5. Maladay

#### Seasonal Holydays

Each Seasonal Holyday occurs on the 50th day of the season.

1. Chaoflux
2. Discoflux
3. Confuflux
4. Bureflux
5. Afflux

#### St. Tib's Day

Perhaps the most ineffable of Holydays,
St. Tib's Day occurs once every 4 years (1 + 4 = 5),
*obviously* not counting years which in the Gregorian calendar
are multiples of 100,
*obviously* unless it's also a multiple of 400
(100 + 400 = 500; 500 / 100 = 5).

It has no numerical date,
and may be found between the 59th and 60th days
of the Season of Chaos.

## Links

If you're viewing the HTMLized version of this,
don't worry about this section.
There are no secret messages in the original markdown, I assure you.

[bo0tsbug00013]: https://github.com/bo0ts/ddate/issues/13
[esbug]: https://github.com/escondida/ddate/issues/
[pd00034]: https://www.principiadiscordia.com/book/41.php
[pd00039]: https://www.principiadiscordia.com/book/46.php
[pd00040]: https://www.principiadiscordia.com/book/47.php
