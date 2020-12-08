#!/usr/bin/bash

	if [ $# -lt 1 ]
	then
		echo "Insert file!"
		read file
	else
		file=$1
	fi
	i=0	#contatore vet1
	for string in $(cat $file)
	do
		k=0
		while [ $k -lt $i ]
		do
			if [ "${vet1[$k]}" = "$string" ]	#se ho già memorizzato il contenuto, aumento il contatore
			then
				let vet2[$k]=vet2[$k]+1
				break
			fi
			let k=k+1
		done
		if [ $k -eq $i ]	#se non ho trovato nulla -> nuovo elemento
		then
			vet1[$i]=$string
			let vet2[$i]=1
			let i=i+1
		fi
	done
	k=0
	while [ $k -lt $i ]
	do
		echo ${vet1[$k]}": "${vet2[$k]}
		let k=k+1
	done
	exit 0
			
