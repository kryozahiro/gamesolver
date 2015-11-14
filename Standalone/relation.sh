#!/bin/sh

in_file="'${1}'"
out_file="'${2}'"

gnuplot << EOF
set terminal png
set output $out_file
set key off
set xlabel 'generation'
set ylabel 'fitness (lower is better)'
set autoscale fix
plot $in_file with lines
EOF
