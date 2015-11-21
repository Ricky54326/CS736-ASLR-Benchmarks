#!/bin/sh

if [ "$#" -ne 1 ]; then
	echo "Usage: fbsd.sh on|off"
	exit
fi


if [ "$1" == "on" ]; then
	VAL=2
elif [ "$1" == "off" ]; then
	VAL=0
fi

sysctl -w hardening.pax.aslr.status=$VAL
