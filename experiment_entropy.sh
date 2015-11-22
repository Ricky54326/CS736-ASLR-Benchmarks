#!/bin/bash
# runs entropy experiments
OUTPUT_FILE=entropy-output.txt
rm -f $OUTPUT_FILE

# ENTROPY 1
printf "Entropy 1...\t\t"
./run_test ./entropy1
uniq entropy1-output.txt > entropy1-output-uniq.txt
LINES_ORIG1=$(wc -l entropy1-output.txt)
LINES_UNIQ1=$(wc -l entropy1-output-uniq.txt)
COUNT1=$(./sub "$LINES_ORIG" "$LINES_UNIQ")
echo $COUNT1 >> $OUTPUT_FILE
echo $COUNT1

# ENTROPY 2
printf "Entropy 2...\t\t"
./run_test ./entropy2
uniq entropy2-output.txt > entropy2-output-uniq.txt
LINES_ORIG2=$(wc -l entropy2-output.txt)
LINES_UNIQ2=$(wc -l entropy2-output-uniq.txt)
COUNT2=$(./sub "$LINES_ORIG" "$LINES_UNIQ")
echo $COUNT2 >> $OUTPUT_FILE
echo $COUNT2


# ENTROPY 3
printf "Entropy 3...\t\t"
./run_test ./entropy3
uniq entropy3-output.txt > entropy3-output-uniq.txt
LINES_ORIG3=$(wc -l entropy3-output.txt)
LINES_UNIQ3=$(wc -l entropy3-output-uniq.txt)
COUNT3=$(./sub "$LINES_ORIG" "$LINES_UNIQ")
echo $COUNT3 >> $OUTPUT_FILE
echo $COUNT3


