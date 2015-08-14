=============
PREFIX OFFSET
=============

:Author: Karel Ha
:Contact: karel.ha@cern.ch, mathemage@gmail.com
:Created on: $Date: Wed Aug 12 2015 $

GitHub repo
-----------

Move to a folder in AFS::

  mathemage@mathemage-TTL-TEKNOPRO:~$ cd /afs/cern.ch/user/k/kha/
  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ mkdir xphi-lhcb/

and clone the created repo on GitHub::

  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ git clone git@github.com:mathemage/xphi-lhcb.git

Problem statement
-----------------

Compute offsets of LHCb parts events from an array of their lengths, using algorithms for prefix sum.

Comments on the source code
---------------------------

Code has been written for one architecture without offloads: either running entirely on host or entirely on a MIC.

main.cpp
~~~~~~~~

- `lengths` = input, the lengths of frames / events
- `read_offsets` = absolute positions for each frame, calculated as prefix_sum of preceding lengths

commons.h
~~~~~~~~~

This file contain common includes, typedefs, defines...

- `length_t` = data type for the lengths of frames (uint16_t)
- `offset_t` = data type for the resulting offsets (uint32_t)

prefix-sum.h and prefix-sum.cpp
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `prefix_sum_sequential()` = sequential algorithm for prefix sum, templated function

utils.cpp
~~~~~~~~~

- `show_array<>()`, `show_lengths()` and `show_offsets()` = procedures to display content of an array (up to given number of elements, otherwise it gives up)
