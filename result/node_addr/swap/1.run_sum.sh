#! /bin/bash

for f in *.rst; do
	INPUTFILE=$f
	./sum_node $INPUTFILE > $INPUTFILE.sum
done
exit
