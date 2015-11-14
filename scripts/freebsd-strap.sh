#!/bin/bash
set -e
STRAP_FILE=/etc/grub.d/40_custom

echo ""					>> $STRAP_FILE
echo "menuentry 'FreeBSD'" 		>> $STRAP_FILE
echo "    set root='(hd1,0)'" 	 	>> $STRAP_FILE
echo "    chainloader +1"		>> $STRAP_FILE
echo "}"				>> $STRAP_FILE
