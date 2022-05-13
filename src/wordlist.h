#ifndef WORDLIST_H
#define WORDLIST_H

#include <sys/stat.h> // stat, fstat()
#include "cmdline.h"

struct mmap_wordfile_t {
  struct stat sb;
  char* addr;
  size_t length;
};

int mmap_wordfile(char*, struct mmap_wordfile_t*);


struct wordlist_t {
  size_t length;
  char** lines;
};

int generate_wordlist_from_memory(char*, size_t, struct gengetopt_args_info, struct wordlist_t*);

#endif
