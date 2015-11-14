#!/bin/sh

PROGNAME=`basename $0`
VERSION=1.0
MINARG=2
USAGE=`cat <<_EOD_
Usage: $PROGNAME experiment loop
	run experiment "loop" times
_EOD_`

if [ $# -lt $MINARG ]; then
	echo "$PROGNAME: too few arguments" 1>&2
	echo "$USAGE" 1>&2
	exit 1
fi

for OPT in "$@"; do
	case $OPT in
		'--help'|'-h')
			echo "$USAGE"
			exit 0
			;;
		'--version')
			echo $VERSION
			exit 0
			;;
		-*)
			echo "$PROGNAME: unknown option $OPT" 1>&2
			echo "$USAGE" 1>&2
			exit 1
			;;
	esac
done

#prepare
startTime=`date +%s%N`
rm result/*
rm repeat/*.*
rm repeat/results/*
rm repeat/plots/*
cp config.xml repeat/config.xml.bak
touch repeat/result.log
touch repeat/stderr.log

#repeat trials
loop=`expr $2 - 1`
printf "(bestIndex best worst mean sd) of each position:\r\n\r\n" >> repeat/result.log
for i in `seq 0 $loop`; do
	./AiBench.exe -c repeat/config.xml.bak -e $1 >> repeat/result.log 2>> repeat/stderr.log
	printf "\r\n" >> repeat/result.log

	#copy result
	cd result
	for file in *.log; do
		cp "$file" "../repeat/results/${i}${file}"
	done
	cd ../
done

#plot result
in_file="'repeat/result.log'"
out_file="'repeat/result.png'"
gnuplot << EOF
set terminal png
set output $out_file
set nokey
set ylabel 'fitness (lower is better)'
set xrange [-1:2]
set autoscale y
set boxwidth 1 absolute
set xtics rotate by -45
stats $in_file every ::0::0 using 4 name 'A' nooutput
stats $in_file every ::1::1 using 4 name 'B' nooutput
plot '+' using (0):(A_mean):(A_ssd):xticlabels('validation 1') with boxerrorbars,\
	 '+' using (1):(B_mean):(B_ssd):xticlabels('validation 2') with boxerrorbars
EOF

#print time
endTime=`date +%s%N`
printf "`awk "BEGIN {print ($endTime - $startTime)/1000000000}"` sec elapsed\r\n" >> repeat/result.log
