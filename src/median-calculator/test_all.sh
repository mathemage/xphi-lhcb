#! /bin/bash

make clean
make median-calculator-icpc

for f in *.in
do
  echo "Median of $(cat $f) == $(./median-calculator-icpc <$f)"
done
