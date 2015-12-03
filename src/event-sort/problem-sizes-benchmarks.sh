#!/bin/bash

niters=10

function scan_block_params {
  srange=`seq 2 32`
  mrange=`seq 4 32`

  for sblock in $srange; do
    for mblock in $mrange; do
      cmd="./event-sort.knc.exe -q -1 $sblock -2 $mblock -s $1 -m $2 -i $niters"
      #echo $cmd >/dev/stdout
      $cmd
    done
    echo
  done
}

for i in `seq 10`; do
  sources=$((100*$i))
  mep_factor=$((1000*$i))
  cmd="scan_block_params $sources $mep_factor | tee $sources-$mep_factor.dat"
  echo $cmd
  $cmd
done
