================
COMMON FUNCTIONS
================

:Author: Karel Ha
:Contact: karel.ha@cern.ch, mathemage@gmail.com
:Created on: $Date: Wed Sep 9 2015 $

Comments on the source code
---------------------------

utils.cpp
~~~~~~~~~

Following functions has been added:

- `show_array<>()`, `show_lengths()` and `show_offsets()` = procedures to display content of an array (up to given number of elements, otherwise it gives up)
- `get_range()` = calculate range from min and max, fail if invalid minmax values are given
- `init_srand()` = initiate random seed (only once per program run)
- `try_malloc()`, `try_calloc` = malloc and calloc with a check for success
- `allocate_sources()` = allocate buffers per each source: (`total_sources` x `mep_factor`) random lengths
- `deallocate_sources()` = deallocate memory allocated for buffers by `allocate_sources()`
- `generate_random_lengths()` = allocated & fill buffers with random integers in the range [`min_len`, `max_len`]
- `fill_sources_with_random_lengths()` = fill all pre-allocated buffers with random integers in the range [`min_len`, `max_len`]
- `modify_lengths_randomly()` = modify arrays of lengths randomly instead of generating new random numbers every iteration (currently: the first length is changed to another random number, thus affecting all the next offsets)
- `allocate_mep_contents()`, `deallocate_mep_contents()` = (de)allocate memory for `mep_contents`
- `copy_MEPs_serial_version()` = baseline copy `mep_contents` -> `sorted_events` without parallelization
- `copy_MEPs_OMP_version()` = parallelized copy `mep_contents` -> `sorted_events` using OpenMP parallel for

prefix-sum.cpp
~~~~~~~~~~~~~~

- `prefix_sum_sequential()` = sequential algorithm for prefix sum, implemented as a templated function in `prefix-sum.h`
- `get_read_offsets_serial_vesion()` = compute offsets for reading from MEPs without parallelization
- `get_read_offsets_OMP_version()` = compute offsets for reading from MEPs with OpenMP parallel for
- `get_write_offsets_serial_vesion()` = compute offsets for writing into the buffer for sorted events without parallelization
- `get_write_offsets_OMP_vesion()` = compute offsets for writing into the buffer for sorted events with OpenMP parallel for
