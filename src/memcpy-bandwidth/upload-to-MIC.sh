#!/bin/bash

mic_num=0           # default on MIC0
gcc_compile=false
prog_name="memcpy-bandwidth"
libiomp_dir=/home/xeonphi
libs=../../lib/libiomp5.so
number_of_iterations=0
chunk_size=0
number_of_chunks=0
verbose_enabled=false
nthreads=0
run_benchmark=false
benchmark_script="./benchmarks.sh"

while getopts ":g:bm:i:c:n:t::v" opt; do
  case $opt in
    m)
      mic_num=$OPTARG
      ;;
    g)
      echo "Compiling with gcc" >&2
      gcc_compile=true
      ;;
    i)
      number_of_iterations=$OPTARG
      ;;
    c)
      chunk_size=$OPTARG
      ;;
    n)
      number_of_chunks=$OPTARG
      ;;
    t)
      nthreads=$OPTARG
      ;;
    v)
      verbose_enabled=true
      ;;
    b)
      echo "Running $benchmark_script" >&2
      run_benchmark=true
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
  make_what=mic-gcc
  executable="$prog_name".mic.gcc.exe
else
  make_what=mic
  executable="$prog_name".mic.exe
fi

run_command="./$executable" >&2

if [ "$number_of_iterations" -gt 0 ] ; then
  run_command="$run_command -i $number_of_iterations"
fi

if [ "$chunk_size" -gt 0 ] ; then
  run_command="$run_command -c $chunk_size"
fi

if [ "$number_of_chunks" -gt 0 ] ; then
  run_command="$run_command -n $number_of_chunks"
fi

if [ "$verbose_enabled" = true ] ; then
  run_command="$run_command -v"
fi

if [ "$nthreads" -gt 0 ] ; then
  run_command="$run_command -t $nthreads"
fi



if [ "$run_benchmark" = true ] ; then
  run_command="sh $benchmark_script"
fi

echo "Running '$run_command' using MIC$mic_num..."

make "$make_what" \
  && scp "$executable" "$benchmark_script" $libs xeonphi@mic$mic_num:~/ \
  && ssh xeonphi@mic$mic_num "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libiomp_dir && $run_command"
