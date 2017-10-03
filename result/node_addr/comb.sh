#! /bin/bash

for f in *.rst; do
    INPUTFILE=$f
    gnuplot -e "filename='$INPUTFILE'" comb.plot
done
exit

