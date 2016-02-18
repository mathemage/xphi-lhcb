#!/bin/bash

for mic_num in 0 1 2
do
  base="runs-with-1-iteration-MIC$mic_num-28.30"
  keyword="mean"
	grep $keyword $base.results.out | sed -s 's/[^0-9 ]*//g' >$base.numbers.out
done
