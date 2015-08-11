======================
OFFLOAD BANDWIDTH DEMO
======================

:Author: Karel Ha
:Contact: karel.ha@cern.ch, mathemage@gmail.com
:Created on: $Date: Sun Aug 09 2015 $

GitHub repo
-----------

Move to a folder in AFS::

  mathemage@mathemage-TTL-TEKNOPRO:~$ cd /afs/cern.ch/user/k/kha/
  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ mkdir xphi-lhcb/

and clone the created repo on GitHub::

  mathemage@mathemage-TTL-TEKNOPRO:/afs/cern.ch/user/k/kha$ git clone git@github.com:mathemage/xphi-lhcb.git

Comments on the source code
---------------------------

- Class `tbb::tick_count` used for time measurements.
- command-line args:
  
  - argument `-m` for switching between MIC cards
  - argument `-i` for number of iterations
  - argument `-e` for number of elements

- Use `ALLOC`, `REUSE` and `FREE` to configure `offload` mode:

  - Whether to allocate on the MIC...
  - Whether to deallocate on the MIC...
    
- Variable `pData` points to data to be offloaded to MIC.
  
  - It's dynamically allocated by `calloc()`.
  - Initial allocation is done by `offload_transfer ... nocopy(... ALLOC)`.
  - Final deallocation is done by `offload_transfer ... nocopy(... FREE)`.
  - In-between, `REUSE` makes data to be only copied. Hence, (de)allocation
    times are not counted towards the total time.

- Identifier `VERBOSE_MODE` turns on "debug" messages. (Un)comment the
  corresponding `#define` directive.

  - It can be checked `pData` points to different addresses on the host and the device.
  - Also, the first element is modified (with periodically repeating letters of
    alphabet). Thus, the actual data are copied, if the first elements on both
    the device and the MIC match each other.

Output
------

Compile using `make` and launch::

  [kha@lhcb-phi offload-bandwidth]$ make && ./offload-bandwidth.exe
  icpc -lrt main.cpp -o offload-bandwidth.exe
  Transfered: 200 GB
  Total time: 30.0943 secs
  Bandwidth: 6.64577 GBps

or in `VERBOSE_MODE`::

  [kha@lhcb-phi offload-bandwidth]$ make && ./offload-bandwidth.exe
  icpc -lrt main.cpp -o offload-bandwidth.exe
  Iteration #0
  pData@host: 0x7f85a3220010               pData[0]@host == a
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == a

  Iteration #1
  pData@host: 0x7f85a3220010               pData[0]@host == b
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == b

  Iteration #2
  pData@host: 0x7f85a3220010               pData[0]@host == c
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == c

  Iteration #3
  pData@host: 0x7f85a3220010               pData[0]@host == d
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == d

  Iteration #4
  pData@host: 0x7f85a3220010               pData[0]@host == e
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == e

  Iteration #5
  pData@host: 0x7f85a3220010               pData[0]@host == f
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == f

  Iteration #6
  pData@host: 0x7f85a3220010               pData[0]@host == g
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == g

  Iteration #7
  pData@host: 0x7f85a3220010               pData[0]@host == h
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == h

  Iteration #8
  pData@host: 0x7f85a3220010               pData[0]@host == i
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == i

  Iteration #9
  pData@host: 0x7f85a3220010               pData[0]@host == j
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == j

  Iteration #10
  pData@host: 0x7f85a3220010               pData[0]@host == k
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == k

  Iteration #11
  pData@host: 0x7f85a3220010               pData[0]@host == l
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == l

  Iteration #12
  pData@host: 0x7f85a3220010               pData[0]@host == m
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == m

  Iteration #13
  pData@host: 0x7f85a3220010               pData[0]@host == n
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == n

  Iteration #14
  pData@host: 0x7f85a3220010               pData[0]@host == o
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == o

  Iteration #15
  pData@host: 0x7f85a3220010               pData[0]@host == p
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == p

  Iteration #16
  pData@host: 0x7f85a3220010               pData[0]@host == q
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == q

  Iteration #17
  pData@host: 0x7f85a3220010               pData[0]@host == r
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == r

  Iteration #18
  pData@host: 0x7f85a3220010               pData[0]@host == s
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == s

  Iteration #19
  pData@host: 0x7f85a3220010               pData[0]@host == t
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == t

  Iteration #20
  pData@host: 0x7f85a3220010               pData[0]@host == u
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == u

  Iteration #21
  pData@host: 0x7f85a3220010               pData[0]@host == v
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == v

  Iteration #22
  pData@host: 0x7f85a3220010               pData[0]@host == w
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == w

  Iteration #23
  pData@host: 0x7f85a3220010               pData[0]@host == x
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == x

  Iteration #24
  pData@host: 0x7f85a3220010               pData[0]@host == y
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == y

  Iteration #25
  pData@host: 0x7f85a3220010               pData[0]@host == z
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == z

  Iteration #26
  pData@host: 0x7f85a3220010               pData[0]@host == a
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == a

  Iteration #27
  pData@host: 0x7f85a3220010               pData[0]@host == b
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == b

  Iteration #28
  pData@host: 0x7f85a3220010               pData[0]@host == c
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == c

  Iteration #29
  pData@host: 0x7f85a3220010               pData[0]@host == d
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == d

  Iteration #30
  pData@host: 0x7f85a3220010               pData[0]@host == e
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == e

  Iteration #31
  pData@host: 0x7f85a3220010               pData[0]@host == f
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == f

  Iteration #32
  pData@host: 0x7f85a3220010               pData[0]@host == g
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == g

  Iteration #33
  pData@host: 0x7f85a3220010               pData[0]@host == h
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == h

  Iteration #34
  pData@host: 0x7f85a3220010               pData[0]@host == i
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == i

  Iteration #35
  pData@host: 0x7f85a3220010               pData[0]@host == j
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == j

  Iteration #36
  pData@host: 0x7f85a3220010               pData[0]@host == k
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == k

  Iteration #37
  pData@host: 0x7f85a3220010               pData[0]@host == l
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == l

  Iteration #38
  pData@host: 0x7f85a3220010               pData[0]@host == m
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == m

  Iteration #39
  pData@host: 0x7f85a3220010               pData[0]@host == n
  pData@MIC1: 0x7fd42ff40010               pData[0]@MIC1 == n

  Transfered: 200 GB
  Total time: 30.1241 secs
  Bandwidth: 6.6392 GBps

Parallel run on all available MICs
----------------------------------

For running on all 4 MICs available at `lhcb-phi` machine, launch::

  [kha@lhcb-phi offload-bandwidth]$ ./run-on-all-MICs.sh 
  icpc -lrt main.cpp -o offload-bandwidth.exe
  Launching offload-bandwith on MIC 0...
  Launching offload-bandwith on MIC 1...
  Launching offload-bandwith on MIC 2...
  Launching offload-bandwith on MIC 3...

  [kha@lhcb-phi offload-bandwidth]$ cat *.out
  Using MIC0...
  Transferred: 20 GB
  Total time: 4.39036 secs
  Bandwidth: 4.55544 GBps
  Using MIC1...
  Transferred: 20 GB
  Total time: 4.53063 secs
  Bandwidth: 4.4144 GBps
  Using MIC2...
  Transferred: 20 GB
  Total time: 4.39714 secs
  Bandwidth: 4.54841 GBps
  Using MIC3...
  Transferred: 20 GB
  Total time: 4.52327 secs
  Bandwidth: 4.42158 GBps

Instead of a single run::

  [kha@lhcb-phi offload-bandwidth]$ ./offload-bandwidth.exe -i 20 -e 1500000000
  Using MIC0...
  Transferred: 20 GB
  Total time: 4.37642 secs
  Bandwidth: 4.56995 GBps

However, when larger array of data is transferred, bandwidth increases::

  [kha@lhcb-phi offload-bandwidth]$ ./offload-bandwidth.exe -i 20 -e 5000000000
  Using MIC0...
  Transferred: 100 GB
  Total time: 14.609 secs
  Bandwidth: 6.8451 GBps
