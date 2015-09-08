 
/* ========================================

   * File Name : utils.h

   * Creation Date : 13-08-2015

   * Last Modified : Tue 08 Sep 2015 10:28:15 AM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#ifndef __UTILS_H__
#define __UTILS_H__

#include "commons.h"
#include <ctime>
#include <string>
#include <cstring>

// #define VERBOSE_MODE

long long get_argument_long_value(char *optarg, char *optname);

template<typename T>
void show_array(T *arr, size_t elems, const int fixed_width, const size_t
    show_limit, const size_t elems_on_line);
void show_lengths(length_t *lengths, size_t elems);
void show_offsets(offset_t *offsets, size_t elems);

length_t get_range(length_t min_len, length_t max_len);
void ** allocate_mep_contents(long long total_sources, size_t mep_factor, float
    margin_factor, size_t element_size);
void deallocate_mep_contents(void **mep_contents, long long total_sources);

length_t * generate_random_lengths(size_t elems, length_t min_len, length_t
    max_len);
void fill_sources_with_random_lengths(length_t *sources, long long
    total_sources, size_t mep_factor, length_t min_len, length_t max_len);
void modify_lengths_randomly(length_t *sources, long long total_sources, size_t
    mep_factor, length_t min_len, length_t max_len);

void copy_MEPs_serial_version(void **mep_contents, offset_t *read_offsets, void *sorted_events, offset_t *write_offsets, long long total_sources, size_t mep_factor, length_t *sources);

#endif
