#! /bin/bash
niters=100

# problem sizes 1000x1000, 1000x2000, 1000x3000...
for i in `seq 10`; do
  sources=1000
  events=$((1000*$i))
  gnuplot << EOF
    reset
    set term png truecolor size 1024,768
    set output "$sources-$events.png"
    set xlabel "Events per block"
    set ylabel "Sources per block"
    set xrange [4:32]
    set yrange [2:32]

    set title "Throughputs for various dimensions of blocks (in GB/s), $sources sources, MEP factor of $events, $niters iterations"
    plot "./$sources-$events.dat" u (\$1+4):(\$2+2):3 matrix w image notitle
EOF
done

# problem sizes 1000x11000, 1000x12000, 1000x13000...
for i in `seq 10`; do
  sources=1000
  events=$((1000*$i + 10000))
  gnuplot << EOF
    reset
    set term png truecolor size 1024,768
    set output "$sources-$events.png"
    set xlabel "Events per block"
    set ylabel "Sources per block"
    set xrange [4:32]
    set yrange [2:32]

    set title "Throughputs for various dimensions of blocks (in GB/s), $sources sources, MEP factor of $events, $niters iterations"
    plot "./$sources-$events.dat" u (\$1+4):(\$2+2):3 matrix w image notitle
EOF
done
