 
/* ========================================

   * File Name : utils.cpp

   * Creation Date : 13-08-2015

   * Last Modified : Mon 12 Oct 2015 01:09:18 PM CEST

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
  printf("\n");
}


void show_lengths(length_t *lengths, size_t elems) {
  show_array<length_t>(lengths, elems, 5, 80, 16);
}


void show_offsets(offset_t *offsets, size_t elems) {
  show_array<offset_t>(offsets, elems, 10, 80, 8);
}


length_t get_range(length_t min_len, length_t max_len) {
  length_t range_len = max_len - min_len + 1;
  if (min_len >= max_len || range_len <= 0) {
    fprintf(stderr, "Invalid range of lengths!\n");
    exit(EXIT_FAILURE);
  }
  return range_len;
}


void init_srand() {
  static bool srand_for_first_time = true;
  if (srand_for_first_time) {
    srand(time(NULL));
    srand_for_first_time = false;
  }
}


void * try_malloc(size_t size) {
  void *chunk = malloc(size);
  if (chunk == NULL) {
    printf("Exitting: malloc failed!\n");
    exit(EXIT_FAILURE);
  }
  return chunk;
}


void * try_calloc(size_t num, size_t size) {
  void *chunk = calloc(num, size);
  if (chunk == NULL) {
    printf("Exitting: calloc failed!\n");
    exit(EXIT_FAILURE);
  }
  return chunk;
}


double get_mean_value(const vector<double> & values) {
  return accumulate(values.begin(), values.end(), 0.0) / values.size();
}


void time_statistics(const vector<double> & data_points) {
  double bottom = *min_element(data_points.begin(), data_points.end());
  double top = *max_element(data_points.begin(), data_points.end());
  int total_bins = ceil(sqrt(data_points.size()));
  vector<int> bins(total_bins, 0);
  double bin_width = (top - bottom) / total_bins;

  printf("min: %.5f secs\n", bottom);
  printf("max: %.5f secs\n", top);
  printf("mean: %.5f secs\n", get_mean_value(data_points));
  printf("Histogram:\n");
  int bin_index;
  for (auto & val : data_points) {
    bin_index = (val - bottom) / bin_width;
    bin_index = max(bin_index, 0);
    bin_index = min(bin_index, total_bins - 1);
    bins[bin_index]++;
  }
  for (int i = 0; i < total_bins; ++i) {
    printf("[%.5f, %.5f): %d times\n", bottom + i * bin_width, bottom + (i+1) * bin_width, bins[i]);
  }
}


length_t * generate_random_lengths(size_t elems, length_t min_len, length_t max_len) {
  length_t *lengths = (length_t *) calloc(elems, sizeof(length_t));
#ifdef VERBOSE_MODE
  printf("%d-array allocated...\n", elems);
#endif
  length_t range_len = get_range(min_len, max_len);
  init_srand();
  for (size_t i = 0; i < elems; i++) {
    lengths[i] = min_len + (rand() % range_len);
  }
#ifdef VERBOSE_MODE
  printf("%d random lengths generated...\n", elems);
#endif
  return lengths;
}


void ** allocate_mep_contents(long long total_sources, size_t mep_factor, float margin_factor, size_t element_size) {
  if (margin_factor < 1) {
    fprintf(stderr, "Invalid value of margin!\n");
    exit(EXIT_FAILURE);
  }

#ifdef VERBOSE_MODE
  unsigned long long total_alloc_mem = 0;
#endif
  void **sources = (void **) try_calloc(total_sources, sizeof(void *));
  for (long long i = 0; i < total_sources; ++i) {
    sources[i] = (void *) try_calloc(margin_factor * mep_factor, element_size);
#ifdef VERBOSE_MODE
    total_alloc_mem += margin_factor * mep_factor * element_size;
#endif
  }
#ifdef VERBOSE_MODE
  printf("Total allocated memory for MEP contents: %llu\n", total_alloc_mem);
#endif
  return sources;
}


void deallocate_mep_contents(void **mep_contents, long long total_sources) {
  for (long long i = 0; i < total_sources; ++i) {
    free(mep_contents[i]);
  }
  free(mep_contents);
}


void fill_sources_with_random_lengths(length_t *sources, long long
    total_sources, size_t mep_factor, length_t min_len, length_t max_len) {

  length_t range_len = get_range(min_len, max_len);
  init_srand();
  for (long long si = 0; si < total_sources; si++) {
    for (long long mi = 0; mi < mep_factor; mi++) {
      // Segmentation fault can occur here, let system handle such scenario...
      sources[si * mep_factor + mi] = min_len + (rand() % range_len);
    }
  }
}


// modify_lengths_randomly() modifies only the 1st elements of sources with
// random number, which will affect all the prefix sums
void modify_lengths_randomly(length_t *sources, long long total_sources, size_t
    mep_factor, length_t min_len, length_t max_len) {

  length_t range_len = get_range(min_len, max_len);
  init_srand();

  for (long long si = 0; si < total_sources; si++) {
    sources[si * mep_factor] = min_len + (rand() % range_len);
  }
}


void copy_MEPs_serial_version(void **mep_contents, offset_t *read_offsets, void *sorted_events, offset_t *write_offsets, long long total_sources, size_t mep_factor, length_t *sources) {
  for (long long si = 0; si < total_sources; si++) {
    for (size_t mi = 0; mi < mep_factor; mi++) {
      memcpy(sorted_events + write_offsets[mi * total_sources + si],
          mep_contents[si] + read_offsets[si * mep_factor + mi],
          sources[si * mep_factor + mi]);
    }
  }
}


void copy_MEPs_OMP_version(void **mep_contents, offset_t *read_offsets, void *sorted_events, offset_t *write_offsets, long long total_sources, size_t mep_factor, length_t *sources) {
#pragma omp parallel for
  for (long long i = 0; i < total_sources * mep_factor; i++) {
    long long si = i / mep_factor;
    long long mi = i % mep_factor;
    memcpy(sorted_events + write_offsets[mi * total_sources + si],
        mep_contents[si] + read_offsets[si * mep_factor + mi],
        sources[si * mep_factor + mi]);
  }
}
