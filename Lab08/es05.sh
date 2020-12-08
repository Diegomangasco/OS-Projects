#!/usr/bin/bash

	name=$1
	timeout=$2
	zombie=0
	while [ 0 ]
	do
		sleep $timeout
		k=$(ps -el | grep $name)
		echo "$k"
		if [ "$k" = "" ]	#se non lo trovo più
		then
			break
		else
			t=$(cut -f 10 "$k")
			echo "$t"
			if [ "$t" = "Z" ]
			then
				let zombie=zombie+1
			fi
			if [ $zombie -eq 5 ]	#se supero le cinque volte in cui lo trovo zombie
			then
				break;
			fi
		fi
	done
	exit 0
