 
/* ========================================

   * File Name : utils.cpp

   * Creation Date : 13-08-2015

   * Last Modified : Tue 16 Feb 2016 03:12:59 PM CET

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include "utils.h"
#include "commons.h"

void log_msg(string msg) {
  if (log_progress) {
    msg = "Event-sort log: <" + msg + ">\n";
    fprintf(outstream, msg.c_str());
  }
}


long long get_argument_long_value(char *optarg, char *optname) {
  char* endp = NULL;
  long value = -1;
  if (!optarg ||  ((value=strtol(optarg, &endp, 10)),(endp && *endp)) ) {
    fprintf(stderr, "Invalid '%s' option: '%s' ... expecting a number\n", optname, optarg ? optarg : "");
    exit(EXIT_FAILURE);
  };
  return value;
}


int get_argument_int_value(char *optarg, char *optname) {
  char* endp = NULL;
  int value = -1;
  if (!optarg ||  ((value=strtol(optarg, &endp, 10)),(endp && *endp)) ) {
    fprintf(stderr, "Invalid '%s' option: '%s' ... expecting a number\n", optname, optarg ? optarg : "");
    exit(EXIT_FAILURE);
  };
  return value;
}


unsigned int get_argument_unsigned_int_value(char *optarg, char *optname) {
  int value = get_argument_int_value(optarg, optname);
  if (value < 0) {
    fprintf(stderr, "Invalid '%s' option: '%s' ... expecting a non-negative number\n", optname, optarg ? optarg : "");
    exit(EXIT_FAILURE);
  }
  return value;
}


template<typename T>
void show_array(T *arr, size_t elems, const int fixed_width, const size_t
    show_limit, const size_t elems_on_line) {
  if (elems > show_limit) {
    fprintf(outstream, "Too many numbers to display!\n");
    return;
  }

  for (size_t i = 0; i < elems; i++) {
    fprintf(outstream, "%*d", fixed_width, arr[i]);
    if (i % elems_on_line == elems_on_line - 1) {
      fprintf(outstream, "\n");
    } else {
      fprintf(outstream, "\t");
    }
  }
  fprintf(outstream, "\n");
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
    unsigned int seed = use_srand_seed ? srand_seed : time(NULL);
    srand(seed);
#ifdef VERBOSE_MODE
    fprintf(outstream, "srand(%d) called...\n", seed);
#endif
    srand_for_first_time = false;
  }
}


void * try_malloc(size_t size) {
  void *chunk = malloc(size);
  if (chunk == NULL) {
    fprintf(outstream, "Exitting: malloc failed!\n");
    exit(EXIT_FAILURE);
  }
  return chunk;
}


void * try_calloc(size_t num, size_t size) {
  void *chunk = calloc(num, size);
  if (chunk == NULL) {
    fprintf(outstream, "Exitting: calloc failed!\n");
    exit(EXIT_FAILURE);
  }
  return chunk;
}


double get_mean_value(const vector<double> & values) {
  return accumulate(values.begin(), values.end(), 0.0) / values.size();
}


void create_histogram(const vector<double> & data_points) {
  if (data_points.empty()) {
    fprintf(outstream, "No data points provided!\n");
    return;
  }

  double bottom = *min_element(data_points.begin(), data_points.end());
  double top = *max_element(data_points.begin(), data_points.end());
  int total_bins = ceil(sqrt(data_points.size()));
  vector<int> bins(total_bins, 0);
  double bin_width = (top - bottom) / total_bins;

  fprintf(outstream, "min: %.5f\n", bottom);
  fprintf(outstream, "max: %.5f\n", top);
  fprintf(outstream, "mean: %.5f\n", get_mean_value(data_points));
  fprintf(outstream, "Histogram:\n");
  int bin_index;
  for (auto & val : data_points) {
    bin_index = (val - bottom) / bin_width;
    bin_index = max(bin_index, 0);
    bin_index = min(bin_index, total_bins - 1);
    bins[bin_index]++;
  }
  for (int i = 0; i < total_bins; ++i) {
    fprintf(outstream, "[%.5f, %.5f): %d times\n", bottom + i * bin_width, bottom + (i+1) * bin_width, bins[i]);
  }
}


length_t * generate_random_lengths(size_t elems, length_t min_len, length_t max_len) {
  length_t *lengths = (length_t *) calloc(elems, sizeof(length_t));
#ifdef VERBOSE_MODE
  fprintf(outstream, "%d-array allocated...\n", elems);
#endif
  length_t range_len = get_range(min_len, max_len);
  init_srand();
  for (size_t i = 0; i < elems; i++) {
    lengths[i] = min_len + (rand() % range_len);
  }
#ifdef VERBOSE_MODE
  fprintf(outstream, "%d random lengths generated...\n", elems);
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
  fprintf(outstream, "Total allocated memory for MEP contents: %llu\n", total_alloc_mem);
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
  log_msg("Copying MEP fragments for " + to_string(total_sources)
      + " source(s) and " + to_string((long long) mep_factor) + " MEP fragment(s) per source");
  for (long long si = 0; si < total_sources; si++) {
    for (size_t mi = 0; mi < mep_factor; mi++) {
      memcpy(sorted_events + write_offsets[mi * total_sources + si],
          mep_contents[si] + read_offsets[si * mep_factor + mi],
          sources[si * mep_factor + mi]);
    }
  }
}


void copy_MEPs_OMP_version(void **mep_contents, offset_t *read_offsets, void *sorted_events, offset_t *write_offsets, long long total_sources, size_t mep_factor, length_t *sources, int nthreads) {
  log_msg("Copying MEP fragments for " + to_string(total_sources)
      + " source(s) and " + to_string((long long) mep_factor) + " MEP fragment(s) per source using OpenMP");
  if (nthreads > 0) {
#pragma omp parallel for num_threads(nthreads)
    for (long long i = 0; i < total_sources * mep_factor; i++) {
      long long si = i / mep_factor;
      long long mi = i % mep_factor;
      memcpy(sorted_events + write_offsets[mi * total_sources + si],
          mep_contents[si] + read_offsets[si * mep_factor + mi],
          sources[si * mep_factor + mi]);
    }
  } else {
#pragma omp parallel for
    for (long long i = 0; i < total_sources * mep_factor; i++) {
      long long si = i / mep_factor;
      long long mi = i % mep_factor;
      memcpy(sorted_events + write_offsets[mi * total_sources + si],
          mep_contents[si] + read_offsets[si * mep_factor + mi],
          sources[si * mep_factor + mi]);
    }
  }
}

void copy_MEPs_block_scheme(void **mep_contents, offset_t *read_offsets, void *sorted_events, offset_t *write_offsets, long long total_sources, size_t mep_factor, length_t *sources, int s_block_size, int m_block_size) {
  long long s_blocks = (total_sources + s_block_size - 1) / s_block_size;
  long long m_blocks = (mep_factor + m_block_size - 1) / m_block_size;

  log_msg("Copying MEP fragments for " + to_string(total_sources)
      + " source(s) and " + to_string((long long) mep_factor) + " MEP fragment(s) per source using blocks of "
      + to_string((long long) s_block_size) + "x" + to_string((long long) m_block_size) );
#pragma omp parallel for
  for (long long i = 0; i < s_blocks * m_blocks; i++) {
    long long s_block_start = i / m_blocks;
    long long m_block_start = i % m_blocks;
    for (long long si = s_block_start; si < s_block_start + s_block_size && si < total_sources; si++) {
      for (long long mi = m_block_start; mi < m_block_start + m_block_size && mi < mep_factor; mi++) {
        memcpy(sorted_events + write_offsets[mi * total_sources + si],
            mep_contents[si] + read_offsets[si * mep_factor + mi],
            sources[si * mep_factor + mi]);
      }
    }
  }
}
