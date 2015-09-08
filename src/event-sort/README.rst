==========
EVENT SORT
==========

:Author: Karel Ha
:Contact: karel.ha@cern.ch, mathemage@gmail.com
:Created on: $Date: Tue Aug 25 2015 $

GitHub repo
-----------

Move to a folder in AFS::

  mathemage@mathemage-TTL-TEKNOPRO:~$ cd /afs/cern.ch/user/k/kha/
  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ mkdir xphi-lhcb/

and clone the created repo on GitHub::

  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ git clone git@github.com:mathemage/xphi-lhcb.git

Instructions
------------

Rainer instructed to simulate event sorting on Xeon Phi.

Comments on the source code
---------------------------

Code has been written for one architecture without offloads: either running entirely on host or entirely on a MIC.

main.cpp
~~~~~~~~

- `mep_factor` = number of collision that are to be currently processed (default: 1000 collisions)
- `iterations` = number of iterations, each time prefix sum and MEP copy is simulated on Xeon Phi
- `total_sources` = number of sources, i.e. 500 cards each with 2 inputs from the detector -> default:1000 sources
- `min_length`, `max_length` = lower and upper for the size of 1 collision data from 1 source
- `sources` = array of arrays of lengths, `sources[i]` = array of random data lengths for source #i

For other functions and variables, see `../prefix-offset/README.rst`.

../utils.cpp
~~~~~~~~~~~~

Following functions has been added:

- `allocate_sources()` = allocate buffers per each source: (`total_sources` x `mep_factor`) random lengths
- `fill_sources_with_random_lengths()` = fill all pre-allocated buffers with random integers in the range [`min_len`, `max_len`]
- `deallocate_sources()` = deallocate memory allocated for buffers by `allocate_sources()`
- `modify_lengths_randomly()` = modify arrays of lengths randomly instead of generating new random numbers every iteration (currently: the first length is changed to another random number, thus affecting all the next offsets)

../prefix-sum.cpp
~~~~~~~~~~~~~~~~~

- `get_read_offsets_serial_vesion()` = compute offsets for reading from MEPs
- `get_write_offsets_serial_vesion()` = compute offsets for writing into the buffer for sorted events

Output
------

Upload to `mic0` via::

  [kha@lhcb-phi event-sort]$ ./upload-to-MIC.sh
  Using MIC0...
  icpc -g -lrt -I../../include -mmic main.cpp ../prefix-sum.cpp ../utils.cpp -o event-sort.mic.exe
  event-sort.mic.exe                                                                                 100%   40KB  40.2KB/s   00:00

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.91907 secs
  Time for computing write_offsets: 13.9239 secs
  Total time: 22.843 secs
  Total size: 0.4 GB
  Processed (prefix sum): 8.75543e+06 elements per second
  Throughput: 0.0175109 GBps
  ---------------------------

Non-contiguous arrays of source lengths
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Compare results for various number of sources::

  [xeonphi@lhcb-phi-mic0 ~]$ ./event-sort.mic.exe -s 1 -m 10000000

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.91228 secs
  Time for computing write_offsets: 3.95514 secs
  Total time: 12.8674 secs
  Total size: 0.4 GB
  Processed (prefix sum): 1.55431e+07 elements per second
  Throughput: 0.0310863 GBps
  ---------------------------


  [xeonphi@lhcb-phi-mic0 ~]$ ./event-sort.mic.exe -s 2 -m 5000000

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.91137 secs
  Time for computing write_offsets: 6.66393 secs
  Total time: 15.5753 secs
  Total size: 0.4 GB
  Processed (prefix sum): 1.28408e+07 elements per second
  Throughput: 0.0256817 GBps
  ---------------------------


  [xeonphi@lhcb-phi-mic0 ~]$ ./event-sort.mic.exe -s 4 -m 2500000

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.91306 secs
  Time for computing write_offsets: 8.01962 secs
  Total time: 16.9327 secs
  Total size: 0.4 GB
  Processed (prefix sum): 1.18115e+07 elements per second
  Throughput: 0.023623 GBps
  ---------------------------


  [xeonphi@lhcb-phi-mic0 ~]$ ./event-sort.mic.exe -s 8 -m 1250000

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.91786 secs
  Time for computing write_offsets: 8.47783 secs
  Total time: 17.3957 secs
  Total size: 0.4 GB
  Processed (prefix sum): 1.14971e+07 elements per second
  Throughput: 0.0229942 GBps
  ---------------------------


  [xeonphi@lhcb-phi-mic0 ~]$ ./event-sort.mic.exe

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.91394 secs
  Time for computing write_offsets: 13.4045 secs
  Total time: 22.3184 secs
  Total size: 0.4 GB
  Processed (prefix sum): 8.96121e+06 elements per second
  Throughput: 0.0179224 GBps
  ---------------------------

The conclusion: with greater number of sources, the computation time for write_offsets deteriorates. There is a cost to having sources in non-contiguous memory::

  [xeonphi@lhcb-phi-mic2 ~]$ ./event-sort.mic.exe -s 1000 -m 1000 -i 100

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.90061 secs
  Time for computing write_offsets: 13.1774 secs
  Total time: 22.078 secs
  Total size: 0.4 GB
  Processed (prefix sum): 9.05879e+06 elements per second
  Throughput: 0.0181176 GBps
  ---------------------------


  [xeonphi@lhcb-phi-mic2 ~]$ ./event-sort.mic.exe -s 10000 -m 10000 -i 1  

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 9.16011 secs
  Time for computing write_offsets: 52.8355 secs
  Total time: 61.9956 secs
  Total size: 0.4 GB
  Processed (prefix sum): 3.22604e+06 elements per second
  Throughput: 0.00645207 GBps
  ---------------------------

Contiguous arrays of source lengths
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

When contiguous block of memory is allocated for `sources`, the time for computing `write_offsets` decreases significantly for greater number of sources::
    
  [kha@lhcb-phi event-sort]$ ssh xeonphi@mic0
  [xeonphi@lhcb-phi-mic0 ~]$ sh ./benchmarks.sh
  ./event-sort.mic.exe -s 1 -m 10000000

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.7282 secs
  Time for computing write_offsets: 3.95466 secs
  Total time: 12.6829 secs
  Total size: 0.4 GB
  Processed (prefix sum): 1.57693e+07 elements per second
  Throughput: 0.0315386 GBps
  ---------------------------

  ./event-sort.mic.exe -s 2 -m 5000000

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.72957 secs
  Time for computing write_offsets: 6.61965 secs
  Total time: 15.3492 secs
  Total size: 0.4 GB
  Processed (prefix sum): 1.303e+07 elements per second
  Throughput: 0.02606 GBps
  ---------------------------
  ./event-sort.mic.exe -s 4 -m 2500000

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.73222 secs
  Time for computing write_offsets: 7.95306 secs
  Total time: 16.6853 secs
  Total size: 0.4 GB
  Processed (prefix sum): 1.19866e+07 elements per second
  Throughput: 0.0239732 GBps
  ---------------------------
  ./event-sort.mic.exe -s 8 -m 1250000

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.72969 secs
  Time for computing write_offsets: 8.68211 secs
  Total time: 17.4118 secs
  Total size: 0.4 GB
  Processed (prefix sum): 1.14865e+07 elements per second
  Throughput: 0.0229729 GBps
  ---------------------------
  ./event-sort.mic.exe

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.72788 secs
  Time for computing write_offsets: 11.5977 secs
  Total time: 20.3256 secs
  Total size: 0.4 GB
  Processed (prefix sum): 9.83982e+06 elements per second
  Throughput: 0.0196796 GBps
  ---------------------------
  ./event-sort.mic.exe -s 1000 -m 1000 -i 100

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.71245 secs
  Time for computing write_offsets: 12.8127 secs
  Total time: 21.5252 secs
  Total size: 0.4 GB
  Processed (prefix sum): 9.29144e+06 elements per second
  Throughput: 0.0185829 GBps
  ---------------------------
  ./event-sort.mic.exe -s 10000 -m 10000 -i 1

  ----------SUMMARY----------
  Total elements: 2e+08
  Time for computing read_offsets: 8.96645 secs
  Time for computing write_offsets: 40.7559 secs
  Total time: 49.7224 secs
  Total size: 0.4 GB
  Processed (prefix sum): 4.02233e+06 elements per second
  Throughput: 0.00804467 GBps
  ---------------------------

Thus, it's better to use one linear contiguous array `length_t *sources` with appropriate index computation.

Baseline time including copy process (serial copy)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The test with default arguments::

  [kha@lhcb-phi event-sort]$ ./upload-to-MIC.sh
  Using MIC0...
  icpc -g -lrt -I../../include -qopt-report3 -qopt-report-phase=vec -mmic main.cpp ../prefix-sum.cpp ../utils.cpp -o event-sort.mic.exe
  icpc: remark #10397: optimization reports are generated in *.optrpt files in the output location
  event-sort.mic.exe                                                                                 100%   40KB  40.2KB/s   00:00
  benchmarks.sh                                                                                      100%  597     0.6KB/s   00:00

  ----------SUMMARY----------
  Total elements: 1e+08
  Time for computing read_offsets: 8.72954 secs
  Time for computing write_offsets: 9.21006 secs
  Time for copying: 163.676 secs
  Total time: 181.615 secs
  Total size: 0.2 GB
  Processed: 550614 elements per second
  Throughput: 0.00110123 GBps
  ---------------------------

The benchmark script::

  [kha@lhcb-phi event-sort]$ ./upload-to-MIC.sh -b
  Running benchmarks.sh
  Using MIC0...
  icpc -g -lrt -I../../include -qopt-report3 -qopt-report-phase=vec -mmic main.cpp ../prefix-sum.cpp ../utils.cpp -o event-sort.mic.exe
  icpc: remark #10397: optimization reports are generated in *.optrpt files in the output location
  event-sort.mic.exe                                                                                 100%   40KB  40.2KB/s   00:00
  benchmarks.sh                                                                                      100%  597     0.6KB/s   00:00
  ./event-sort.mic.exe -s 1 -m 10000000
  k
  ----------SUMMARY----------
  Total elements: 1e+08
  Time for computing read_offsets: 8.72991 secs
  Time for computing write_offsets: 8.36764 secs
  Time for copying: 38.53 secs
  Total time: 55.6275 secs
  Total size: 0.2 GB
  Processed: 1.79767e+06 elements per second
  Throughput: 0.00359534 GBps
  ---------------------------
  ./event-sort.mic.exe -s 2 -m 5000000

  ----------SUMMARY----------
  Total elements: 1e+08
  Time for computing read_offsets: 8.72921 secs
  Time for computing write_offsets: 7.7414 secs
  Time for copying: 56.6856 secs
  Total time: 73.1562 secs
  Total size: 0.2 GB
  Processed: 1.36694e+06 elements per second
  Throughput: 0.00273388 GBps
  ---------------------------
  ./event-sort.mic.exe -s 4 -m 2500000

  ----------SUMMARY----------
  Total elements: 1e+08
  Time for computing read_offsets: 8.73135 secs
  Time for computing write_offsets: 7.24039 secs
  Time for copying: 64.0729 secs
  Total time: 80.0446 secs
  Total size: 0.2 GB
  Processed: 1.2493e+06 elements per second
  Throughput: 0.00249861 GBps
  ---------------------------
  ./event-sort.mic.exe -s 8 -m 1250000

  ----------SUMMARY----------
  Total elements: 1e+08
  Time for computing read_offsets: 8.72943 secs
  Time for computing write_offsets: 7.24408 secs
  Time for copying: 91.9184 secs
  Total time: 107.892 secs
  Total size: 0.2 GB
  Processed: 926854 elements per second
  Throughput: 0.00185371 GBps
  ---------------------------
  ./event-sort.mic.exe

  ----------SUMMARY----------
  Total elements: 1e+08
  Time for computing read_offsets: 8.72825 secs
  Time for computing write_offsets: 9.21279 secs
  Time for copying: 162.194 secs
  Total time: 180.135 secs
  Total size: 0.2 GB
  Processed: 555139 elements per second
  Throughput: 0.00111028 GBps
  ---------------------------
  ./event-sort.mic.exe -s 1000 -m 1000 -i 100

  ----------SUMMARY----------
  Total elements: 1e+08
  Time for computing read_offsets: 8.71094 secs
  Time for computing write_offsets: 10.4375 secs
  Time for copying: 110.073 secs
  Total time: 129.221 secs
  Total size: 0.2 GB
  Processed: 773868 elements per second
  Throughput: 0.00154774 GBps
  ---------------------------
  ./event-sort.mic.exe -s 10000 -m 10000 -i 1
  benchmarks.sh: line 9:  4715 Segmentation fault      ./event-sort.mic.exe -s 10000 -m 10000 -i 1

.. Note::

  The final test with 10000 sources and 10000 collisons fails with segmentation faults due to insufficent memory for `malloc/calloc`.
