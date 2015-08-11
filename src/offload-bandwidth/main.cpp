 
/* ========================================

   * File Name : main.cpp

   * Creation Date : 09-08-2015

   * Last Modified : Tue 11 Aug 2015 12:09:19 PM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include <cstdio>
#include <cstdlib>
#include "tbb/tick_count.h"

#define MIC_NUM 1
#define ALLOC alloc_if(1) free_if(0)
#define REUSE alloc_if(0) free_if(0)
#define FREE alloc_if(0) free_if(1)
//#define VERBOSE_MODE

typedef char data_type;

using namespace std;

int main() {
  tbb::tick_count start, end;
  tbb::tick_count::interval_t total_time;
  char mic_message[100];

  // offload 40 times 5 GB = 200 GB
  const long long iterations = 40;
  const long long elements = 5000000000;

  // initial allocation on MIC
  data_type *pData = (data_type*) calloc(elements, sizeof(data_type));
#pragma offload_transfer target(mic:MIC_NUM) nocopy(pData:length(elements) ALLOC)

  for (long long i = 0; i < iterations; i++) {
#ifdef VERBOSE_MODE
    printf("Iteration #%d\n", i);
    pData[0] = 'a' + (i%26);          // for checking, if the data were actually copied
    printf("pData@host: %p\t\t pData[0]@host == %c\n", pData, pData[0]);
#endif

    start = tbb::tick_count::now();
#pragma offload target(mic:MIC_NUM) in(pData:length(elements) REUSE)
    {
#ifdef VERBOSE_MODE
      sprintf(mic_message, "pData@MIC%d: %p\t\t pData[0]@MIC%d == %c\n", MIC_NUM,
          pData, MIC_NUM, pData[0]);
#endif
    }
    end = tbb::tick_count::now();
    total_time += (end - start);

#ifdef VERBOSE_MODE
    printf("%s\n", mic_message);
#endif
  }

  // final deallocation on MIC
#pragma offload_transfer target(mic:MIC_NUM) nocopy(pData:length(elements) FREE)

  free(pData);

  const unsigned long long bytes_in_gb = 1000000000;
  const unsigned long long total_size = elements / bytes_in_gb * iterations * sizeof(data_type) ;
  printf("Transfered: %llu GB\n", total_size);
  printf("Total time: %g secs\n", total_time.seconds());
  printf("Bandwidth: %g GBps\n", total_size / total_time.seconds());
  return 0;
}
