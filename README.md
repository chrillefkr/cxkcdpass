# Disclaimer

This project is under development. Don't expect anything to actually work.

# cxkcdpass

A password generator which generates strong passphrases, inspired by [XKCD 936](https://xkcd.com/936/).
Based on [xkcdpass](https://github.com/redacted/XKCD-password-generator), a
Python implementation, whereas `cxkcdpass` is implemented in `C`, therefore the
c prefix in the name.

```
$ cxkcdpass
> eglantine inken reanxiety subgrade purpuroid kerectomy
```

# Install

Installation from source is currently the only way to install cxkcdpass, as
there has been no effort put into making any packages yet.

## Source

Requirements for installing from source:
* gcc/clang
* not sure if there's anything else. I don't think there is (yet).

If you make changes to `cxkcdpass.ggo` (i.e. the gengetopt config), you'll need
to have `gengetopt` installed to re-generate `cmdline.h` and `cmdline.c`.

```
$ make
$ # sudo make install
```

`make install` doesn't work yet :) sorry!

# Requirements

OS needs to be some modern MacOS/Linux, because of mmap, wordexp, arc4random,
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

  -h, --help                    Print help and exit
  -V, --version                 Print version and exit
  -w, --wordfile=WORDFILE       Specify that the file WORDFILE contains the
                                  list of valid words from which to generate
                                  passphrases. Multiple wordfiles can be
                                  provided, separated by commas. Provided
                                  wordfiles: eff-long, eff-short, eff-special,
                                  legacy, spa-mich (Spanish), fin-kotus
                                  (Finnish), fr-freelang (French),
                                  fr-corrected.txt (French), pt-ipublicis
                                  (Portuguese), ita-wiki (Italian), ger-anlx
                                  (German), eff_large_de_sample.wordlist
                                  (German), nor-nb (Norwegian)
                                  (default=`eff-long')
  -m, --min=MIN_LENGTH          Generate passphrases containing words with at
                                  least MIN_LENGTH characters.  (default=`5')
  -M, --max=MAX_LENGTH          Generate passphrases containing words with at
                                  most MAX_LENGTH characters.  (default=`9')
  -n, --numwords=NUM_WORDS      Generate passphrases containing exactly
                                  NUM_WORDS words.  (default=`6')
  -a, --acrostic=ACROSTIC       Generate passphrases with an acrostic matching
                                  ACROSTIC.
  -i, --interactive             Generate and output a passphrase, query the
                                  user to accept it, and loop until one is
                                  accepted.  (default=off)
  -r, --regex-match=REGEX       Limit passphrases to only include words
                                  matching the regex pattern REGEX (e.g.
                                  '^[a-z]*$').
  -c, --count=COUNT             Generate COUNT passphrases.
  -d, --delim=DELIM             Separate words within a passphrase with DELIM.
                                  (default=` ')
  -R, --random-delimiters       Use randomized delimiters between words.
                                  --delimiter will be ignored  (default=off)
  -D, --valid-delimiters=VALID_DELIMITERS
                                A string with all valid delimiter characters.
                                  For example, '^&*' would use ^, &, or *.
                                  (default=`!@#$%^&*()0123456789')
  -s, --sep=SEP                 Separate generated passphrases with SEP.
  -C, --case=ENUM               Choose the method for setting the case of each
                                  word in the passphrase.  (possible
                                  values="initial", "alternating",
                                  "upper", "lower", "random", "first",
                                  "capitalize" default=`initial')
  -v, --verbose                 Report various metrics for given options.
                                  (default=off)
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

So cxkcdpass is approximately ~400 % faster than xkcdpass. Yaay! Can we make it
faster though?

# Contributing

Help is appreciated, but this is also a hobby project for myself, so I don't
think I need any help. Though please report any bugs or issues.

There's a `TODO.md` file with things I want to get done.

# Changelog

* 0.0.1 First MVP working version. Only `--wordfile`, `--min`, `--max` and
`--numwords` implemented.

# License

Copyright © 2022 Christopher Janzon

This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
