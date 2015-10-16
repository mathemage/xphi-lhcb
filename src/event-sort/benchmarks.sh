#! /bin/bash

echo "Varying the number of sources and the MEP factor..."
echo './event-sort.mic.exe -s 1 -m 10000000' ; ./event-sort.mic.exe -s 1 -m 10000000
echo './event-sort.mic.exe -s 2 -m 5000000' ; ./event-sort.mic.exe -s 2 -m 5000000
echo './event-sort.mic.exe -s 4 -m 2500000' ; ./event-sort.mic.exe -s 4 -m 2500000
echo './event-sort.mic.exe -s 8 -m 1250000' ; ./event-sort.mic.exe -s 8 -m 1250000
echo './event-sort.mic.exe' ; ./event-sort.mic.exe
echo './event-sort.mic.exe -s 1000 -m 1000 -i 100' ; ./event-sort.mic.exe -s 1000 -m 1000 -i 100

echo
echo "Varying the number of iterations..."
echo './event-sort.mic.exe -i 1' ; ./event-sort.mic.exe -i 1
echo './event-sort.mic.exe -i 10' ; ./event-sort.mic.exe -i 10
# careful: these might take some time!!
echo './event-sort.mic.exe -i 100' ; ./event-sort.mic.exe -i 100
echo './event-sort.mic.exe -i 200' ; ./event-sort.mic.exe -i 200
echo './event-sort.mic.exe -i 1000' ; ./event-sort.mic.exe -i 1000

echo "Varying the number of threads..."
echo './event-sort.mic.exe -t 1 ' ; ./event-sort.mic.exe -t 1 
echo './event-sort.mic.exe -t 2 ' ; ./event-sort.mic.exe -t 2 
echo './event-sort.mic.exe -t 4 ' ; ./event-sort.mic.exe -t 4 
echo './event-sort.mic.exe -t 8 ' ; ./event-sort.mic.exe -t 8 
echo './event-sort.mic.exe -t 16' ; ./event-sort.mic.exe -t 16
echo './event-sort.mic.exe -t 32' ; ./event-sort.mic.exe -t 32
echo './event-sort.mic.exe -t 64' ; ./event-sort.mic.exe -t 64
echo './event-sort.mic.exe -t 128' ; ./event-sort.mic.exe -t 128
echo './event-sort.mic.exe -t 228' ; ./event-sort.mic.exe -t 228
echo './event-sort.mic.exe -t 256' ; ./event-sort.mic.exe -t 256
echo './event-sort.mic.exe -t 512' ; ./event-sort.mic.exe -t 512

echo "Varying the dimensions of memcpy blocks..."
range="1 2 4 8 16 32 64 80 96 128"
echo -e "\t\t1\t2\t4\t8\t16\t32\t64\t80\t96\t128"
echo -e "\t_______________________________________________________________________________________"
for x in $range; do
  echo -ne "$x\t|\t"
  for y in $range; do
    #echo -ne "$x&$y\t"
    ./event-sort.mic.exe -q -1 $x -2 $y
  done
  echo
done
