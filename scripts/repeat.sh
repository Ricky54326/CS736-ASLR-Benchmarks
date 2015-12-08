#!/bin/sh

set -e
COUNT=1

rm -f output.txt
for run in {1..$COUNT}
do
	rm -rf parent.txt child.txt
        $1 > /dev/null
done
