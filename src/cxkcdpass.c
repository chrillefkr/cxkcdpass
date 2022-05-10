#include <stdio.h> // fprintf()
#include <stdlib.h> // malloc(), free(), arc4random_uniform()
#include <string.h> // strlen(), strdup()
#include <wordexp.h> // wordexp_t, wordexp(), wordfree()
#include <sys/stat.h> // stat, fstat()
#include <unistd.h> // write()
#include <sys/mman.h> // mmap()
#include <fcntl.h> // open(), O_RDONLY
#include <errno.h> // errno

#include <regex.h> // regex_t, regcomp, regexec, regfree, regerror

#include "cmdline.h"

#define WORDLIST_PREALLOC_SIZE 1024
#define LINE_BUFFER_PREALLOC_SIZE 128
#define MIN(a,b) (((a)<(b))?(a):(b))

size_t linelen(char* line) {
  size_t i = 0;
  while (line[i] != '\n') i++;
  return i;
}



int validate_options(struct gengetopt_args_info ai) {
  if (ai.max_arg < ai.min_arg) {
    ai.max_arg = ai.min_arg;
    fprintf(stderr, "Warning: maximum word length less than minimum. Setting maximum equal to minimum.\n");
  }
  if (
    // ai.min_given ||
    // ai.max_given ||
    // ai.numwords_given ||
    ai.acrostic_given ||
    ai.interactive_given ||
    // ai.regex_match_given ||
    ai.count_given ||
    ai.delim_given ||
    ai.random_delimiters_given ||
    ai.valid_delimiters_given ||
    ai.sep_given ||
    ai.case_given
  ) {
    fprintf(stderr, "Error: Passed arguments aren't supported yet. Sorry ¯\\_(ツ)_/¯\n");
    return -1;
  }
  // Make sure acrostic and numwords aren't both set
  return 0;
}

char* locate_wordfile(char* wordfile, struct gengetopt_args_info ai) {
  struct stat st;
  wordexp_t exp_result;
  wordexp(wordfile, &exp_result, WRDE_NOCMD);
  char **stat_error_msgs;
  if (ai.wordfile_given) {
    stat_error_msgs = (char**)malloc(exp_result.we_wordc * sizeof(char*));
  }
  for (int i = 0; i < exp_result.we_wordc; i++) {
    int stat_res = stat(exp_result.we_wordv[i], &st);
    // Check if file is a regular file on disk, as mmap doesn't support anything else
    if (stat_res != 0 || !S_ISREG(st.st_mode)) {
      if (ai.wordfile_given) {
        if (stat_res != 0) stat_error_msgs[i] = strdup(strerror(errno));
        else stat_error_msgs[i] = strdup("Not a regular file, i.e. unsupported.");
      }
      continue;
    }
    if (ai.wordfile_given) {
      for (int j = 0; j < i; j++) {
        free(stat_error_msgs[j]);
      }
    }
    return strdup(exp_result.we_wordv[i]);
  }

  // if run with -w/--wordfile, let user know we couldn't find the file, and why
  if (ai.wordfile_given) {
    if (exp_result.we_wordc == 0) fprintf(stderr, "Warning: Can't find file '%s': unknown error, please report it as a bug\n", wordfile);
    else if (exp_result.we_wordc == 1) {
      fprintf(stderr, "Warning: Can't find file '%s': %s\n", exp_result.we_wordv[0], stat_error_msgs[0]);
      free(stat_error_msgs[0]);
    } else if (exp_result.we_wordc > 1) {
      fprintf(stderr, "Warning: Can't find files:\n");
      for (int i = 0; i < exp_result.we_wordc; i++) {
        fprintf(stderr, "  %s: %s\n", exp_result.we_wordv[i], stat_error_msgs[i]);
        free(stat_error_msgs[i]);
      }
    }
  }
  wordfree(&exp_result);

  // TODO: Also try for ./static folder

  const char common_word_files[4][35] = {
    "/usr/share/cracklib/cracklib-small",
    "/usr/share/dict/cracklib-small",
    "/usr/dict/words",
    "/usr/share/dict/words",
  };
  for (int i = 0; i < sizeof(common_word_files); i++) {
    if (stat(common_word_files[i], &st) != 0 || !S_ISREG(st.st_mode)) continue;
    if (ai.wordfile_given || ai.verbose_given) fprintf(stderr, "Info: Using wordfile '%s'.\n", common_word_files[i]);
    return strdup(common_word_files[i]);
  }

  return NULL;
}

struct mmap_wordfile_t {
  struct stat sb;
  char* addr;
  size_t length;
};

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

struct wordlist_t {
  size_t length;
  char** lines;
};
int generate_wordlist(char *addr, size_t length, struct gengetopt_args_info ai, struct wordlist_t *wl) {
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
      printf("line_buffer: %s\n", line_buffer);

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
  // wordlist_i--;
  wordlist = (char**)realloc(wordlist, (wordlist_l + 1) * sizeof(char**));
  wordlist[wordlist_l] = NULL;
  if (ai.verbose_given) fprintf(stderr, "Info: Wordlist has %i words, and we've got %lu after filtering\n", wordlist_line_count, wordlist_l);
  wl->lines = wordlist;
  wl->length = wordlist_l;
  return 0;
}

int generate_wordlist2(char* wordfile, struct gengetopt_args_info ai) {
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
  size_t hypothetical_max_words = sb.st_size / 2;
  #define WORDLIST_PREALLOC_SIZE 1024
  // write(STDOUT_FILENO, addr, sb.st_size);
  #define MIN(a,b) (((a)<(b))?(a):(b))
  size_t wordlist_l = MIN(hypothetical_max_words, WORDLIST_PREALLOC_SIZE);
  char** wordlist = (char**)malloc(wordlist_l * sizeof(char*));
  size_t wordlist_i = 0;

  char* last_lb = (char*) addr;
  for (int i = 0; i < sb.st_size; i++) {
    char c = addr[i];
    if (i != sb.st_size && c != '\n') continue;
    if (wordlist_i == wordlist_l) {
      wordlist_l *= 2;
      wordlist = (char**)realloc(wordlist, wordlist_l * sizeof(char**));
    }
    wordlist[wordlist_i++] = last_lb;
    // TODO: for acrostic, add to array of pointers to each initial character
    // write(STDOUT_FILENO, last_lb, linelen(last_lb));
    last_lb = &addr[i + 1];

  }
  wordlist_l = (wordlist_i);
  // wordlist_i--;
  wordlist = (char**)realloc(wordlist, wordlist_l * sizeof(char**));
  fprintf(stderr, "Info: Found %lu words in wordlist\n", wordlist_l);

  for (size_t i = 0; i < wordlist_l; i++) {
    // Filter
  }

  munmap(addr, sb.st_size);
  return 0;
}

int* get_random_choices(int min_amount, int max_amount, int min_value, int max_value){
  int a_diff = max_amount - min_amount;
  int v_diff = max_value - min_value;
  int amount = min_amount + (a_diff == 0 ? 0 : arc4random_uniform(a_diff));
  int* out = (int*) malloc((amount + 1) * sizeof(int));
  for (int i = 0; i < amount; i++) out[i] = min_value + (v_diff == 0 ? 0 : arc4random_uniform(v_diff));
  out[amount] = -1;
  return out;
}

int print_passphrase(int* wordlist_choices, char** wordlist, char* delimiters) {
  for (int i = 0; wordlist_choices[i] != -1; i++) {
    int choice = wordlist_choices[i];
    char* line = wordlist[choice];
    size_t line_length = linelen(line);
    write(STDOUT_FILENO, line, line_length);
    if (wordlist_choices[i+1] != -1) write(STDOUT_FILENO, " ", 1);
  }
  write(STDOUT_FILENO, "\n", 1);
  return 0;
}

int main(int argc, char* argv[]) {
  struct gengetopt_args_info ai;
  if (cmdline_parser(argc, argv, &ai) != 0) {
    return 2;
  }
  if (validate_options(ai) != 0) {
    return 2;
  }
  char* wordfile = locate_wordfile(ai.wordfile_arg, ai);
  if (wordfile == NULL) {
    fprintf(stderr, "Error: Could not find a word file.\n");
  }
  struct mmap_wordfile_t mw;
  if (mmap_wordfile(wordfile, &mw) != 0) {
    return 1;
  }
  struct wordlist_t wl;
  if (generate_wordlist(mw.addr, mw.length, ai, &wl) != 0) {
    return 1;
  }
  int* wordlist_choices = get_random_choices(ai.numwords_arg, ai.numwords_arg, 0, wl.length);
  print_passphrase(wordlist_choices, wl.lines, NULL);
  munmap(mw.addr, mw.length);
  return 0;
}
