 
/* ========================================

   * File Name : utils.h

   * Creation Date : 13-08-2015

   * Last Modified : Mon 24 Aug 2015 03:43:07 PM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#ifndef __UTILS_H__
#define __UTILS_H__

#include "commons.h"
#include <ctime>
#include <string>

long long get_argument_long_value(char *optarg, char *optname);

template<typename T>
void show_array(T *arr, size_t elems, const int fixed_width, const size_t show_limit, const size_t elems_on_line);
void show_lengths(length_t *lengths, size_t elems);
void show_offsets(offset_t *offsets, size_t elems);

length_t * generate_random_lengths(size_t elems, length_t min_len, length_t max_len);

#endif
