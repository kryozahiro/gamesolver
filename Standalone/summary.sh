#!/bin/sh

in_file="'${1}'"
out_file="'${2}'"

gnuplot << EOF
set terminal png
set output $out_file
set key outside center bottom horizontal box
set xlabel 'generation'
set ylabel 'fitness (lower is better)'
set autoscale fix
plot $in_file using 1:2 title 'best' with lines,\
	'' using 1:3 title 'worst' with lines,\
	'' using 1:4 title 'mean' with lines
EOF
