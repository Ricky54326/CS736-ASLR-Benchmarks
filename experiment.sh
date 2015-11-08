#!/usr/local/bin/bash

set -e

TEST=bash ./scripts/test.sh

printf "Current cpu speed ... "
# SPEED=$(cat /proc/cpuinfo | grep "cpu MHz" | head -n 1 | cut -d ":" -f 2)

# If you want to specify the cpu speed manually, enter it below:
SPEED="3400.000"
echo "$SPEED MHz"

printf "Rebuilding...\t\t\t\t\t\t\t\t"
gmake clean all > /dev/null
echo "[ OK ]"

#printf "Performance test 1: "
#FAST=$($TEST ./performance1)
#./calc -s $SPEED $FAST

printf "Performance test 2: "
FAST=$($TEST ./performance2)
./calc -s $SPEED $FAST

printf "Performance test 3: "
FAST=$($TEST ./performance3)
./calc -s $SPEED $FAST

printf "Performance test 4: "
FAST=$($TEST ./performance4)
./calc -s $SPEED $FAST
