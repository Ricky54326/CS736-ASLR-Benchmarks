#!/usr/local/bin/bash

set -e

REPEAT="sh scripts/repeat.sh"
rm -f output.txt parent.txt child.txt
$REPEAT $1
sort -h output.txt | head -n 1
