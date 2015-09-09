#!/bin/bash

mic_num=0           # default on MIC0
gcc_compile=false
prog_name="event-sort"
benchmark_script="benchmarks.sh"
run_benchmark=false
libiomp_dir=/home/xeonphi

while getopts ":gm::b" opt; do
  case $opt in
    m)
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

if [ "$run_benchmark" = true ] ; then
  run_command="sh $benchmark_script"
else
  run_command="./$executable"
fi

echo "Using MIC$mic_num..."

make "$make_what" \
  && scp "$executable" "$benchmark_script" $libs xeonphi@mic$mic_num:~/ \
  && ssh xeonphi@mic$mic_num "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$libiomp_dir && $run_command"
