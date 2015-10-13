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

Varying number of threads
~~~~~~~~~~~~~~~~~~~~~~~~~

Run benchmark with flag::

  [kha@lhcb-phi memcpy-bandwidth]$ ./upload-to-MIC.sh -b
  Running ./benchmarks.sh
  Running \'sh ./benchmarks.sh\' using MIC2...
  icpc -g -O2 -lrt -I../../include -openmp -std=c++14 -qopt-report3 -qopt-report-phase=vec -mmic main.cpp ../utils.cpp -o memcpy-bandwidth.mic.exe
  icpc: remark #10397: optimization reports are generated in *.optrpt files in the output location
  memcpy-bandwidth.mic.exe                                                                           100%  143KB 142.8KB/s   00:00
  benchmarks.sh                                                                                      100% 1012     1.0KB/s   00:00
  libiomp5.so                                                                                        100% 1268KB   1.2MB/s   00:00
  Varying the number of threads...
  ./memcpy-bandwidth.mic.exe -i 50 -t 1

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 1.74109 secs
  min: 0.36746 secs
  max: 0.37222 secs
  mean: 0.37069 secs
  Histogram:
  [0.36746, 0.36806): 3 times
  [0.36806, 0.36865): 1 times
  [0.36865, 0.36925): 3 times
  [0.36925, 0.36984): 5 times
  [0.36984, 0.37043): 8 times
  [0.37043, 0.37103): 6 times
  [0.37103, 0.37162): 6 times
  [0.37162, 0.37222): 18 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 18.5343 secs
  Total size: 50 GB
  Throughput: 2.6977 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 2

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.89692 secs
  min: 0.18654 secs
  max: 0.18807 secs
  mean: 0.18708 secs
  Histogram:
  [0.18654, 0.18673): 4 times
  [0.18673, 0.18692): 12 times
  [0.18692, 0.18711): 14 times
  [0.18711, 0.18730): 7 times
  [0.18730, 0.18749): 9 times
  [0.18749, 0.18768): 2 times
  [0.18768, 0.18788): 1 times
  [0.18788, 0.18807): 1 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 9.35416 secs
  Total size: 50 GB
  Throughput: 5.34522 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 4

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.46543 secs
  min: 0.09364 secs
  max: 0.09494 secs
  mean: 0.09397 secs
  Histogram:
  [0.09364, 0.09381): 9 times
  [0.09381, 0.09397): 18 times
  [0.09397, 0.09413): 17 times
  [0.09413, 0.09429): 3 times
  [0.09429, 0.09445): 1 times
  [0.09445, 0.09462): 1 times
  [0.09462, 0.09478): 0 times
  [0.09478, 0.09494): 1 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 4.69872 secs
  Total size: 50 GB
  Throughput: 10.6412 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 8

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.25385 secs
  min: 0.04846 secs
  max: 0.05004 secs
  mean: 0.04899 secs
  Histogram:
  [0.04846, 0.04866): 5 times
  [0.04866, 0.04886): 12 times
  [0.04886, 0.04905): 18 times
  [0.04905, 0.04925): 7 times
  [0.04925, 0.04945): 4 times
  [0.04945, 0.04964): 2 times
  [0.04964, 0.04984): 1 times
  [0.04984, 0.05004): 1 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 2.44951 secs
  Total size: 50 GB
  Throughput: 20.4123 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 16

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.13358 secs
  min: 0.02842 secs
  max: 0.03074 secs
  mean: 0.02930 secs
  Histogram:
  [0.02842, 0.02871): 4 times
  [0.02871, 0.02900): 1 times
  [0.02900, 0.02929): 24 times
  [0.02929, 0.02958): 17 times
  [0.02958, 0.02987): 0 times
  [0.02987, 0.03016): 1 times
  [0.03016, 0.03045): 0 times
  [0.03045, 0.03074): 3 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 1.46499 secs
  Total size: 50 GB
  Throughput: 34.13 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 32

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.07908 secs
  min: 0.01854 secs
  max: 0.02020 secs
  mean: 0.01895 secs
  Histogram:
  [0.01854, 0.01874): 28 times
  [0.01874, 0.01895): 8 times
  [0.01895, 0.01916): 2 times
  [0.01916, 0.01937): 2 times
  [0.01937, 0.01958): 1 times
  [0.01958, 0.01978): 1 times
  [0.01978, 0.01999): 6 times
  [0.01999, 0.02020): 2 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 0.947421 secs
  Total size: 50 GB
  Throughput: 52.7749 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 64

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.05850 secs
  min: 0.01569 secs
  max: 0.01754 secs
  mean: 0.01621 secs
  Histogram:
  [0.01569, 0.01592): 14 times
  [0.01592, 0.01615): 21 times
  [0.01615, 0.01639): 3 times
  [0.01639, 0.01662): 3 times
  [0.01662, 0.01685): 2 times
  [0.01685, 0.01708): 1 times
  [0.01708, 0.01731): 5 times
  [0.01731, 0.01754): 1 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 0.81029 secs
  Total size: 50 GB
  Throughput: 61.7063 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 128

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.04721 secs
  min: 0.01496 secs
  max: 0.01633 secs
  mean: 0.01547 secs
  Histogram:
  [0.01496, 0.01513): 7 times
  [0.01513, 0.01530): 13 times
  [0.01530, 0.01547): 13 times
  [0.01547, 0.01564): 5 times
  [0.01564, 0.01581): 3 times
  [0.01581, 0.01599): 1 times
  [0.01599, 0.01616): 3 times
  [0.01616, 0.01633): 5 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 0.7735 secs
  Total size: 50 GB
  Throughput: 64.6413 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 228

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.04471 secs
  min: 0.01417 secs
  max: 0.01519 secs
  mean: 0.01460 secs
  Histogram:
  [0.01417, 0.01430): 6 times
  [0.01430, 0.01442): 15 times
  [0.01442, 0.01455): 6 times
  [0.01455, 0.01468): 0 times
  [0.01468, 0.01481): 5 times
  [0.01481, 0.01493): 12 times
  [0.01493, 0.01506): 4 times
  [0.01506, 0.01519): 2 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 0.729879 secs
  Total size: 50 GB
  Throughput: 68.5045 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 256

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.10833 secs
  min: 0.01585 secs
  max: 0.01712 secs
  mean: 0.01634 secs
  Histogram:
  [0.01585, 0.01601): 4 times
  [0.01601, 0.01617): 13 times
  [0.01617, 0.01633): 9 times
  [0.01633, 0.01648): 7 times
  [0.01648, 0.01664): 10 times
  [0.01664, 0.01680): 4 times
  [0.01680, 0.01696): 2 times
  [0.01696, 0.01712): 1 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 0.816905 secs
  Total size: 50 GB
  Throughput: 61.2066 GBps
  ---------------------------
  ./memcpy-bandwidth.mic.exe -i 50 -t 512

  --------STATISTICS OF TIME INTERVALS--------
  The initial iteration: 0.55628 secs
  min: 0.01576 secs
  max: 0.01830 secs
  mean: 0.01706 secs
  Histogram:
  [0.01576, 0.01607): 2 times
  [0.01607, 0.01639): 7 times
  [0.01639, 0.01671): 9 times
  [0.01671, 0.01703): 5 times
  [0.01703, 0.01734): 7 times
  [0.01734, 0.01766): 12 times
  [0.01766, 0.01798): 5 times
  [0.01798, 0.01830): 3 times
  --------------------------------------------
  ----------SUMMARY----------
  Total time: 0.852908 secs
  Total size: 50 GB
  Throughput: 58.623 GBps
  ---------------------------
