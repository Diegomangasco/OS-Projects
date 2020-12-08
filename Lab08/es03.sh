#!/usr/bin/bash

	if [ $# -lt 1 ]
	then
		echo "Insert directory!"
		read n1
	else
		n1=$1
	fi

	for name in $(ls $n1)
	do
		name2=$(echo $name | tr a-z A-Z)
		path1=$n1"/"$name
		path2=$n1"/"$name2
		mv $path1 $path2
	done
	exit 0
