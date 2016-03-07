#!/bin/bash

seeds="0 83 89"
#ymax="0.28"

for seed in $seeds
do
  outpng="nthreads-vs-stddev-seed-$seed.png"
  gnuplot <<EOF
  set term png truecolor size 1024,768
  set output "$outpng"
  set xlabel "Number of (copy) threads"
  set xrange [1:64]
  set xtics (1,2,4,8,16,32,64)
  set ylabel "(Sample) standard deviation per 10 experiments (in GB/s)"
  #set yrange [0:$ymax]
  set title "Standard deviation (in GB/s) for various number of threads, srand() initialized with seed $seed"
  filename(n)=sprintf("seed-$seed-MIC-%d.dat", n)
  plot for [m=0:2] filename(m) with linespoints title sprintf("kernel.randomize_va_space = %d", m)
EOF
  okular $outpng
done
