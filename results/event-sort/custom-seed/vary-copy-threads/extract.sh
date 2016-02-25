#!/bin/bash

outdir="."

for s in 0 83 89
do
  for m in 0 1 2
  do
    outfile="$outdir/seed-$s-MIC-$m.dat"
    >$outfile
    for f in seed-$s/runs-with-1-iteration-MIC$m-28.30.srand-seed-$s-copy-threads-{1..64}.results.out
    do
      if [ -f "$f" ]
      then
        cat $f | sed 's/.*copy-threads == \([0-9]*\):/\1/' | sed 's/.*deviation = \([0-9\.]*\).*/\1/' | tr '\n' ' '
        echo
      fi >>$outfile
    done
  done
done
