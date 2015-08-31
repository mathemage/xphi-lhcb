
/* ========================================

 * File Name : main.cpp

 * Creation Date : 25-08-2015

 * Last Modified : Mon 31 Aug 2015 12:11:38 PM CEST

 * Created By : Karel Ha <mathemage@gmail.com>

 ==========================================*/

#include "commons.h"
#include "utils.h"
#include "prefix-sum.h"

#define VERBOSE_MODE

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
      case 'x':
        max_length = get_argument_long_value(optarg, "-x");
        break;
      case 'n':
        min_length = get_argument_long_value(optarg, "-n");
        break;
      case 'h':
        printf("Usage: %s", argv[0]);
        printf(" [-i number_of_iterations] [-m mep_factor]");
        printf(" [-s number_of_sources]");
        printf(" [-x max_length] [-n min_length]");
        printf("\n");
        exit(EXIT_SUCCESS);
      default:
        fprintf(stderr, "Usage: %s", argv[0]);
        fprintf(stderr, " [-i number_of_iterations] [-m mep_factor]");
        fprintf(stderr, " [-s number_of_sources]");
        fprintf(stderr, " [-x max_length] [-n min_length]");
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
  }
  /* ------------------------------------------------------------------------ */

  /* ------------------------------------------------------------------------ */
  /* TRANSPOSE OF MEPs */
  length_t **sources = allocate_sources(total_sources, mep_factor);
  fill_sources_with_random_lengths(sources, total_sources, mep_factor, min_length, max_length);

  offset_t *read_offsets = (offset_t *) calloc(total_sources * mep_factor, sizeof(offset_t));
  // TODO write_offsets
  
  tbb::tick_count start, end;
  tbb::tick_count::interval_t total_time;

  for (long long i = 0; i < iterations; i++) {
#ifdef VERBOSE_MODE
    printf("\n---------------------------\n");
    printf("Iteration #%d\n", i+1);
#endif
    modify_lengths_randomly(sources, total_sources, min_length, max_length);

    start = tbb::tick_count::now();
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* PREFIX OFFSETS FOR READING: EXCLUSIVE SCAN */
    for (long long si = 0; si < total_sources; si++) {
      prefix_sum_sequential<length_t, offset_t>
        (sources[si], &read_offsets[si * mep_factor], mep_factor, 0);
#ifdef VERBOSE_MODE
      printf("\nGenerated lengths of MEPs:\n");
      show_lengths(sources[si], mep_factor);
      printf("\nOffsets:\n");
      show_offsets(&read_offsets[si * mep_factor], mep_factor);
#endif
    }

#ifdef VERBOSE_MODE
    printf("\nAll read_offsets:\n");
    show_offsets(read_offsets, mep_factor * total_sources);
#endif
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    /* MEPs' MEMCOPY */
    // TODO
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    end = tbb::tick_count::now();
    total_time += (end - start);
  }

  deallocate_sources(sources, total_sources);
  free(read_offsets);
  /* ------------------------------------------------------------------------ */

  printf("\n----------SUMMARY----------\n");
  const double total_elements = mep_factor * total_sources * iterations;
  printf("Total elements: %g\n", total_elements);
  printf("Total time: %g secs\n", total_time.seconds());
  const unsigned long long bytes_in_gb = 1000000000;
  const double total_size = total_elements * sizeof(length_t) / bytes_in_gb;
  printf("Total size: %g GB\n", total_size);
  printf("Processed (prefix sum): %g elements per second\n", total_elements /
      total_time.seconds());
  printf("Throughput: %g GBps\n", total_size / total_time.seconds());
  printf("---------------------------\n");
  return 0;
}
