#! /bin/bash

libiomp_dir=/home/xeonphi/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libiomp_dir

echo './event-sort.mic.exe -s 1 -m 10000000' ; ./event-sort.mic.exe -s 1 -m 10000000
echo './event-sort.mic.exe -s 2 -m 5000000' ; ./event-sort.mic.exe -s 2 -m 5000000
echo './event-sort.mic.exe -s 4 -m 2500000' ; ./event-sort.mic.exe -s 4 -m 2500000
echo './event-sort.mic.exe -s 8 -m 1250000' ; ./event-sort.mic.exe -s 8 -m 1250000
echo './event-sort.mic.exe' ; ./event-sort.mic.exe
echo './event-sort.mic.exe -s 1000 -m 1000 -i 100' ; ./event-sort.mic.exe -s 1000 -m 1000 -i 100
echo './event-sort.mic.exe -s 10000 -m 10000 -i 1  ' ; ./event-sort.mic.exe -s 10000 -m 10000 -i 1  
