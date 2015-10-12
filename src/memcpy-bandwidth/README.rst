================
MEMCPY-BANDWIDTH
================

:Author: Karel Ha
:Contact: karel.ha@cern.ch, mathemage@gmail.com
:Created on: $Date: Sat Oct 10 2015 $

GitHub repo
-----------

Move to a folder in AFS::

  mathemage@mathemage-TTL-TEKNOPRO:~$ cd /afs/cern.ch/user/k/kha/
  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ mkdir xphi-lhcb/

and clone the created repo on GitHub::

  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ git clone git@github.com:mathemage/xphi-lhcb.git

Instructions
------------

Rainer instructed to check the speed of the standard memcpy() function. In the event-sort algorithm, memcpy is the performance bottleneck. We suspect that coping overly small blocks data may cause this. To test this hypothesis, parallel mecpy operations for large blocks of data are performed and measured using this program.

Comments on the source code
---------------------------

Code has been written for one architecture without offloads: either running entirely on host or entirely on a MIC.

main.cpp
~~~~~~~~

- `number_of_iterations` = number of iterations, each time several parallel memcopy are run
- `chunk_size` = size of one chunk, which is the block of memory subject to memcopy
- `number_of_chunks` = number of chunks, by default set to high values of 200 to use many threads on Xeon Phi
- `VERBOSE_MODE` = if defined, check that data are copied by showing contents of input and output
- `tick`, `tock` = TBB start/stop time variables for time measurements
- `source`, `destination` = allocated memory for the use of memcopy
- `checkable` = Should the program check, if the data were copied (using `memcmp()`)? If there are >1 GB of data in total, it does not check, because it would take too long.
- `stopwatch_an_iteration()` = perform one iteration (`memcpy()` of all chunks) and return total time of 1 iteration
- `iteration_times` = vector of durations of each iteration
- `initial_time` = time of the 1st iteration (not included in the statistics)
- `total_time` = sum of all `iteration_times`

For other functions and variables, see `../README.rst`.

Output
------

Upload to `mic0` via::

  [kha@lhcb-phi event-sort]$ ./upload-to-MIC.sh
  Running ./memcpy-bandwidth.mic.exe using MIC0...
  icpc -g -O2 -lrt -I../../include -openmp -std=c++14 -qopt-report3 -qopt-report-phase=vec -mmic main.cpp ../utils.cpp -o memcpy-bandwidth.mic.exe
  icpc: remark #10397: optimization reports are generated in *.optrpt files in the output location
  memcpy-bandwidth.mic.exe                                                                           100%  140KB 139.7KB/s   00:00    
  libiomp5.so                                                                                        100% 1268KB   1.2MB/s   00:00    

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.24239 secs
  min: 0.00565 secs
  max: 0.00675 secs
  mean: 0.00611 secs
  Histogram:
  [0.00565, 0.00567): 4 times
  [0.00567, 0.00570): 5 times
  [0.00570, 0.00572): 33 times
  [0.00572, 0.00574): 66 times
  [0.00574, 0.00577): 111 times
  [0.00577, 0.00579): 143 times
  [0.00579, 0.00582): 142 times
  [0.00582, 0.00584): 106 times
  [0.00584, 0.00587): 68 times
  [0.00587, 0.00589): 42 times
  [0.00589, 0.00592): 31 times
  [0.00592, 0.00594): 18 times
  [0.00594, 0.00597): 5 times
  [0.00597, 0.00599): 5 times
  [0.00599, 0.00601): 0 times
  [0.00601, 0.00604): 1 times
  [0.00604, 0.00606): 0 times
  [0.00606, 0.00609): 0 times
  [0.00609, 0.00611): 0 times
  [0.00611, 0.00614): 0 times
  [0.00614, 0.00616): 3 times
  [0.00616, 0.00619): 6 times
  [0.00619, 0.00621): 28 times
  [0.00621, 0.00624): 96 times
  [0.00624, 0.00626): 146 times
  [0.00626, 0.00628): 181 times
  [0.00628, 0.00631): 208 times
  [0.00631, 0.00633): 176 times
  [0.00633, 0.00636): 149 times
  [0.00636, 0.00638): 83 times
  [0.00638, 0.00641): 68 times
  [0.00641, 0.00643): 31 times
  [0.00643, 0.00646): 21 times
  [0.00646, 0.00648): 8 times
  [0.00648, 0.00651): 5 times
  [0.00651, 0.00653): 1 times
  [0.00653, 0.00656): 4 times
  [0.00656, 0.00658): 0 times
  [0.00658, 0.00660): 3 times
  [0.00660, 0.00663): 0 times
  [0.00663, 0.00665): 1 times
  [0.00665, 0.00668): 1 times
  [0.00668, 0.00670): 0 times
  [0.00670, 0.00673): 0 times
  [0.00673, 0.00675): 1 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 12.2235 secs
  Total size: 800 GB
  Throughput: 65.4479 GBps
  ---------------------------

The first iteration is 40 times slower, needs to be investigated...
