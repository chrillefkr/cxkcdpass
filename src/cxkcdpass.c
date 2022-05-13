#include <stdio.h> // fprintf()
#include <stdlib.h> // malloc(), free()
#include <string.h> // strlen(), strdup(), strerror
#include <wordexp.h> // wordexp_t, wordexp(), wordfree()
#include <sys/stat.h> // stat, fstat()
#include <unistd.h> // write()
#include <errno.h> // errno
#include <sys/mman.h> // mmap(), munmap()

#include "cmdline.h"
#include "common.h" // linelen()
#include "random.h"
#include "wordlist.h"

int validate_options(struct gengetopt_args_info ai) {
  if (ai.max_arg < ai.min_arg) {
    ai.max_arg = ai.min_arg;
    fprintf(stderr, "Warning: maximum word length less than minimum. Setting maximum equal to minimum.\n");
  }
  // TODO: Make sure acrostic and num-words aren't both set
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

  const char* common_word_files[4] = {
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
  if (initialize_random() != 0) {
    fprintf(stderr, "Fatal: Could not initialize RNG.\n");
    return 1;
  }
  if (validate_options(ai) != 0) {
    return 2;
  }
  char* wordfile = locate_wordfile(ai.wordfile_arg, ai);
  if (wordfile == NULL) {
    fprintf(stderr, "Error: Could not find a word file.\n");
    return 1;
  }
  struct mmap_wordfile_t mw;
  if (mmap_wordfile(wordfile, &mw) != 0) {
    free(wordfile);
    return 1;
  }
  free(wordfile);
  struct wordlist_t wl;
  if (generate_wordlist_from_memory(mw.addr, mw.length, ai, &wl) != 0) {
    return 1;
  }
  int* wordlist_choices = get_random_choices(ai.num_words_arg, ai.num_words_arg, 0, wl.length);
  print_passphrase(wordlist_choices, wl.lines, NULL);
  free(wordlist_choices);
  free(wl.lines);
  munmap(mw.addr, mw.length);
  cmdline_parser_free(&ai);
  return 0;
}
