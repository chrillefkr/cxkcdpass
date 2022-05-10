CC = gcc
CCFLAGS =

UNAME_S := $(shell uname -s)
WSLENV ?= notwsl
ifeq ($(UNAME_S),Linux)
  CCFLAGS += -D LINUX -lbsd -lsodium

	ifndef WSLENV
	  CCFLAGS += -D WSL
	endif
endif
ifeq ($(UNAME_S),Darwin)
  CCFLAGS += -D OSX
endif
UNAME_P := $(shell uname -p)
ifeq ($(UNAME_P),x86_64)
  CCFLAGS += -D AMD64
endif
ifneq ($(filter %86,$(UNAME_P)),)
  CCFLAGS += -D IA32
endif
ifneq ($(filter arm%,$(UNAME_P)),)
  CCFLAGS += -D ARM
endif


all: cxkcdpass src/cmdline.c src/cmdline.h

cxkcdpass: src/cmdline.c src/cmdline.h src/cxkcdpass.c
	$(CC) -O2 src/cmdline.c src/cxkcdpass.c -o cxkcdpass $(CCFLAGS)

src/cmdline.c src/cmdline.h: cxkcdpass.ggo
	gengetopt --input=cxkcdpass.ggo --output-dir=./src/
