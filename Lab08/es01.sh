#!/usr/bin/bash

	if [ $# -lt 3 ]
	then
		echo "Insert values!"
		read dire funct fileN
	fi
	dire=$1
	funct=$2
	fileN=$3
	find $dire -regex "$dire\/.+" > "list.txt" #file provvisorio
	while read finput 
	do
		if [ "$finput" != "$dire/list.txt" -a "$finput" != "$dire/fileOutput.txt" ]
		then
			counter=0
			echo $finput
			while read line
			do
				let counter=counter+1
				memline=$line
				k=$(echo $line | grep $funct)	#cerco un match della funzione sulla linea
				if [ "$k" != "" ]	#se ho avuto un match
				then
					echo "File: "$finput" Linea: "$counter " Contenuto: "$memline >> $fileN
				fi
			done < $finput
		fi
	done < "list.txt"
	t=$(sort -k 2,4 $fileN) 	#ordino e riscrivo
	echo $t > $fileN
	exit 0
