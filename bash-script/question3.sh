#!/bin/bash

[[ $# -eq 1 ]] && curr_dir=$1 || curr_dir="$PWD"		# trick for ternary operator. choosing current directory
 
counter=0				# counter for removed file
if [[ -d $curr_dir ]]				#checking directory is valid
then
	for file in ${curr_dir}/*

	do
		
		if ! [[ -s $file ]] && [[ -f $file ]]		# if it is a regular file and not has greater than 0 size

		then
			rm -rf $file		# removes file
			((counter++))
		else
			continue
		fi

	done
else
	echo "Error: ${curr_dir} not found." && exit 1			# error message and exit with 1
fi
echo "$counter file(S) removed from : $curr_dir"

