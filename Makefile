
all: cxkcdpass cmdline.c cmdline.h

cxkcdpass: cmdline.c cmdline.h cxkcdpass.c
	@gcc -O2 cmdline.c cxkcdpass.c -o cxkcdpass

cmdline.c cmdline.h: cxkcdpass.ggo
	@gengetopt -i cxkcdpass.ggo -G
