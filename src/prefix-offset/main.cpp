 
/* ========================================

   * File Name : main.cpp

   * Creation Date : 12-08-2015

   * Last Modified : Fri 14 Aug 2015 02:07:32 PM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include "commons.h"
#include "utils.h"
#include "prefix-sum.h"

int main() {
  const size_t elems = 1000000000;
  length_t *lengths = generate_random_lengths(elems, 0, 65534);

  printf("\n");

  tbb::tick_count start, end;
  /* ------------------------------------------------------------------------ */
  /* PREFIX OFFSETS FOR READING: EXCLUSIVE SCAN */
  offset_t *read_offsets = (offset_t *) calloc(elems, sizeof(offset_t));
  start = tbb::tick_count::now();
  prefix_sum_sequential<length_t, offset_t>(lengths, read_offsets, elems, 0);
  end = tbb::tick_count::now();
  printf("Offsets:\n");
  show_offsets(read_offsets, elems);
  /* ------------------------------------------------------------------------ */

  free(read_offsets);
  free(lengths);

  tbb::tick_count::interval_t total_time = end - start;
  printf("Total time: %g secs\n", total_time.seconds());
  printf("Processed: %g elements per second\n", elems / total_time.seconds());
  const unsigned long long bytes_in_gb = 1000000000;
  const unsigned long long total_size = elems * sizeof(length_t) / bytes_in_gb;
  printf("Processed: %g GBps\n", total_size / total_time.seconds());
  return 0;
}
