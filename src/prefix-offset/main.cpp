 
/* ========================================

   * File Name : main.cpp

   * Creation Date : 12-08-2015

   * Last Modified : Thu 03 Sep 2015 05:13:16 PM CEST

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
  int mic_num = 0;
  long long elements = 10000;
  long long iterations = 20000;

  while ((opt = getopt(argc, argv, "i:e:h")) != -1) {
    switch (opt) {
      case 'i':
        iterations = get_argument_long_value(optarg, "-i");
        break;
      case 'e':
        elements = get_argument_long_value(optarg, "-e");
        break;
      case 'h':
        printf("Usage: %s ", argv[0]);
        printf("[-i number_of_iterations] [-e number_of_array_elements]\n");
        exit(EXIT_SUCCESS);
      default:
        fprintf(stderr, "Usage: %s ", argv[0]);
        fprintf(stderr, "[-i number_of_iterations] [-e number_of_array_elements]\n");
        exit(EXIT_FAILURE);
    }
  }
  /* ------------------------------------------------------------------------ */
  length_t *lengths = generate_random_lengths(elements, 80, 150);

  tbb::tick_count start, end;
  /* ------------------------------------------------------------------------ */
  /* PREFIX OFFSETS FOR READING: EXCLUSIVE SCAN */
  offset_t *read_offsets = (offset_t *) calloc(elements, sizeof(offset_t));

  start = tbb::tick_count::now();
  for (long long i = 0; i < iterations; i++) {
    prefix_sum_sequential<length_t, offset_t>(lengths, read_offsets, elements, 0);
  }
  end = tbb::tick_count::now();

#ifdef VERBOSE_MODE
  printf("Generated random lengths:\n");
  show_lengths(lengths, elements);
  printf("Offsets:\n");
  show_offsets(read_offsets, elements);
#endif
  /* ------------------------------------------------------------------------ */

  free(read_offsets);
  free(lengths);

  const long long total_elements = elements * iterations;
  printf("Total elements: %llu\n", total_elements);

  tbb::tick_count::interval_t total_time = end - start;
  printf("Total time: %g secs\n", total_time.seconds());
  printf("Processed: %g elements per second\n", total_elements / total_time.seconds());

  const unsigned long long bytes_in_gb = 1000000000;
  const unsigned long long total_size = total_elements * sizeof(length_t) / bytes_in_gb;
  printf("Processed: %g GBps\n", total_size / total_time.seconds());

  return 0;
}
