#! /bin/bash

for f in *.rst; do
	INPUTFILE=$f
	gnuplot -e "filename='$INPUTFILE'" show.plot
	awk '{sump+=$1;suml+=$2;sumr+=$3}END{print sump,suml,sumr}' $INPUTFILE >> $1.sum

done
exit
