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
  sources=$((100*$i))
  mep_factor=$((1000*$i))
  scan_block_params $sources $mep_factor | tee "$directory/$sources-$mep_factor.dat"
done
