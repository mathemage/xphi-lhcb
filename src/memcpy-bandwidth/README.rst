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

Same allocated memory for all iterations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

2000 iterations, 200 chunks of 2 MB. Upload to `mic0` via::

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

The first iteration is 40 times slower, which needs to be investigated. Cache lines might be the reasons, since the same block of memory is always copied. Therefore, we will also test what happens if we re-allocate memory for each iteration...

Re-allocated memory on each iteration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Upload to `mic0` via::

  [kha@lhcb-phi memcpy-bandwidth]$ ./upload-to-MIC.sh -i 2000
  Running ./memcpy-bandwidth.mic.exe -i 2000 using MIC0...
  icpc -g -O2 -lrt -I../../include -openmp -std=c++14 -qopt-report3 -qopt-report-phase=vec -mmic main.cpp ../utils.cpp -o memcpy-bandwidth.mic.exe
  icpc: remark #10397: optimization reports are generated in *.optrpt files in the output location
  memcpy-bandwidth.mic.exe                                                                           100%  140KB 139.8KB/s   00:00
  libiomp5.so                                                                                        100% 1268KB   1.2MB/s   00:00

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.24374 secs
  min: 0.02050 secs
  max: 0.11904 secs
  mean: 0.03538 secs
  Histogram:
  [0.02050, 0.02269): 944 times
  [0.02269, 0.02488): 494 times
  [0.02488, 0.02707): 100 times
  [0.02707, 0.02926): 36 times
  [0.02926, 0.03145): 47 times
  [0.03145, 0.03364): 53 times
  [0.03364, 0.03583): 34 times
  [0.03583, 0.03802): 7 times
  [0.03802, 0.04021): 0 times
  [0.04021, 0.04240): 0 times
  [0.04240, 0.04459): 0 times
  [0.04459, 0.04678): 0 times
  [0.04678, 0.04896): 0 times
  [0.04896, 0.05115): 0 times
  [0.05115, 0.05334): 0 times
  [0.05334, 0.05553): 0 times
  [0.05553, 0.05772): 0 times
  [0.05772, 0.05991): 0 times
  [0.05991, 0.06210): 0 times
  [0.06210, 0.06429): 0 times
  [0.06429, 0.06648): 0 times
  [0.06648, 0.06867): 0 times
  [0.06867, 0.07086): 0 times
  [0.07086, 0.07305): 0 times
  [0.07305, 0.07524): 0 times
  [0.07524, 0.07743): 0 times
  [0.07743, 0.07962): 0 times
  [0.07962, 0.08181): 0 times
  [0.08181, 0.08400): 0 times
  [0.08400, 0.08619): 0 times
  [0.08619, 0.08838): 0 times
  [0.08838, 0.09057): 0 times
  [0.09057, 0.09276): 0 times
  [0.09276, 0.09495): 3 times
  [0.09495, 0.09714): 4 times
  [0.09714, 0.09933): 9 times
  [0.09933, 0.10152): 31 times
  [0.10152, 0.10371): 35 times
  [0.10371, 0.10590): 39 times
  [0.10590, 0.10809): 43 times
  [0.10809, 0.11028): 54 times
  [0.11028, 0.11247): 36 times
  [0.11247, 0.11466): 18 times
  [0.11466, 0.11685): 7 times
  [0.11685, 0.11904): 6 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 70.759 secs
  Total size: 800 GB
  Throughput: 11.306 GBps
  ---------------------------

After a discussion with my supervisor, we decided to switch back to the previou
model: same allocated memory for all iterations.

Same allocated memory for all iterations (with number of threads)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

1000 iterations, 500 chunks of 2 MB. To show number of threads, we use `omp_get_num_threads()`::

  [kha@lhcb-phi memcpy-bandwidth]$ ./upload-to-MIC.sh
  Running ./memcpy-bandwidth.mic.exe using MIC0...
  icpc -g -O2 -lrt -I../../include -openmp -std=c++14 -qopt-report3 -qopt-report-phase=vec -mmic main.cpp ../utils.cpp -o memcpy-bandwidth.mic.exe
  icpc: remark #10397: optimization reports are generated in *.optrpt files in the output location
  memcpy-bandwidth.mic.exe                                                                           100%  142KB 142.2KB/s   00:00
  libiomp5.so                                                                                        100% 1268KB   1.2MB/s   00:00
  Starting iteration with 228 threads
  Iteration #1: Starting iteration with 228 threads
  Iteration #2: Starting iteration with 228 threads
  ...
  Iteration #1000: Starting iteration with 228 threads

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.04463 secs
  min: 0.01418 secs
  max: 0.01536 secs
  mean: 0.01464 secs
  Histogram:
  [0.01418, 0.01422): 9 times
  [0.01422, 0.01426): 14 times
  [0.01426, 0.01429): 43 times
  [0.01429, 0.01433): 60 times
  [0.01433, 0.01437): 66 times
  [0.01437, 0.01440): 101 times
  [0.01440, 0.01444): 77 times
  [0.01444, 0.01448): 57 times
  [0.01448, 0.01451): 44 times
  [0.01451, 0.01455): 36 times
  [0.01455, 0.01459): 14 times
  [0.01459, 0.01462): 6 times
  [0.01462, 0.01466): 8 times
  [0.01466, 0.01470): 7 times
  [0.01470, 0.01473): 8 times
  [0.01473, 0.01477): 13 times
  [0.01477, 0.01481): 36 times
  [0.01481, 0.01484): 51 times
  [0.01484, 0.01488): 60 times
  [0.01488, 0.01492): 72 times
  [0.01492, 0.01495): 47 times
  [0.01495, 0.01499): 50 times
  [0.01499, 0.01503): 37 times
  [0.01503, 0.01506): 37 times
  [0.01506, 0.01510): 18 times
  [0.01510, 0.01514): 14 times
  [0.01514, 0.01517): 8 times
  [0.01517, 0.01521): 5 times
  [0.01521, 0.01525): 1 times
  [0.01525, 0.01528): 0 times
  [0.01528, 0.01532): 0 times
  [0.01532, 0.01536): 1 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 14.643 secs
  Total size: 1000 GB
  Throughput: 68.2922 GBps
  ---------------------------
