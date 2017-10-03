#!/usr/bin/gnuplot
#set key bottom
set terminal wxt persist
set ylab "modified count" font ", 20"
set xlab "# of input key" font ", 20"
set term postscript enhanced eps color
set style fill pattern
set xtics("1000"0, "2000"1, "3000"2, "4000"3, "5000"4, "5652"5) font ", 15"
#set yrange[0:100]
set size 3, 1.5
set output "tt.sum.eps"
plot "tt.sum" u 1 t "parent" w histogram lt rgb 'black', ""u 2 t "left child" w histogram lt rgb 'black', ""u 3 t "right child" w histogram lt rgb 'black'
