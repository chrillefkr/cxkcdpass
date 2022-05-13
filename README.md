# Disclaimer

This project is under development. Don't expect anything to actually work.

# cxkcdpass

A password generator which generates strong passphrases from wordlists, inspired
by [XKCD 936](https://xkcd.com/936/).
Based on [xkcdpass](https://github.com/redacted/XKCD-password-generator), a
Python implementation, whereas `cxkcdpass` is implemented in `C`, therefore the
c in cxkcdpass.

```
$ cxkcdpass
> coroner nutlike sandbank remarry judge yesterday
```

# Install

Installation from source is currently the only way to install cxkcdpass, as
there has been no effort put into making any packages yet.

## From source

Requirements for installing from source:
* gcc/clang compatible compiler
* Windows:
  * Not yet supported
* MacOS/BSD:
  * Nothing, as arc4random is already included
* Linux
  * libsodium, for RNG
    * E.g. `$ sudo apt install libsodium-dev`

### Tarball

TODO: As I haven't made any releases yet, there are no tarballs to install from.

### Straight off this git repository

Requires autotools (automake, autoconf, etc)

```
$ git clone git@github.com:chrillefkr/cxkcdpass.git && cd cxkcdpass
$ autoreconf --install
$ cd build
$ ../configure
$ make
$ make install
```

`make install` doesn't work yet :) sorry!

# Requirements

OS needs to be some modern MacOS/Linux, because of mmap, wordexp,
etc. until cxkcdpass has been adopted to work on Windows.

Wordlists are not included yet. There are some available for grabs at the
[xkcdpass](https://github.com/redacted/XKCD-password-generator/tree/master/xkcdpass/static) repository.
You can specify wordlist with the `--wordfile` parameter.
cxkcdpass will look for wordlists in the following order:

* `--wordfile <WORDFILE>` path
  * The absolute `<WORDFILE>` path, and
  * /path/to/cxkcdpass/installation/`<WORDFILE>` path
* /usr/share/cracklib/cracklib-small
* /usr/share/dict/cracklib-small
* /usr/dict/words
* /usr/share/dict/words

# Usage

Output from running `$ cxkcdpass --help`:
```
Usage: cxkcdpass [OPTION]...
Generate random passphrases from a wordfile

  -h, --help                 Print help and exit
  -V, --version              Print version and exit
  -w, --wordfile=WORDFILE    Specify that the file WORDFILE contains the list
                               of valid words from which to generate
                               passphrases. Multiple wordfiles can be provided.
                               Provided wordfiles: eff-long, eff-short,
                               eff-special, legacy, spa-mich (Spanish),
                               fin-kotus (Finnish), fr-freelang (French),
                               fr-corrected.txt (French), pt-ipublicis
                               (Portuguese), ita-wiki (Italian), ger-anlx
                               (German), eff_large_de_sample.wordlist (German),
                               nor-nb (Norwegian)  (default=`eff-long')
  -m, --min=MIN_LENGTH       Generate passphrases containing words with at
                               least MIN_LENGTH characters.  (default=`5')
  -M, --max=MAX_LENGTH       Generate passphrases containing words with at most
                               MAX_LENGTH characters.  (default=`9')
  -n, --num-words=NUM_WORDS  Generate passphrases containing exactly NUM_WORDS
                               words.  (default=`6')
  -r, --regex-match=REGEX    Limit passphrases to only include words matching
                               the regex pattern REGEX (e.g. '^[a-z]*$').
  -v, --verbose              Report various metrics for given options and
                               general verbose output.  (default=off)
```

# Performance

I hope cxkcdpass will be faster, smaller and in general more optimized and
efficient than the Python counterpart. Otherwise, it's no fun coding in C.

Tested on my MacBook Pro (Late 2013, x86_64), compiled with `Apple clang version 13.0.0 (clang-1300.0.29.30)`.

```
$ time xkcdpass
subheader grant spearfish employer linguini defacing

real    0m0.066s
user    0m0.042s
sys     0m0.018s

$ time cxkcdpass
unfleece satine unbitted myelocyte creole loftsman

real    0m0.016s
user    0m0.008s
sys     0m0.006s
```

And testing with a million (1 000 000) words:

```
$ time xkcdpass -n 1000000 >/dev/null

real    0m7.954s
user    0m7.546s
sys     0m0.397s

$ time cxkcdpass -n 1000000 >/dev/null

real    0m1.606s
user    0m1.035s
sys     0m1.898s
```

So cxkcdpass is approximately 4-5 times faster than xkcdpass. Yaay! Can we make it
faster though?

# Contributing

Help is appreciated, but this is also a hobby project for myself, so I don't
think I need any help. Though please report any bugs or issues.

There's a `TODO.md` file with things I want to get done.

If you make changes to `cxkcdpass.ggo` (i.e. the gengetopt config), you'll need
to have `gengetopt` installed to re-generate `cmdline.h` and `cmdline.c`.
Commandline used: `gengetopt --input=cxkcdpass.ggo --output-dir=./src/`. See
[pre-build.sh](./pre-build.sh)

Makefile.dev helps you out with this. Invoke with

```
$ make -f Makefile.dev
```

# Changelog

* 0.0.1 First MVP working version. Only `--wordfile`, `--min`, `--max`,
`--numwords` and `--regex-match` implemented.

# License

Copyright © 2022 Christopher Janzon

This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
