#!/bin/bash

for mic_num in 0 1 2
do
  cnt=0
  for n in $(<"./runs-with-1-iteration-MIC$mic_num-28.30.out")
  do
    echo -n "$n"
    ((cnt++))
    if [ $cnt -eq 10 ]
    then
      echo
      cnt=0
    else
      echo -ne "\t"
    fi
  done
  echo
done
