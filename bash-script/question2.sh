#!/bin/bash

read -p "Enter number: (For exit the program type < end >) " user_var		#read first numbers 
max=$user_var				# max value
no_entry_flag=0				# will be set 1 if entry was given

while [[ "${user_var,,}" != "end" ]]			#checking user input "end"
do
	
	[[ max -lt user_var ]] && max=$user_var || max=$max						# if max < user_var , user_var will be max, otherwise it ll be same
	read -p "Enter number: (For exit the program type 'end') " user_var		# reads numbers
	no_entry_flag=1			# setting entry flag 1

done

[[ $no_entry_flag -eq 1 ]] && echo "The maximum value is : $max" || echo "No entry"			# printing max value or no entr




