 
/* ========================================

   * File Name : prefix-sum.cpp

   * Creation Date : 13-08-2015

   * Last Modified : Mon 07 Sep 2015 11:37:24 AM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include "prefix-sum.h"


void get_read_offsets_serial_vesion(length_t *sources, offset_t *read_offsets,
    long long total_sources, size_t mep_factor) {

  if (total_sources < 0) {
    fprintf(stderr, "get_read_offsets_serial_vesion: ");
    fprintf(stderr, "Invalid value of total_sources!\n");
    exit(EXIT_FAILURE);
  }
  if (mep_factor <= 0) {
    fprintf(stderr, "get_read_offsets_serial_vesion: ");
    fprintf(stderr, "Invalid value of mep_factor!\n");
    exit(EXIT_FAILURE);
  }

  for (long long si = 0; si < total_sources; si++) {
    read_offsets[si * mep_factor] = 0;
    for (long long mi = 1; mi < mep_factor; mi++) {
      read_offsets[si*mep_factor + mi] = read_offsets[si*mep_factor + mi - 1]
                                          + sources[si * mep_factor + mi-1];
    }
  }
}


void get_write_offsets_serial_vesion(length_t *sources, offset_t *write_offsets,
    long long total_sources, size_t mep_factor) {

  if (total_sources < 0) {
    fprintf(stderr, "get_write_offsets_serial_vesion: ");
    fprintf(stderr, "Invalid value of total_sources!\n");
    exit(EXIT_FAILURE);
  }
  if (mep_factor <= 0) {
    fprintf(stderr, "get_write_offsets_serial_vesion: ");
    fprintf(stderr, "Invalid value of mep_factor!\n");
    exit(EXIT_FAILURE);
  }

  for (long long mi = 0; mi < mep_factor; mi++) {
    write_offsets[mi * total_sources] = 0;
    for (long long si = 1; si < total_sources; si++) {
      write_offsets[mi*total_sources + si] = write_offsets[mi*total_sources + si - 1]
                                              + sources[ (si-1) * mep_factor + mi];
    }
  }
}
