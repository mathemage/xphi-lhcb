#! /bin/bash

make clean
make median-calculator-icpc

echo "################################################################################"
echo "                               Medians                                          "
for f in *.in
do
  echo "Median of $(cat $f) == $(./median-calculator-icpc <$f)"
done
echo "################################################################################"

echo

echo "################################################################################"
echo "                             Uncertainties                                      "
for f in *.in
do
  echo "Uncertainty of $(cat $f) == $(./median-calculator-icpc --uncertainty <$f)"
done
echo "################################################################################"
