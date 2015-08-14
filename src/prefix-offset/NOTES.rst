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
- uncomment `#define VERBOSE_MODE` for "debug" messages

prefix-sum.h and prefix-sum.cpp
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- `prefix_sum_sequential()` = sequential algorithm for prefix sum, templated function

utils.cpp
~~~~~~~~~

- `show_array<>()`, `show_lengths()` and `show_offsets()` = procedures to display content of an array (up to given number of elements, otherwise it gives up)

upload-to-MIC.sh
~~~~~~~~~~~~~~~~

The script compiles the program via::
  
  make mic

Afterwards, it copies to a MIC and executes the program there.

Output
------

Compile, upload to `MIC0` and run via the script...

- for (1000 iterations for 40000000 elements, 0-100 Bytes lengths)::

    [kha@lhcb-phi prefix-offset]$ ./upload-to-MIC.sh
    icpc -lrt -mmic main.cpp utils.cpp prefix-sum.cpp -o mic-prefix-offset.exe
    mic-prefix-offset.exe                                                                              100%   14KB  14.3KB/s   00:00
    Pseudo-terminal will not be allocated because stdin is not a terminal.
    Lengths:
    Too many numbers to display!

    Offsets:
    Too many numbers to display!
    Total time: 521.639 secs
    Processed: 7.66814e+07 elements per second
    Processed: 0.153363 GBps

- for (100 iterations for 40000000 elements, 0-100 Bytes lengths)::

    [kha@lhcb-phi prefix-offset]$ ./upload-to-MIC.sh 
    ...
    Total time: 52.2623 secs
    Processed: 7.6537e+07 elements per second
    Processed: 0.153074 GBps

- for (100000 iterations for 60000 elements, 0-65534 Bytes lengths)::

    [kha@lhcb-phi prefix-offset]$ ./upload-to-MIC.sh 
    ...
    Total elements: 6000000000
    Total time: 77.8086 secs
    Processed: 7.71123e+07 elements per second
    Processed: 0.154225 GBps

- for (100000 iterations for 60000 elements, 0-65534 Bytes lengths) on `MIC2` instead of `MIC0`::

    [kha@lhcb-phi prefix-offset]$ ./upload-to-MIC.sh 2
    ...
    Total elements: 6000000000
    Total time: 78.0641 secs
    Processed: 7.68599e+07 elements per second
    Processed: 0.15372 GBps
