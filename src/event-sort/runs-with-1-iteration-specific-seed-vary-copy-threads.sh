#!/bin/bash
# repeated runs of event-sort with 1 iteration and custom srand() seed (to fix input dataset) and vary number of threads for copying MEP fragments => any variation in throughputs?

prog_name="event-sort"
libiomp_dir=/home/xeonphi
libs=../../lib/libiomp5.so
flags=""
niter=1
sblocksize=28
mblocksize=30
nruns=10
calc="statistics-calculator-icpc"
calcdir="../statistics-calculator/"
seed=${1:-0}

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

function run_on_MIC {
  mic_num=$1
  outdir="../../results/event-sort/custom-seed/vary-copy-threads/seed-$seed"
  mkdir $outdir -p

  for cpthreads in 1 2 4 8 16 32 64
  do
    prefix="$outdir/runs-with-1-iteration-MIC$mic_num-$sblocksize.$mblocksize.srand-seed-$seed-copy-threads-$cpthreads"
    rawoutfile="$prefix.raw.out"
    >$rawoutfile
    resultsoutfile="$prefix.results.out"
    >$resultsoutfile

    run_command="./$executable $flags -i $niter -q -1 $sblocksize -2 $mblocksize --srand-seed $seed --copy-threads $cpthreads"
    echo "Starting '$run_command' on MIC$mic_num..."
    echo "srand-seed == $seed, copy-threads == $cpthreads:" | tee -a $rawoutfile >>$resultsoutfile
    for i in $(seq $nruns)
    do
      ssh xeonphi@mic$mic_num "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libiomp_dir && $run_command"
    done | tee -a $rawoutfile | $calcdir/$calc >>$resultsoutfile
    echo | tee -a $rawoutfile >>$resultsoutfile
    echo "Finished '$run_command' on MIC$mic_num..."
  done
}

(
cd $calcdir
make clean
make $calc
)

for mic_num in 0 1 2
do
  make "$make_what" && scp "$executable" "$libs" xeonphi@mic$mic_num:~/ || continue
  run_on_MIC $mic_num &
done
