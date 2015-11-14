#!/bin/sh

in_file="'${1}'"
out_file="'${2}'"
xmax=9
ymax=`expr $xmax + 5`

gnuplot << EOF
set terminal gif animate delay 10
set output $out_file
set xrange [0:$xmax]
set yrange [0:$ymax]
set size square
set nokey
stats $in_file nooutput

do for [i=0:int(STATS_blocks)-1] {
	set title sprintf("time = %d", i)
	plot $in_file index i every :::0::0 using 1:2 with circles linestyle 1,\
		'' index i every :::0::0 using (5):($ymax-1-\$0):3 with labels linestyle 1,\
		'' index i every :::0::0 using (5):($ymax-3-\$0):4 with labels linestyle 1,\
		'' index i every :::1::1 using 1:2 with points linestyle 2,\
		'' index i every :::1::1 using 1:2:3 with labels linestyle 2,\
		'' index i every :::2 using 1:2 with points linestyle 3,\
		'' index i every :::2 using 1:2:3 with labels linestyle 3,\
}
EOF
