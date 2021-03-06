#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Usage: linux.sh on|off"
	exit
fi


if [ "$1" == "on" ]; then
	VAL=2
elif [ "$1" == "off" ]; then
	VAL=0
	echo "WARNING: make sure to turn ASLR back on after testing!"
fi

printf ""
echo $VAL | tee /proc/sys/kernel/randomize_va_space
