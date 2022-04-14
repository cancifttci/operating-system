#!/bin/bash

file=$1      # console argument
counter=0

while IFS= read -r line   #reads input from file

do 
	
	echo $'\n'
	
	while [[ $counter -ne $line ]]  # checking end of line in file

	do
		echo -n "*"					#printing *'s
		((counter++))
	done

	echo $'\n'

	counter=0

done<"$file"			#end of file


