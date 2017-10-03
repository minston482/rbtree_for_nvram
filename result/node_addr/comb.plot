#!/usr/bin/gnuplot
set key bottom
set terminal wxt persist
set ylab "count" font ", 20"
set xlab "address" font ", 20"
set term postscript enhanced eps color
#set style fill solid
#set xtics("256M"0, "512M"1, "1G"2, "2G"3, "4G"4, "8G"5, "16G"6, "32G"7) font ", 15"
#set yrange[0:100]
set size 3, 1.5
set output sprintf('%s.eps', filename);
plot filename u 1 t "original" w line lt 1 lc rgb 'black' lw 2, ""u 2 t "swap" w line lt 1 lc rgb 'green' lw 2
