#!/bin/bash
sed -e '1,12d' $1 > $1.prep
awk '{print $7,$8}' $1.prep > $1.result
