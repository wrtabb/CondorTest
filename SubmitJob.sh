#!/bin/bash

entries=(
	100
	500
	1000
	5000
	10000
	50000
	-1000
)
for index in ${!entries[*]}; do
	echo "Beginning to process ${entries[$index]} Entries"
	condor_submit \
		arg1=${entries[index]} \
		condor_control.condor
done
