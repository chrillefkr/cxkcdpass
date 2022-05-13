#include <stddef.h> // size_t

#include "common.h"

#ifndef __OPTIMIZE__
size_t linelen(char* line) {
  size_t i = 0;
  while (line[i] != '\n' && line[i] != '\0') i++;
  return i;
}
#endif
