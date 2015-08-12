 
/* ========================================

   * File Name : main.cpp

   * Creation Date : 09-08-2015

   * Last Modified : Wed 12 Aug 2015 04:36:05 PM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include <cstdio>
#include <cstdlib>
#include <string>
#include "tbb/tick_count.h"

#define ALLOC alloc_if(1) free_if(0)
#define REUSE alloc_if(0) free_if(0)
#define FREE alloc_if(0) free_if(1)
//#define VERBOSE_MODE

typedef char data_t;

using namespace std;

long long get_argument_long_value(char *optarg, char *optname) {
  char* endp = NULL;
  long value = -1;
  if (!optarg ||  ((value=strtol(optarg, &endp, 10)),(endp && *endp)) ) {
    fprintf(stderr, "Invalid '%s' option %s - expecting a number\n", optname,
        optarg ? optarg : "");
    exit(EXIT_FAILURE);
  };
  return value;
}

int main(int argc, char *argv[]) {
  /* ------------------------------------------------------------------------ */
  /* PARSING ARGUMENTS */
  int opt;
  int mic_num = 0;
  long long iterations = 40, elements = 5000000000;   // by default, offload 40
                                                      // times 5 GB = 200 GB

  while ((opt = getopt(argc, argv, "m:i:e:")) != -1) {
    switch (opt) {
      case 'm':
        mic_num = (int) get_argument_long_value(optarg, "-m");
        break;
      case 'i':
        iterations = get_argument_long_value(optarg, "-i");
        break;
      case 'e':
        elements = get_argument_long_value(optarg, "-e");
        break;
      default:
        fprintf(stderr, "Usage: %s [-m]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  printf("Using MIC%d...\n", mic_num);
  /* ------------------------------------------------------------------------ */

  tbb::tick_count start, end;
  tbb::tick_count::interval_t total_time;
#ifdef VERBOSE_MODE
  char mic_message[100];
#endif

  // initial allocation on MIC
  data_t *pData = (data_t*) calloc(elements, sizeof(data_t));
#pragma offload_transfer target(mic:mic_num) nocopy(pData:length(elements) ALLOC)

  /* ------------------------------------------------------------------------ */
  /* COPY DATA TO THE MIC */
  for (long long i = 0; i < iterations; i++) {
#ifdef VERBOSE_MODE
    printf("Iteration #%d\n", i);
    pData[0] = 'a' + (i%26);          // for checking, if the data were actually copied
    printf("pData@host: %p\t\t pData[0]@host == %c\n", pData, pData[0]);
#endif

    start = tbb::tick_count::now();
#pragma offload target(mic:mic_num) in(pData:length(elements) REUSE)
    {
#ifdef VERBOSE_MODE
      sprintf(mic_message, "pData@MIC%d: %p\t\t pData[0]@MIC%d == %c\n", mic_num,
          pData, mic_num, pData[0]);
#endif
    }
    end = tbb::tick_count::now();
    total_time += (end - start);

#ifdef VERBOSE_MODE
    printf("%s\n", mic_message);
#endif
  }
  /* ------------------------------------------------------------------------ */

  // final deallocation on MIC
#pragma offload_transfer target(mic:mic_num) nocopy(pData:length(elements) FREE) signal(&pData)

  free(pData);

  const unsigned long long bytes_in_gb = 1000000000;
  const unsigned long long total_size = elements / bytes_in_gb * iterations * sizeof(data_t) ;
  printf("Transferred: %llu GB\n", total_size);
  printf("Total time: %g secs\n", total_time.seconds());
  printf("Bandwidth: %g GBps\n", total_size / total_time.seconds());
  return 0;
}
