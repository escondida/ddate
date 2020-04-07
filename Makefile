CC ?= cc
PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin
DATADIR ?= $(PREFIX)/share
DOCDIR ?= $(DATADIR)/doc
MANDIR ?= $(DATADIR)/man

ifeq ($(shell test -d .git && echo true), true)
VERSION != git describe --long | sed 's@-\(.*\)-@.r\1.@'
else ifeq ($(shell test -e ddata/meta/version && echo true), true)
VERSION != cat ddata/meta/version
else
VERSION = unknown
endif

ifeq ($(CC), clang)
# -Wno-disable-macro-expansion: gotta have my fprintf
CFLAGS += \
	-Weverything -fno-diagnostics-color \
	-fsanitize-trap=undefined -Wno-disabled-macro-expansion
endif

TARGS = ddate slogans

SRCS = src/bob.c \
	src/convert.c \
	src/ddate.c \
	src/fmt.c \
	src/slogans.c \
	src/tibs.c \
	src/util.c

OBJS = $(SRCS:%.c=%.o)
DEPS = $(SRCS:%.c=%.d)

VDIRS = src

SLOGANS = \
	ddata/slogans/discordian \
	ddata/slogans/illuminati \
	ddata/slogans/miscellaneous \
	ddata/slogans/neologisms \
	ddata/slogans/subgenius

ifeq ($(shell uname), Linux)
SLOGANS += ddata/slogans/linux
else ifeq ($(shell uname), darwin)
SLOGANS += ddata/slogans/apple
endif

CFLAGS += \
	-Werror -g -ggdb -std=gnu11 \
	-O2 -fPIE -march=native -D_FORTIFY_SOURCE=2 \
	-fstack-protector-strong --param=ssp-buffer-size=1 -flto \
	-fsanitize=undefined \
	-DPREFIX=\"$(PREFIX)\" -DDATADIR=\"$(DATADIR)\" \
	-DVERSION=\"$(VERSION)\"

LDFLAGS += -Wl,-z,relro,-z,now,--build-id -pie

all: flags $(TARGS)

ddate: $(OBJS)
	@echo $(CC) -o $@ $^
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

slogans: $(SLOGANS)
	cat $^ > $@

flags:
	@echo CFLAGS: $(CFLAGS)
	@echo LDFLAGS: $(LDFLAGS)

%.o: %.c
	@echo $(CC) -c -o $@ $^
	@$(CC) $(CFLAGS) -MMD -c -o $@ $<

-include $(DEPS)

install: $(TARGS)
	install -D ddate $(DESTDIR)$(BINDIR)/ddate
	install -D -m 644 slogans $(DESTDIR)$(DATADIR)/ddate/slogans
	install -D -m 644 doc/ddate.1 $(DESTDIR)$(MANDIR)/man1/ddate.1
	install -D -m 644 ddata/slogans/README $(DESTDIR)$(DOCDIR)/ddate/slogans
	install -m 644 doc/index doc/versions README.md \
			$(DESTDIR)$(DOCDIR)/ddate

# Honestly, you shouldn't do this.
# You ought to have packaged it using your distro's packaging tools.
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/ddate \
		$(DESTDIR)$(DATADIR)/ddate/slogans \
		$(DESTDIR)$(MANDIR)/man1/ddate.1 \
		$(DESTDIR)$(DOCDIR)/ddate/slogans \
		$(DESTDIR)$(DOCDIR)/ddate/*
	rmdir $(DESTDIR)$(DATADIR)/ddate \
		$(DESTDIR)$(DOCDIR)/ddate


clean:
	rm -f $(TARGS) $(OBJS) $(DEPS)

.PHONY: all clean flags install uninstall
