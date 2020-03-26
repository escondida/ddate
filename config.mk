MKSHELL = rc
CC = clang

# No -Werror for now.
CFLAGS = -Weverything -g -ggdb -fno-diagnostics-color \
	-std=gnu11 -O2 -fPIE -march=native -D_FORTIFY_SOURCE=2 \
	-fstack-protector-strong --param=ssp-buffer-size=1 -flto \
	-fsanitize=undefined -fsanitize-trap=undefined \
	`{pkg-config --cflags glew glfw3 libcurl sqlite3} \
	$CFLAGS \
	-Wno-disabled-macro-expansion # fprintf

LDFLAGS = -Wl,-z,relro,-z,now,--build-id -pie \
	`{pkg-config --libs glew glfw3 libcurl sqlite3} \
	-lm -lpthread \
	$LDFLAGS

all:V: $TARGS

%:Q: %.o
	echo $CC -o $target $prereq
	$CC $CFLAGS $LDFLAGS -o $target $prereq

%.o:Q: %.c
	echo $CC -c -o $target $stem.c
	$CC $CFLAGS -c -o $target $stem.c

clean:V:
	rm -f $TARGS *.o
