#ifndef COMMON_H
#define COMMON_H

#include <stddef.h> // size_t

#ifdef __OPTIMIZE__
inline size_t linelen(char* line) {
  size_t i = 0;
  while (line[i] != '\n' && line[i] != '\0') i++;
  return i;
}
#else
size_t linelen(char*);
#endif


#endif
