#ifndef RANDOM_H
#define RANDOM_H

#include <stddef.h> // size_t

int initialize_random();
int* get_random_choices(size_t, size_t, size_t, size_t);

#endif
