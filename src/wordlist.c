#include <sys/mman.h> // mmap()
#include <sys/stat.h> // stat, fstat()
#include <fcntl.h> // open(), O_RDONLY
#include <stdlib.h> // malloc(), free()
#include <stdio.h> // fprintf()
#include <errno.h> // errno
#include <string.h> // strlen(), strdup(), , strerror
#include <regex.h> // regex_t, regcomp, regexec, regfree, regerror

#include "wordlist.h"
#include "common.h" // linelen()

#define WORDLIST_PREALLOC_SIZE 1024
#define LINE_BUFFER_PREALLOC_SIZE 128
#define MIN(a,b) (((a)<(b))?(a):(b))

int mmap_wordfile(char* wordfile, struct mmap_wordfile_t *mw) {
  int fd = open(wordfile, O_RDONLY);
  if (fd == -1) {
    fprintf(stderr, "Error: Could not open '%s' for reading: %s\n", wordfile, strerror(errno));
    return -1;
  }
  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    fprintf(stderr, "Error: Could not fstat '%s': %s\n", wordfile, strerror(errno));
    return -1;
  }
  char* addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (addr == MAP_FAILED) {
    fprintf(stderr, "Error: Could not mmap '%s': %s\n", wordfile, strerror(errno));
    return -1;
  }
  mw->sb = sb;
  mw->addr = addr;
  mw->length = sb.st_size;
  return 0;
}

int generate_wordlist_from_memory(char *addr, size_t length, struct gengetopt_args_info ai, struct wordlist_t *wl) {
  int wordlist_line_count = 0;
  size_t hypothetical_max_words = length / 2;
  // write(STDOUT_FILENO, addr, length);
  size_t wordlist_l = MIN(hypothetical_max_words, WORDLIST_PREALLOC_SIZE);
  char** wordlist = (char**)malloc(wordlist_l * sizeof(char*));
  size_t wordlist_i = 0;

  size_t line_buffer_size = LINE_BUFFER_PREALLOC_SIZE;
  char *line_buffer = NULL;
  regex_t regex_match_regex;
  if (ai.regex_match_given) {
    int ret = regcomp(&regex_match_regex, ai.regex_match_arg, 0);
    if (ret) {
        fprintf(stderr, "Error: Could not compile regex\n");
        return -1;
    }

    line_buffer = (char*) malloc(line_buffer_size * sizeof(char));
  }

  char* last_lb = (char*) addr;
  for (int i = 0; i < length; i++) {
    char c = addr[i];
    if (i != length && c != '\n') continue;
    char* line = last_lb;
    last_lb = &addr[i + 1];

    wordlist_line_count++;

    // filter
    size_t line_length = linelen(line);
    if (line_length > ai.max_arg) continue;
    if (line_length < ai.min_arg) continue;

    if (ai.regex_match_given) {
      if (line_length + 1 > line_buffer_size) {
        do {
        line_buffer_size *= 2;
        } while(line_length + 1 > line_buffer_size);
        line_buffer = (char*) realloc((void*) line_buffer, line_buffer_size * sizeof(char));
      }
      memcpy(line_buffer, line, line_length);
      line_buffer[line_length] = '\0';

      int ret = regexec(&regex_match_regex, line_buffer, 0, NULL, 0);
      if (ret == REG_NOMATCH) continue;
    }

    if (wordlist_i == wordlist_l) {
      wordlist_l *= 2;
      wordlist = (char**)realloc(wordlist, wordlist_l * sizeof(char**));
    }
    wordlist[wordlist_i++] = line;
    // TODO: for acrostic, add to array of pointers to each initial character

    // write(STDOUT_FILENO, last_lb, linelen(last_lb));
  }
  if (line_buffer) free(line_buffer);

  wordlist_l = (wordlist_i);
  if (wordlist_l == 0) {
    fprintf(stderr, "Error: Wordlist has %i words, and we've got 0 after filtering. Can't generate passphrase without words\n", wordlist_line_count);
    free(wordlist);
    return -1;
  }
  if (wordlist_l < ai.num_words_arg) { // TODO: also multiply for --count
    fprintf(stderr, "Error: Too few words for passphrase. Wordlist has %i words, and we've got %lu after filtering.\n", wordlist_line_count, wordlist_l);
    return -1;
  }
  // wordlist_i--;
  wordlist = (char**)realloc(wordlist, (wordlist_l + 1) * sizeof(char**));
  wordlist[wordlist_l] = NULL;
  if (ai.verbose_given) fprintf(stderr, "Info: Wordlist has %i words, and we've got %lu after filtering\n", wordlist_line_count, wordlist_l);
  wl->lines = wordlist;
  wl->length = wordlist_l;
  return 0;
}
