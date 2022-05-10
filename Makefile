CC="gcc"

all: cxkcdpass src/cmdline.c src/cmdline.h

cxkcdpass: src/cmdline.c src/cmdline.h src/cxkcdpass.c
	$(CC) -O2 src/cmdline.c src/cxkcdpass.c -o cxkcdpass

src/cmdline.c src/cmdline.h: cxkcdpass.ggo
	gengetopt --input=cxkcdpass.ggo --output-dir=./src/
