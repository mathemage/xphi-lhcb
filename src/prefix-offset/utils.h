 
/* ========================================

   * File Name : utils.h

   * Creation Date : 13-08-2015

   * Last Modified : Thu 13 Aug 2015 02:36:41 PM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#ifndef __UTILS_H__
#define __UTILS_H__

#include "commons.h"
#include <ctime>

template<typename T>
void show_array(T *arr, size_t elems, const int fixed_width, const size_t show_limit, const size_t elems_on_line);
void show_lengths(length_t *lengths, size_t elems);
void show_offsets(offset_t *offsets, size_t elems);

length_t * generate_random_lengths(size_t elems, length_t min_len, length_t max_len);

#endif
