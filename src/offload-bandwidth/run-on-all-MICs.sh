#!/bin/bash

last_mic_num=${1:-3}
iters=90
elems=1000000000

make || exit

for i in `seq 0 $last_mic_num`;
do
  echo "Launching offload-bandwith on MIC $i..."
  ./offload-bandwidth.exe -m $i -i $iters -e $elems &>$i.out &
done
