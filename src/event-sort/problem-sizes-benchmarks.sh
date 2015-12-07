#!/bin/bash

niters=100

function scan_block_params {
  srange=`seq 2 32`
  mrange=`seq 4 32`

  for sblock in $srange; do
    for mblock in $mrange; do
      cmd="./event-sort.knc.exe -q -1 $sblock -2 $mblock -s $1 -m $2 -i $niters"
      #echo $cmd
      $cmd
    done
    echo
  done
}

directory="./results/problem-sizes-variation/$niters-iterations"
mkdir $directory -p

for i in `seq 10`; do
  # problem sizes 100x1000, 200x2000, 300x3000...
  #sources=$((100*$i))
  #mep_factor=$((1000*$i))

  # problem sizes 1000x1000, 1000x2000, 1000x3000...
  #sources=1000
  #mep_factor=$((1000*$i))

  # problem sizes 1000x10000, 1000x11000, 1000x12000...
  sources=1000
  mep_factor=$((1000*$i + 10000))
  scan_block_params $sources $mep_factor | tee "$directory/$sources-$mep_factor.dat"
done
