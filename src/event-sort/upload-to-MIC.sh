#!/bin/bash

mic_num=0           # default on MIC0
gcc_compile=false
prog_name="event-sort"
benchmark_script="benchmarks.sh"
problem_sizes_benchmark_script="problem-sizes-benchmarks.sh"
run_benchmark=false
run_problem_sizes_benchmark=false
libiomp_dir=/home/xeonphi
libs=../../lib/libiomp5.so
flags=""

while getopts ": 0:gb3i:s:m:n:x:t:1:2:pqe" opt; do
  case $opt in
    0)
      mic_num=$OPTARG
      ;;
    g)
      echo "Compiling with gcc" >&2
      gcc_compile=true
      ;;
    b)
      echo "Running $benchmark_script" >&2
      run_benchmark=true
      ;;
    3)
      echo "Running $problem_sizes_benchmark_script" >&2
      run_problem_sizes_benchmark=true
      ;;
    [ismnxt12pqe])
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

if [ "$gcc_compile" = true ] ; then
  make_what=knc-gcc
  executable="$prog_name".knc.gcc.exe
else
  make_what=knc
  executable="$prog_name".knc.exe
fi

if [ "$run_problem_sizes_benchmark" = true ] ; then
  run_command="sh $problem_sizes_benchmark_script"
elif [ "$run_benchmark" = true ] ; then
  run_command="sh $benchmark_script"
else
  run_command="./$executable $flags"
fi

echo "Running '$run_command' using MIC$mic_num..."

make "$make_what" \
  && scp "$executable" "$benchmark_script" "$problem_sizes_benchmark_script" "$libs" xeonphi@mic$mic_num:~/ \
  && ssh xeonphi@mic$mic_num "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libiomp_dir && $run_command"
