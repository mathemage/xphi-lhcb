#! /bin/bash
niters=100

for i in `seq 0 20`; do
  sources=600
  events=$((5800 + 20*$i))
  gnuplot << EOF
    reset
    set term png truecolor size 1024,768
    set output "$sources-$events.png"
    set xlabel "Events per block"
    set ylabel "Sources per block"
    set xrange [4:32]
    set yrange [2:32]
    set cbrange [10:30]

    set title "Throughputs for various dimensions of blocks (in GB/s), $sources sources, MEP factor of $events, $niters iterations"
    plot "./$sources-$events.dat" u (\$1+4):(\$2+2):3 matrix w image notitle
EOF
done
