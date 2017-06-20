#!/bin/bash 

#$1 - times to run
#$2 - file to run on


COUNTER1=1
while [  $COUNTER1 -le 32 ]; do

	COUNTER2=1
	while [  $COUNTER2 -le $1 ]; do
		./a.out -q -t $COUNTER1 -test -f $2
		let COUNTER2=COUNTER2+1 
	done

	let COUNTER1=COUNTER1+1
done 