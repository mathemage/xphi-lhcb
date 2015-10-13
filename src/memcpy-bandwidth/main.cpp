
/* ========================================

 * File Name : main.cpp

 * Creation Date : 25-08-2015

 * Last Modified : Tue 13 Oct 2015 11:40:39 AM CEST

 * Created By : Karel Ha <mathemage@gmail.com>

 ==========================================*/

#include "commons.h"
#include "utils.h"
#include <cassert>

// default values from configuration for LHCb Upgrade 2
long long number_of_iterations = 1000;
long long chunk_size = 2000000;
long long number_of_chunks = 500;
tbb::tick_count tick, tock;
void *source, *destination;
bool verbose_enabled = false;

vector<double> iteration_times;

// returns the duration of one iteration (in seconds)
double stopwatch_an_iteration() {
#pragma omp parallel

#pragma omp master
  if (verbose_enabled) {
    printf("Starting iteration with %d threads\r\n", omp_get_num_threads());
  }

#pragma omp master
  tick = tbb::tick_count::now();
#pragma omp parallel for
  for (long long ni = 0; ni < number_of_chunks; ni++) {
    memcpy(destination + ni * chunk_size, source + ni * chunk_size, chunk_size);
  }
  tock = tbb::tick_count::now();
  return (tock-tick).seconds();
}


int main(int argc, char *argv[]) {
  /* ------------------------------------------------------------------------ */
  /* PARSING ARGUMENTS */
  int opt;

  while ((opt = getopt(argc, argv, ":vi:c:n::h")) != -1) {
    switch (opt) {
      case 'i':
        number_of_iterations = get_argument_long_value(optarg, "-i");
        if (number_of_iterations < 1) {
          fprintf(stderr, "Invalid number of iterations: enter positive integer!\n");
          exit(EXIT_FAILURE);
        }
        break;
      case 'c':
        chunk_size = get_argument_long_value(optarg, "-c");
        break;
      case 'n':
        number_of_chunks = get_argument_long_value(optarg, "-n");
        break;
      case 'v':
        printf("Verbose mode enabled!");
        verbose_enabled = true;
        break;
      case 'h':
        printf("Usage: %s", argv[0]);
        printf(" [-i number_of_iterations] [-c chunk_size in bytes] [-n number_of_chunks] [-v enable_verbose_mode]");
        printf("\n");
        exit(EXIT_SUCCESS);
      default:
        fprintf(stderr, "Usage: %s", argv[0]);
        fprintf(stderr, " [-i number_of_iterations] [-c chunk_size in bytes] [-n number_of_chunks] [-v enable_verbose_mode]");
        fprintf(stderr, "\n");
        exit(EXIT_FAILURE);
    }
  }
  /* ------------------------------------------------------------------------ */

  bool checkable = number_of_iterations * number_of_chunks * chunk_size < 1000000000;

  iteration_times.assign(number_of_iterations, 0);
  double total_time = 0;

  source = try_calloc(number_of_chunks, chunk_size);
  destination = try_calloc(number_of_chunks, chunk_size);

  // initial iteration won't be included in the benchmarks
  double initial_time = stopwatch_an_iteration();
  for (long long i = 0; i < number_of_iterations; i++) {
    if (verbose_enabled) {
      printf("Iteration #%d: ", i+1);
    }
    total_time += (iteration_times[i] = stopwatch_an_iteration());
    if (verbose_enabled && checkable) {
      assert(0 == memcmp(source, destination, number_of_chunks * chunk_size));
      printf("Iteration #%d has been verified successfully...\n", i+1);
    }
  }

  printf("\n--------STATISTICS OF TIME INTERVALS--------\n");
  printf("The initial iteration: %.5f secs\n", initial_time);
  time_statistics(iteration_times);
  printf("--------------------------------------------");

  printf("\n----------SUMMARY----------\n");
  printf("Total time: %g secs\n", total_time);

  const unsigned long long bytes_in_gb = 1000000000;
  const double total_elements = number_of_iterations * number_of_chunks * chunk_size;
  const double total_size = total_elements / bytes_in_gb;
  printf("Total size: %g GB\n", total_size);

  printf("Throughput: %g GBps\n", total_size / total_time);
  printf("---------------------------\n");
  return 0;
}
