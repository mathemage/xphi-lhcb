 
/* ========================================

   * File Name : utils.cpp

   * Creation Date : 13-08-2015

   * Last Modified : Fri 14 Aug 2015 03:50:16 PM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include "utils.h"

template<typename T>
void show_array(T *arr, size_t elems, const int fixed_width, const size_t
    show_limit, const size_t elems_on_line) {
  if (elems > show_limit) {
    printf("Too many numbers to display!\n");
    return;
  }

  for (size_t i = 0; i < elems; i++) {
    printf("%*d", fixed_width, arr[i]);
    if (i % elems_on_line == elems_on_line - 1) {
      printf("\n");
    } else {
      printf("\t");
    }
  }
}

void show_lengths(length_t *lengths, size_t elems) {
  show_array<length_t>(lengths, elems, 5, 80, 16);
}

void show_offsets(offset_t *offsets, size_t elems) {
  show_array<offset_t>(offsets, elems, 10, 80, 8);
}

length_t * generate_random_lengths(size_t elems, length_t min_len, length_t max_len) {
  length_t range_len = max_len - min_len + 1;
  length_t *lengths = (length_t *) calloc(elems, sizeof(length_t));
#ifdef VERBOSE_MODE
  printf("%d-array allocated...\n", elems);
#endif

  if (range_len <= 0) {
    fprintf(stderr, "Invalid range of lengths!\n");
    exit(EXIT_FAILURE);
  }

  srand(time(NULL));
  for (size_t i = 0; i < elems; i++) {
    lengths[i] = min_len + (rand() % range_len);
  }
#ifdef VERBOSE_MODE
  printf("%d random lengths generated...\n", elems);
#endif
  printf("Lengths:\n");
  show_lengths(lengths, elems);
  return lengths;
}
