#!/bin/bash
# repeated runs of event-sort with 1 iteration => any variation in throughputs?

prog_name="event-sort"
libiomp_dir=/home/xeonphi
libs=../../lib/libiomp5.so
flags=""
niter=1
sblocksize=28
mblocksize=30
nruns=100

while getopts ": 0:s:m:n:x:t:1:2:pe" opt; do
  case $opt in
    0)
      mic_num=$OPTARG
      ;;
    1)
      sblocksize=$OPTARG
      ;;
    2)
      mblocksize=$OPTARG
      ;;
    [smnxtpe])
      flags="$flags-$opt $OPTARG "
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

make_what=knc
executable="$prog_name".knc.exe
run_command="./$executable $flags -i $niter -q -1 $sblocksize -2 $mblocksize"

function run_on_MIC {
  mic_num=$1
  outfile="./results/runs-with-1-iteration-MIC$mic_num-$sblocksize.$mblocksize.out"
  >$outfile
  for i in $(seq $nruns)
  do
    ssh xeonphi@mic$mic_num \
      "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libiomp_dir && $run_command" \
      >>"$outfile"
  done
  echo "Finished '$run_command' on MIC$mic_num..."
}

for mic_num in 0 1 2
do
  make "$make_what" && scp "$executable" "$libs" xeonphi@mic$mic_num:~/ || continue
  echo "Starting '$run_command' on MIC$mic_num..."
  run_on_MIC $mic_num &
done
