 
/* ========================================

   * File Name : utils.cpp

   * Creation Date : 13-08-2015

   * Last Modified : Mon 31 Aug 2015 12:13:40 PM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include "utils.h"

long long get_argument_long_value(char *optarg, char *optname) {
  char* endp = NULL;
  long value = -1;
  if (!optarg ||  ((value=strtol(optarg, &endp, 10)),(endp && *endp)) ) {
    fprintf(stderr, "Invalid '%s' option: '%s' ... expecting a number\n", optname,
        optarg ? optarg : "");
    exit(EXIT_FAILURE);
  };
  return value;
}

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

length_t get_range(length_t min_len, length_t max_len) {
  length_t range_len = max_len - min_len + 1;
  if (range_len <= 0) {
    fprintf(stderr, "Invalid range of lengths!\n");
    exit(EXIT_FAILURE);
  }
  return range_len;
}

length_t * generate_random_lengths(size_t elems, length_t min_len, length_t max_len) {
  length_t *lengths = (length_t *) calloc(elems, sizeof(length_t));
#ifdef VERBOSE_MODE
  printf("%d-array allocated...\n", elems);
#endif

  length_t range_len = get_range(min_len, max_len);

  static bool srand_for_first_time = true;
  if (srand_for_first_time) {
    srand(time(NULL));
    srand_for_first_time = false;
  }

  for (size_t i = 0; i < elems; i++) {
    lengths[i] = min_len + (rand() % range_len);
  }
#ifdef VERBOSE_MODE
  printf("%d random lengths generated...\n", elems);
#endif
  return lengths;
}

length_t ** allocate_sources(long long total_sources, size_t mep_factor) {
  length_t **sources = (length_t **) calloc(total_sources, sizeof(length_t *));
  for (long long i = 0; i < total_sources; ++i) {
    sources[i] = (length_t *) calloc(mep_factor, sizeof(length_t));
  }
  return sources;
}

void deallocate_sources(length_t **sources, long long total_sources) {
  for (long long i = 0; i < total_sources; ++i) {
    free(sources[i]);
  }
  free(sources);
}


void fill_sources_with_random_lengths(length_t **sources, long long
    total_sources, size_t mep_factor, length_t min_len, length_t max_len) {

  length_t range_len = get_range(min_len, max_len);

  static bool srand_for_first_time = true;
  if (srand_for_first_time) {
    srand(time(NULL));
    srand_for_first_time = false;
  }

  for (long long si = 0; si < total_sources; si++) {
    for (long long mi = 0; mi < mep_factor; mi++) {
      // Segmantation fault can occur here, let system handle such scenario...
      sources[si][mi] = min_len + (rand() % range_len);
    }
  }
}

// modify_lengths_randomly() modifies only the 1st element with random number,
// which will affect all the prefix sums
void modify_lengths_randomly(length_t **sources, long long total_sources,
    length_t min_len, length_t max_len) {

  length_t range_len = get_range(min_len, max_len);

  static bool srand_for_first_time = true;
  if (srand_for_first_time) {
    srand(time(NULL));
    srand_for_first_time = false;
  }

  for (long long si = 0; si < total_sources; si++) {
    sources[si][0] = min_len + (rand() % range_len);
  }
}
