#!/usr/bin/bash

	if [ $# -lt 1 ]
	then
		echo -n "Insert value!"
		read n1
	else
		n1=$1
	fi
	numLines=$(wc -l $n1 | cut -f 1 -d " ")
	max=0
	while read line 
	do
		numChar=$(echo $line | tr -d " " | wc -c)
		if [ $max -le $numChar ]
		then
			max=$numChar
		fi
	done < $n1
	echo "Lines number: $numLines"
	echo "The longest line has $max Characters"
	exit 0
	
		
