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

To verify the correctness of `prefix_sum_sequential()`, launch for an array of 80 elements::

  [xeonphi@lhcb-phi-mic0 ~]$ ./mic-prefix-offset.exe -e 80 -i 1
  Lengths:
  52678   53150   21024   24637   63808   57372    3267   39672   65066   54772   14075   32233   14128    8229   35161   16995
  23082   54173   25652    8246   62507   48032   29148   34841   48181   13052   27121   60262   47210   24583   41410   34354
  12198   62434   58991   43238   21503   62258   17376   21034   18728   31451   20499      88    6912   22892   17083   29995
  44298    9968   38241   41270   58000   34621   10576   40646   14906    4929    2605   62116   62279   44015   63702   41710
   8146   57158   19413   29649   21114   36789   17916   39842   35473   38415   39930   42385   61308   24245    6845   40071
  Offsets:
           0           52678          105828          126852          151489          215297          272669          275936
      315608          380674          435446          449521          481754          495882          504111          539272
      556267          579349          633522          659174          667420          729927          777959          807107
      841948          890129          903181          930302          990564         1037774         1062357         1103767
     1138121         1150319         1212753         1271744         1314982         1336485         1398743         1416119
     1437153         1455881         1487332         1507831         1507919         1514831         1537723         1554806
     1584801         1629099         1639067         1677308         1718578         1776578         1811199         1821775
     1862421         1877327         1882256         1884861         1946977         2009256         2053271         2116973
     2158683         2166829         2223987         2243400         2273049         2294163         2330952         2348868
     2388710         2424183         2462598         2502528         2544913         2606221         2630466         2637311
  Total elements: 80
  Total time: 1.769e-06 secs
  Processed: 4.52233e+07 elements per second
  Processed: 0 GBps

  [xeonphi@lhcb-phi-mic0 ~]$ ./mic-prefix-offset.exe -e 80 -i 1
  Lengths:
   4846   19358   58830   35553   20003   50367   23122   28620   39817   21660   27927   36881   64784   46096   25501   18377
  57560   28875   28532   24501   56173   51374   33630   21459   14949    2173   65324     272   46290   33625   41393   18369
  52984    1921   53922   40219   19520   11509    3304   59337   33170   63998   30683   65186   44560   23416   50795    3817
  52292   13792   28318   10162   65166   29180   64388   14580   31353   64178   14853   44875   32268   23478   30476   52484
  25399   18864   27169   12152   63140   63240    5954   63542   61704   36638   30425    7961   27286   15685   11778   14043
  Offsets:
           0            4846           24204           83034          118587          138590          188957          212079
      240699          280516          302176          330103          366984          431768          477864          503365
      521742          579302          608177          636709          661210          717383          768757          802387
      823846          838795          840968          906292          906564          952854          986479         1027872
     1046241         1099225         1101146         1155068         1195287         1214807         1226316         1229620
     1288957         1322127         1386125         1416808         1481994         1526554         1549970         1600765
     1604582         1656874         1670666         1698984         1709146         1774312         1803492         1867880
     1882460         1913813         1977991         1992844         2037719         2069987         2093465         2123941
     2176425         2201824         2220688         2247857         2260009         2323149         2386389         2392343
     2455885         2517589         2554227         2584652         2592613         2619899         2635584         2647362
  Total elements: 80
  Total time: 2.048e-06 secs
  Processed: 3.90625e+07 elements per second
  Processed: 0 GBps

Results after compilation by gcc
--------------------------------

For a `gcc` cross-compiler, use `/usr/linux-k1om-4.7/bin/x86_64-k1om-linux-gcc`
according to https://software.intel.com/en-us/forums/topic/388552 .

Run the script with `gcc` flag `-g`::

  [kha@lhcb-phi prefix-offset]$ ./upload-to-MIC.sh -g
  Compiling with gcc
  /usr/linux-k1om-4.7/bin/x86_64-k1om-linux-gcc -lrt main.cpp utils.cpp prefix-sum.cpp -o mic-gcc-prefix-offset.exe
  main.cpp: In function 'int main(int, char**)':
  main.cpp:29:58: warning: deprecated conversion from string constant to 'char*' [-Wwrite-strings]
  main.cpp:32:56: warning: deprecated conversion from string constant to 'char*' [-Wwrite-strings]
  mic-gcc-prefix-offset.exe                                                                          100%   15KB  15.2KB/s   00:00    
  Lengths:
  Too many numbers to display!
  Offsets:
  Too many numbers to display!
  Total elements: 6000000000
  Total time: 270.995 secs
  Processed: 2.21406e+07 elements per second
  Processed: 0.0442812 GBps

  [kha@lhcb-phi prefix-offset]$ ./upload-to-MIC.sh -gm 2
  Compiling with gcc
  Using MIC2...
  ...
  Total elements: 6000000000
  Total time: 270.673 secs
  Processed: 2.2167e+07 elements per second
  Processed: 0.044334 GBps
