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
