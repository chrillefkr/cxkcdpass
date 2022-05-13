#include <sodium.h> // randombytes_uniform()

#include "random.h"

int initialize_random() {
  int ret;
  if ((ret = sodium_init()) < 0) {
    return ret;
  }
  return 0;
}

inline int generate_random_number(int num) {
  if (num == 0) return 0;
  return randombytes_uniform(num);
}

int* get_random_choices(size_t min_amount, size_t max_amount, size_t min_value, size_t max_value){
  int a_diff = max_amount - min_amount;
  int v_diff = max_value - min_value;

  if (min_amount > max_amount || min_value > max_value) return NULL;

  int amount;
  if (a_diff != 0)
    amount = min_amount + generate_random_number(a_diff);
  else {
    amount = min_amount;
  }

  int* out = (int*) malloc((amount + 1) * sizeof(int));
  for (int i = 0; i < amount; i++) {
    if (v_diff != 0) {
      out[i] = min_value + generate_random_number(v_diff);
    } else {
      out[i] = min_value;
    }
  }
  out[amount] = -1;
  return out;
}
