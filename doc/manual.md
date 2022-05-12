---
section: 1
date: May 12, 2022
---
# NAME

cxkcdpass - generate random passphrases from wordlist(s)


# SYNOPSIS

**cxkcdpass** [*OPTION*...]


# DESCRIPTION

A password generator which generates strong passphrases from wordlists, inspired
by [XKCD 936](https://xkcd.com/936/).
Based on [xkcdpass](https://github.com/redacted/XKCD-password-generator), a
Python implementation, whereas `cxkcdpass` is implemented in `C`, therefore the
c in cxkcdpass.

```
$ cxkcdpass
> coroner nutlike sandbank remarry judge yesterday
```


# OPTIONS

**-h**, **--help**
: Print help and exit

**-V**, **--version**
: Print version and exit

**-w** *WORDFILE*, **--wordfile**=*WORDFILE*
: Specify that the file *WORDFILE* contains the list of valid words from which to
generate passphrases. Multiple wordfiles can be provided.
Provided wordfiles: `eff-long`, `eff-short`, `eff-special`, `legacy`, `spa-mich`
(Spanish), `fin-kotus` (Finnish), `fr-freelang` (French), `fr-corrected.txt` (French),
`pt-ipublicis` (Portuguese), `ita-wiki` (Italian), `ger-anlx` (German),
`eff_large_de_sample.wordlist` (German), `nor-nb` (Norwegian) (default=`eff-long`)


**-m**, **--min**=*MIN_LENGTH*
: Generate passphrases containing words with at least *MIN_LENGTH* characters.

**-M**, **--max**=*MAX_LENGTH*
: Generate passphrases containing words with at most *MAX_LENGTH* characters.

**-n**, **--num-words**=*NUM_WORDS*
: Generate passphrases containing exactly *NUM_WORDS* words.

**-r**, **--regex-match**=*REGEX*
: Limit passphrases to only include words matching the regex pattern *REGEX*
(e.g. `'\^[a-z]*$'`).

**-v**, **--verbose**
: Report various metrics for given options and general verbose output.


# EXAMPLES

**cxkcdpass**
: Prints a random passphrase

**cxkcdpass -w ./my-list-of-words.txt**
: Prints a random passphrase based on words in ./my-list-of-words.txt

**cxkcdpass -n 10**
: Prints a random passphrase with 10 words


# BUGS

Report bugs and issues on GitHub:
[https://github.com/chrillefkr/cxkcdpass/issues](https://github.com/chrillefkr/cxkcdpass/issues)

# AUTHOR
Christopher Janzon [git@christopherjanzon.com](mailto:git@christopherjanzon.com)

# COPYRIGHT & LICENSE

Copyright © 2022 Christopher Janzon

This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
