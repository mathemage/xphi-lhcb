 
/* ========================================

   * File Name : prefix-sum.h

   * Creation Date : 13-08-2015

   * Last Modified : Wed 09 Sep 2015 07:33:19 PM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#ifndef __PREFIX_SUM__
#define __PREFIX_SUM__

#include "commons.h"

template<typename IN_T, typename SUM_T>
void prefix_sum_sequential(IN_T *in_arr, SUM_T *sum_arr, size_t elems, SUM_T identity_element) {
  sum_arr[0] = identity_element;
  for (size_t i = 1; i < elems; i++) {
    sum_arr[i] = sum_arr[i-1] + in_arr[i-1];
  }
}

void get_read_offsets_serial_vesion(length_t *sources, offset_t *read_offsets, long long total_sources, size_t mep_factor);
void get_read_offsets_OMP_version(length_t *sources, offset_t *read_offsets, long long total_sources, size_t mep_factor);
void get_write_offsets_serial_vesion(length_t *sources, offset_t *read_offsets, long long total_sources, size_t mep_factor);

#endif
