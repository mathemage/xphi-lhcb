#!/bin/bash

mic_num=0           # default on MIC0
gcc_compile=false

while getopts ":gm:" opt; do
  case $opt in
    m)
      mic_num=$OPTARG
      echo "Using MIC$mic_num..."
      ;;
    g)
      echo "Compiling with gcc" >&2
      gcc_compile=true
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
  executable=mic-gcc-prefix-offset.exe
else
  make_what=mic
  executable=mic-prefix-offset.exe
fi

make "$make_what" \
  && scp "$executable" xeonphi@mic$mic_num:~/ \
  && ssh xeonphi@mic$mic_num "./$executable"
