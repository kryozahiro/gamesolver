#!/bin/sh

trial=${1}
stage=${2}
validation=${3}

summary=${trial}s${stage}summary
relation=${trial}s${stage}relation
validator=${trial}s${stage}validator${validation}

if [ -e repeat/results/${summary}.log ]; then
	sh summary.sh repeat/results/${summary}.log repeat/plots/${summary}.png
fi
if [ -e repeat/results/${relation}.log ]; then
	sh relation.sh repeat/results/${relation}.log repeat/plots/${relation}.png
fi
if [ -e repeat/results/${validator}.log ]; then
	sh animation.sh repeat/results/${validator}.log repeat/plots/${validator}.gif
fi
