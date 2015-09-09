
/* ========================================

 * File Name : main.cpp

 * Creation Date : 25-08-2015

 * Last Modified : Wed 09 Sep 2015 03:03:18 PM CEST

 * Created By : Karel Ha <mathemage@gmail.com>

 ==========================================*/

#include "commons.h"
#include "utils.h"
#include "prefix-sum.h"
#include <cmath>

// #define VERBOSE_MODE
// #define TEST_COPY_MEP_FUNCTION

/* 0 = no parallelization
 * 1 = OpenMP parallel for*/
#define COPY_PARALLEL_LEVEL 1

int main(int argc, char *argv[]) {
  /* ------------------------------------------------------------------------ */
  /* PARSING ARGUMENTS */
  int opt;

  // default values from configuration for LHCb Upgrade 2
  size_t mep_factor = 10000;
  long long iterations = 10;
  long long total_sources = 1000;
  length_t min_length = 80;
  length_t max_length = 150;

  while ((opt = getopt(argc, argv, "i:m:s:x:n:h")) != -1) {
    switch (opt) {
      case 'i':
        iterations = get_argument_long_value(optarg, "-i");
        break;
      case 'm':
        mep_factor = get_argument_long_value(optarg, "-m");
        break;
      case 's':
        total_sources = get_argument_long_value(optarg, "-s");
        break;
      case 'n':
        min_length = get_argument_long_value(optarg, "-n");
        break;
      case 'x':
        max_length = get_argument_long_value(optarg, "-x");
        break;
      case 'h':
        printf("Usage: %s", argv[0]);
        printf(" [-i number_of_iterations] [-m mep_factor]");
        printf(" [-s number_of_sources]");
        printf(" [-n min_length] [-x max_length]");
        printf("\n");
        exit(EXIT_SUCCESS);
      default:
        fprintf(stderr, "Usage: %s", argv[0]);
        fprintf(stderr, " [-i number_of_iterations] [-m mep_factor]");
        fprintf(stderr, " [-s number_of_sources]");
        fprintf(stderr, " [-n min_length] [-x max_length]");
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
  }
  /* ------------------------------------------------------------------------ */

  /* ------------------------------------------------------------------------ */
  /* TRANSPOSE OF MEPs */
  length_t *sources = (length_t *) try_calloc(total_sources * mep_factor, sizeof(length_t));
  fill_sources_with_random_lengths(sources, total_sources, mep_factor, min_length, max_length);

  offset_t *read_offsets = (offset_t *) try_calloc(total_sources * mep_factor, sizeof(offset_t));
  offset_t *write_offsets = (offset_t *) try_calloc(total_sources * mep_factor, sizeof(offset_t));
  float margin_factor = 1.5;
  size_t mep_element_size = (min_length+max_length) / 2;
  void **mep_contents = allocate_mep_contents(total_sources, mep_factor, margin_factor, mep_element_size);
  void *sorted_events = try_malloc((size_t) ceil(total_sources * mep_factor * margin_factor * mep_element_size));
  
  tbb::tick_count start, end;
  tbb::tick_count::interval_t total_time, read_offset_time, write_offset_time, copy_time;

  for (long long i = 0; i < iterations; i++) {
    modify_lengths_randomly(sources, total_sources, mep_factor, min_length, max_length);
#ifdef VERBOSE_MODE
    printf("\n---------------------------\n");
    printf("Iteration #%d\n", i+1);
    printf("\nGenerated lengths of MEPs:\n");
    for (long long si = 0; si < total_sources; si++) {
      printf("Source #%lld:\n", si);
      show_lengths(&sources[si * mep_factor], mep_factor);
      printf("\n");
    }
#endif

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* PREFIX OFFSETS FOR READING: EXCLUSIVE SCAN */
    start = tbb::tick_count::now();
    get_read_offsets_serial_vesion(sources, read_offsets, total_sources, mep_factor);
#ifdef VERBOSE_MODE
    printf("\nAll read_offsets:\n");
    show_offsets(read_offsets, mep_factor * total_sources);
#endif
    end = tbb::tick_count::now();
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    total_time += (end - start);
    read_offset_time += (end - start);

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* PREFIX OFFSETS FOR WRITING: EXCLUSIVE SCAN */
    start = tbb::tick_count::now();
    get_write_offsets_serial_vesion(sources, write_offsets, total_sources, mep_factor);
#ifdef VERBOSE_MODE
    printf("\nAll write_offsets:\n");
    show_offsets(write_offsets, mep_factor * total_sources);
#endif
    end = tbb::tick_count::now();
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    total_time += (end - start);
    write_offset_time += (end - start);

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* MEPs' MEMCOPY */
#ifdef TEST_COPY_MEP_FUNCTION
    uint8_t **mep_contents_byte = (uint8_t **) mep_contents;
    uint8_t label = 1;
    offset_t local_offset;
    for (long long si = 0; si < total_sources; si++) {
      local_offset = 0;
      for (size_t mi = 0; mi < mep_factor; mi++) {
        for (length_t li = 0; li < sources[si * mep_factor + mi]; li++) {
          mep_contents_byte[si][local_offset] = label;
          local_offset++;
        }
        label++;
      }
    }

    printf("\n----------Input MEP contents----------\n");
    for (long long si = 0; si < total_sources; si++) {
      printf("[");
      local_offset = 0;
      for (size_t mi = 0; mi < mep_factor; mi++) {
        for (length_t li = 0; li < sources[si * mep_factor + mi]; li++) {
          printf("%x", mep_contents_byte[si][local_offset]);
          local_offset++;
        }
      }
      printf("]\n");
    }
    printf("--------------------------------------\n");
#endif

#ifdef VERBOSE_MODE
    printf("Copying MEP contents...\n");
#endif
    start = tbb::tick_count::now();
#if COPY_PARALLEL_LEVEL == 0
    copy_MEPs_serial_version(mep_contents, read_offsets, sorted_events, write_offsets, total_sources, mep_factor, sources);
#elif COPY_PARALLEL_LEVEL == 1
    copy_MEPs_OMP_version(mep_contents, read_offsets, sorted_events, write_offsets, total_sources, mep_factor, sources);
#endif
    end = tbb::tick_count::now();
    total_time += (end - start);
    copy_time += (end - start);
#ifdef VERBOSE_MODE
    printf("MEP contents copied...\n");
#endif

#ifdef TEST_COPY_MEP_FUNCTION
    printf("\n----------Output MEP contents----------\n");
    local_offset = 0;
    uint8_t *sorted_events_byte = (uint8_t *) sorted_events;
    for (size_t mi = 0; mi < mep_factor; mi++) {
      printf("[");
      for (long long si = 0; si < total_sources; si++) {
        for (length_t li = 0; li < sources[si * mep_factor + mi]; li++) {
          printf("%x", sorted_events_byte[local_offset]);
          local_offset++;
        }
      }
      printf("]\n");
    }
    printf("--------------------------------------\n");
#endif
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  }

  free(sources);
  free(read_offsets);
  free(write_offsets);
  deallocate_mep_contents(mep_contents, total_sources);
  free(sorted_events);
  /* ------------------------------------------------------------------------ */

  printf("\n----------SUMMARY----------\n");
  const double total_elements = mep_factor * total_sources * iterations;
  printf("Total elements: %g\n", total_elements);
  printf("Time for computing read_offsets: %g secs\n", read_offset_time.seconds());
  printf("Time for computing write_offsets: %g secs\n", write_offset_time.seconds());
  printf("Time for copying: %g secs\n", copy_time.seconds());
  printf("Total time: %g secs\n", total_time.seconds());
  const unsigned long long bytes_in_gb = 1000000000;
  const double total_size = total_elements * sizeof(length_t) / bytes_in_gb;
  printf("Total size: %g GB\n", total_size);
  printf("Processed: %g elements per second\n", total_elements / total_time.seconds());
  printf("Throughput: %g GBps\n", total_size / total_time.seconds());
  printf("---------------------------\n");
  return 0;
}
