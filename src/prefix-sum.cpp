 
/* ========================================

   * File Name : prefix-sum.cpp

   * Creation Date : 13-08-2015

   * Last Modified : Thu 15 Oct 2015 11:39:32 AM CEST

   * Created By : Karel Ha <mathemage@gmail.com>

   ==========================================*/

#include "prefix-sum.h"


void get_read_offsets_serial_vesion(length_t *sources, offset_t *read_offsets, long long total_sources, size_t mep_factor) {
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

void get_read_offsets_OMP_version(length_t *sources, offset_t *read_offsets, long long total_sources, size_t mep_factor, int nthreads) {
  if (total_sources < 0) {
    fprintf(stderr, "get_read_offsets_OMP_version: ");
    fprintf(stderr, "Invalid value of total_sources!\n");
    exit(EXIT_FAILURE);
  }
  if (mep_factor <= 0) {
    fprintf(stderr, "get_read_offsets_OMP_version: ");
    fprintf(stderr, "Invalid value of mep_factor!\n");
    exit(EXIT_FAILURE);
  }

  if (nthreads > 0) {
#pragma omp parallel for num_threads(nthreads)
    for (long long si = 0; si < total_sources; si++) {
      read_offsets[si * mep_factor] = 0;
      for (long long mi = 1; mi < mep_factor; mi++) {
        read_offsets[si*mep_factor + mi] = read_offsets[si*mep_factor + mi - 1]
          + sources[si * mep_factor + mi-1];
      }
    }
  } else {
#pragma omp parallel for
    for (long long si = 0; si < total_sources; si++) {
      read_offsets[si * mep_factor] = 0;
      for (long long mi = 1; mi < mep_factor; mi++) {
        read_offsets[si*mep_factor + mi] = read_offsets[si*mep_factor + mi - 1]
          + sources[si * mep_factor + mi-1];
      }
    }
  }
}


void get_write_offsets_serial_vesion(length_t *sources, offset_t *write_offsets, long long total_sources, size_t mep_factor) {
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

  offset_t global_offset = 0;
  for (long long mi = 0; mi < mep_factor; mi++) {
    for (long long si = 0; si < total_sources; si++) {
      write_offsets[mi*total_sources + si] = global_offset;
      global_offset += sources[si * mep_factor + mi];
    }
  }
}


void get_write_offsets_OMP_vesion(length_t *sources, offset_t *write_offsets, long long total_sources, size_t mep_factor, offset_t *read_offsets) {
  if (total_sources < 0) {
    fprintf(stderr, "get_write_offsets_OMP_vesion: ");
    fprintf(stderr, "Invalid value of total_sources!\n");
    exit(EXIT_FAILURE);
  }
  if (mep_factor <= 0) {
    fprintf(stderr, "get_write_offsets_OMP_vesion: ");
    fprintf(stderr, "Invalid value of mep_factor!\n");
    exit(EXIT_FAILURE);
  }

#pragma omp parallel for
  for (long long mi = 0; mi < mep_factor; mi++) {
    offset_t local_offset = 0;
    for (long long si = 0; si < total_sources; si++) {
      local_offset += read_offsets[si * mep_factor + mi];
    }

    for (long long si = 0; si < total_sources; si++) {
      write_offsets[mi*total_sources + si] = local_offset;
      local_offset += sources[si * mep_factor + mi];
    }
  }
}
