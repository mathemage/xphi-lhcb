
/* ========================================

 * File Name : main.cpp

 * Creation Date : 25-08-2015

 * Last Modified : Fri 11 Mar 2016 11:15:08 AM CET

 * Created By : Karel Ha <mathemage@gmail.com>

 ==========================================*/

#include "commons.h"
#include "utils.h"
#include "prefix-sum.h"
#include <cmath>
#include <getopt.h>

// #define VERBOSE_MODE
// #define TEST_COPY_MEP_FUNCTION
#define SHOW_STATISTICS

/* 0 = no parallelization
 * 1 = OpenMP parallel for */
#define READ_OFFSETS_PARALLEL_LEVEL 1

/* 0 = no parallelization
 * 1 = OpenMP parallel for */
#define WRITE_OFFSETS_PARALLEL_LEVEL 1

/* 0 = no parallelization
 * 1 = OpenMP parallel for
 * 2 = OpenMP parallel for + block scheme */
#define COPY_PARALLEL_LEVEL 2


// default values from configuration for LHCb Upgrade 2
size_t mep_factor = 10000;
long long iterations = 10;
long long total_sources = 1000;
length_t min_length = 80;
length_t max_length = 150;
float margin_factor = 1.5;
tbb::tick_count tick, tock;
tbb::tick_count::interval_t total_time, read_offset_time, write_offset_time, copy_time;
double total_size = 0;
int compute_threads = 0;
int copy_threads = 0;
int s_block_size = 1, m_block_size = 1;
unsigned int srand_seed;
bool use_srand_seed = false;
bool log_progress = false;
bool quiet_mode = false;
FILE *outstream = stdout;

const unsigned long long bytes_in_gb = 1000000000;

#ifdef SHOW_STATISTICS
vector<double> iteration_times;
vector<double> iteration_throughputs;
#endif

double stopwatch_an_iteration(length_t *sources, offset_t *read_offsets, offset_t *write_offsets, void **mep_contents, void *sorted_events, bool is_benchmarked) {
  double iteration_time = 0;
  modify_lengths_randomly(sources, total_sources, mep_factor, min_length, max_length);
  log_msg("Lengths randomly modified");
#ifdef VERBOSE_MODE
  fprintf(outstream, "\nGenerated lengths of MEPs:\n");
  for (long long si = 0; si < total_sources; si++) {
    fprintf(outstream, "Source #%lld:\n", si);
    show_lengths(&sources[si * mep_factor], mep_factor);
    fprintf(outstream, "\n");
  }
#endif

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  /* PREFIX OFFSETS FOR READING: EXCLUSIVE SCAN */
  tick = tbb::tick_count::now();
#if READ_OFFSETS_PARALLEL_LEVEL == 0
  get_read_offsets_serial_version(sources, read_offsets, total_sources, mep_factor);
#elif READ_OFFSETS_PARALLEL_LEVEL == 1
  get_read_offsets_OMP_version(sources, read_offsets, total_sources, mep_factor, compute_threads);
#endif
#ifdef VERBOSE_MODE
  fprintf(outstream, "\nAll read_offsets:\n");
  show_offsets(read_offsets, mep_factor * total_sources);
#endif
  tock = tbb::tick_count::now();
  log_msg("Read offsets computed");
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  if (is_benchmarked) {
    total_time += (tock - tick);
    read_offset_time += (tock - tick);
  }
#ifdef SHOW_STATISTICS
  iteration_time += (tock - tick).seconds();
#endif

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  /* PREFIX OFFSETS FOR WRITING: EXCLUSIVE SCAN */
  tick = tbb::tick_count::now();
#if WRITE_OFFSETS_PARALLEL_LEVEL == 0
  get_write_offsets_serial_version(sources, write_offsets, total_sources, mep_factor);
#elif WRITE_OFFSETS_PARALLEL_LEVEL == 1
  get_write_offsets_OMP_version(sources, write_offsets, total_sources, mep_factor, read_offsets, compute_threads);
#endif
#ifdef VERBOSE_MODE
  fprintf(outstream, "\nAll write_offsets:\n");
  show_offsets(write_offsets, mep_factor * total_sources);
#endif
  tock = tbb::tick_count::now();
  log_msg("Write offsets computed");
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  if (is_benchmarked) {
    total_time += (tock - tick);
    write_offset_time += (tock - tick);
  }
#ifdef SHOW_STATISTICS
  iteration_time += (tock - tick).seconds();
#endif

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
#endif

  tick = tbb::tick_count::now();
#if COPY_PARALLEL_LEVEL == 0
  copy_MEPs_serial_version(mep_contents, read_offsets, sorted_events, write_offsets, total_sources, mep_factor, sources);
#elif COPY_PARALLEL_LEVEL == 1
  copy_MEPs_OMP_version(mep_contents, read_offsets, sorted_events, write_offsets, total_sources, mep_factor, sources, copy_threads);
#elif COPY_PARALLEL_LEVEL == 2
  copy_MEPs_block_scheme(mep_contents, read_offsets, sorted_events, write_offsets, total_sources, mep_factor, sources, s_block_size, m_block_size, copy_threads);
#endif
  tock = tbb::tick_count::now();
  log_msg("MEP fragments copied");
  if (is_benchmarked) {
    total_time += (tock - tick);
    copy_time += (tock - tick);
  }
#ifdef SHOW_STATISTICS
  iteration_time += (tock - tick).seconds();
#endif

#ifdef TEST_COPY_MEP_FUNCTION
  fprintf(outstream, "\n----------Input MEP contents----------\n");
  for (long long si = 0; si < total_sources; si++) {
    fprintf(outstream, "Source #%lld\t", si);
    local_offset = 0;
    for (size_t mi = 0; mi < mep_factor; mi++) {
      for (length_t li = 0; li < sources[si * mep_factor + mi]; li++) {
        fprintf(outstream, "%x", mep_contents_byte[si][local_offset]);
        local_offset++;
      }
    }
    fprintf(outstream, "\n");
  }
  fprintf(outstream, "--------------------------------------\n");
  fprintf(outstream, "----------Output MEP contents---------\n");
  local_offset = 0;
  uint8_t *sorted_events_byte = (uint8_t *) sorted_events;
  for (size_t mi = 0; mi < mep_factor; mi++) {
    fprintf(outstream, "Collision #%d\t", mi);
    for (long long si = 0; si < total_sources; si++) {
      for (length_t li = 0; li < sources[si * mep_factor + mi]; li++) {
        fprintf(outstream, "%x", sorted_events_byte[local_offset]);
        local_offset++;
      }
    }
    fprintf(outstream, "\n");
  }
  fprintf(outstream, "--------------------------------------\n");
#endif
  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  if (is_benchmarked) {
    double iteration_size = write_offsets[total_sources * mep_factor - 1] + sources[total_sources * mep_factor - 1];
    total_size += iteration_size;
    iteration_throughputs.emplace_back(iteration_size / iteration_time / bytes_in_gb);
    log_msg("New throughput calculated and stored");
  }

  return iteration_time;
}

#define no_argument 0
#define required_argument 1 
#define optional_argument 2

int main(int argc, char *argv[]) {
  /* ------------------------------------------------------------------------ */
  /* PARSING ARGUMENTS */
  int opt;

  const string help_msg = "Usage: %s [args]\n"
                          "\n"
                          " -i, --iterations \t number of iterations\n"
                          " -m, --mep-factor \t MEP factor\n"
                          " -s, --sources \t\t number of sources\n"
                          " -n, --min-length \t minimum length\n"
                          " -x, --max-length \t maximum length\n"
                          " --compute-threads \t number of threads for computing offsets\n"
                          " --copy-threads \t number of threads for copying MEP fragments\n"
                          " -1, --s-block \t\t sources per_block (for blockscheme memcpy)\n"
                          " -2, --m-block \t\t MEP fragments per_block (for blockscheme memcpy)\n"
                          " --srand-seed \t\t use custom srand() seed\n"
                          " -p, --log-progress \t log progress\n"
                          " -q, --quiet \t\t quiet mode\n"
                          " -e, --stderr \t\t redirect output to stderr\n"
                          " -h, --help \t\t show list of command-line options\n"
                          "\n";

  // test with:
  //    ./event-sort.exe --iterations 1 --mep-factor 1 --sources 1 --min-length 4 --max-length 5 --threads 42 --s-block 5 --m-block 6 --log-progress --stderr >/dev/null
  //    ./event-sort.exe --help
  const struct option longopts[] = {
    {"iterations", required_argument, 0, 'i'},
    {"mep-factor", required_argument, 0, 'm'},
    {"sources", required_argument, 0, 's'},
    {"min-length", required_argument, 0, 'n'},
    {"max-length", required_argument, 0, 'x'},
    {"compute-threads", required_argument, 0, 0},
    {"copy-threads", required_argument, 0, 0},
    {"s-block", required_argument, 0, '1'},
    {"m-block", required_argument, 0, '2'},
    {"srand-seed", required_argument, 0, 0},
    {"log-progress", no_argument, 0, 'p'},
    {"quiet", no_argument, 0, 'q'},
    {"stderr", no_argument, 0, 'e'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  while ((opt = getopt_long(argc, argv, "1:2:t:i:m:s:x:n:hqpe", longopts, &option_index)) != -1) {
    switch (opt) {
      case 0:
        if (longopts[option_index].flag == 0) {
          if (strcmp(longopts[option_index].name, "compute-threads") == 0) {
            compute_threads = get_argument_int_value(optarg, "--compute-threads");
          }
          if (strcmp(longopts[option_index].name, "copy-threads") == 0) {
            copy_threads = get_argument_int_value(optarg, "--copy-threads");
          }
          if (strcmp(longopts[option_index].name, "srand-seed") == 0) {
            srand_seed = get_argument_unsigned_int_value(optarg, "--srand-seed");
            use_srand_seed = true;
            init_srand();
          }
        }
        break;
      case 'i':
        iterations = get_argument_long_value(optarg, "-i");
        if (iterations < 1) {
          fprintf(stderr, "Invalid number of iterations: enter positive integer!\n");
          exit(EXIT_FAILURE);
        }
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
      case '1':
        s_block_size = get_argument_int_value(optarg, "-1");
        break;
      case '2':
        m_block_size = get_argument_int_value(optarg, "-2");
        break;
      case 'p':
        log_progress = true;
        break;
      case 'q':
        quiet_mode = true;
        break;
      case 'e':
        outstream = stderr;
        break;
      case 'h':
        fprintf(outstream, help_msg.c_str(), argv[0]);
        exit(EXIT_SUCCESS);
      default:
        fprintf(stderr, help_msg.c_str(), argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  if (use_srand_seed) {
    log_msg("srand() was initialized with seed " + to_string((long long) srand_seed));
  }
  if (compute_threads > 0) {
    log_msg("The program will use " + to_string((long long) compute_threads) + " compute-threads");
  }
  if (copy_threads > 0) {
    log_msg("The program will use " + to_string((long long) copy_threads) + " copy-threads");
  }
  log_msg("Command-line arguments parsed");
  /* ------------------------------------------------------------------------ */

  /* ------------------------------------------------------------------------ */
  /* TRANSPOSE OF MEPs */
  length_t *sources = (length_t *) try_calloc(total_sources * mep_factor, sizeof(length_t));
  fill_sources_with_random_lengths(sources, total_sources, mep_factor, min_length, max_length);
  log_msg("Lengths of MEP fragments generated");

  offset_t *read_offsets = (offset_t *) try_calloc(total_sources * mep_factor, sizeof(offset_t));
  log_msg("Array of read offsets allocated");
  offset_t *write_offsets = (offset_t *) try_calloc(total_sources * mep_factor, sizeof(offset_t));
  log_msg("Array of write offsets allocated");

  size_t mep_element_size = (min_length+max_length) / 2;
  void **mep_contents = allocate_mep_contents(total_sources, mep_factor, margin_factor, mep_element_size);
  log_msg("MEP fragments allocated");

  void *sorted_events = try_malloc((size_t) ceil(total_sources * mep_factor * margin_factor * mep_element_size));
  log_msg("Array for re-organized MEP fragments allocated");
#ifdef SHOW_STATISTICS
  iteration_times.assign(iterations, 0);
  iteration_throughputs.clear();
#endif

  // initial iteration won't be included in the benchmarks
  log_msg("Initial iteration started");
  double initial_time = stopwatch_an_iteration(sources, read_offsets, write_offsets, mep_contents, sorted_events, false);
  log_msg("Initial iteration finished");
  for (long long i = 0; i < iterations; i++) {
#ifdef VERBOSE_MODE
    fprintf(outstream, "\n______________________________________________________________\n");
    fprintf(outstream, "Iteration #%d\n", i+1);
#endif
    log_msg("Iteration #" + to_string(i) + " started");
    iteration_times[i] = stopwatch_an_iteration(sources, read_offsets, write_offsets, mep_contents, sorted_events, true);
    log_msg("Iteration #" + to_string(i) + " finished");
  }

  free(sources);
  log_msg("Array of lengths deallocated");
  free(read_offsets);
  log_msg("Array of read offsets deallocated");
  free(write_offsets);
  log_msg("Array of write offsets deallocated");
  deallocate_mep_contents(mep_contents, total_sources);
  log_msg("MEP fragments deallocated");
  free(sorted_events);
  log_msg("Array for re-organized MEP fragments deallocated");
  /* ------------------------------------------------------------------------ */

  total_size /= bytes_in_gb;
  if (quiet_mode) {
    fprintf(outstream, "%g\t", total_size / total_time.seconds());
    return 0;
  }

#ifdef SHOW_STATISTICS
  fprintf(outstream, "\n____________STATISTICS OF TIME INTERVALS (in secs)____________\n");
  fprintf(outstream, "The initial iteration: %.5f\n", initial_time);
  create_histogram(iteration_times);
  fprintf(outstream, "______________________________________________________________\n");

  fprintf(outstream, "\n______________STATISTICS OF THROUGHPUTS (in GBps)______________\n");
  create_histogram(iteration_throughputs);
  fprintf(outstream, "_______________________________________________________________\n");
#endif

  fprintf(outstream, "\n____________________________SUMMARY____________________________\n");
  const double total_elements = mep_factor * total_sources * iterations;
  fprintf(outstream, "Total elements: %g\n", total_elements);
  fprintf(outstream, "Time for computing read_offsets: %g secs\n", read_offset_time.seconds());
  fprintf(outstream, "Time for computing write_offsets: %g secs\n", write_offset_time.seconds());
  fprintf(outstream, "Time for copying: %g secs\n", copy_time.seconds());
  fprintf(outstream, "Total time: %g secs\n", total_time.seconds());
  fprintf(outstream, "Total size: %g GB\n", total_size);
  fprintf(outstream, "Processed: %g elements per second\n", total_elements / total_time.seconds());
  fprintf(outstream, "Throughput: %g GBps\n", total_size / total_time.seconds());
  fprintf(outstream, "_______________________________________________________________\n");
  return 0;
}
