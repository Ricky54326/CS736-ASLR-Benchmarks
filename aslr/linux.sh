#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Usage: linux.sh on|off"
	exit
fi


if [ "$1" == "on" ]; then
	VAL=2
elif [ "$1" == "off" ]; then
	VAL=0
fi

echo "Sudo needed to turn on/off aslr"
sudo printf
echo $VAL | sudo tee /proc/sys/kernel/randomize_va_space
