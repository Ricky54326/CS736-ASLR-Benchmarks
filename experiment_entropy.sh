#!/bin/bash
# runs entropy experiments
OUTPUT_FILE=entropy-output.txt
rm -f $OUTPUT_FILE

# ENTROPY 1
printf "Entropy 1...\t\t"
./run_test ./entropy1
uniq entropy1-output.txt > entropy1-output-uniq.txt
LINES_ORIG=$(wc -l entropy1-output.txt)
LINES_UNIQ=$(wc -l entropy1-output-uniq.txt)
COUNT1=$(./sub "$LINES_ORIG" "$LINES_UNIQ")
echo $COUNT1 >> $OUTPUT_FILE
echo $COUNT1

# ENTROPY 2
echo "Entropy 2"
./run_test ./entropy2;
uniq entropy2-output.txt > entropy2-output-uniq.txt;
COUNT2=$(diff entropy2-output.txt entropy2-output-uniq.txt | wc -l);
echo $COUNT2 >> entropy-output.txt;

# ENTROPY 3
echo "Entropy 3"
./run_test ./entropy3;
uniq entropy3-output.txt > entropy3-output-uniq.txt;
COUNT3=$(diff entropy3-output.txt entropy3-output-uniq.txt | wc -l);
echo $COUNT3 >> entropy-output.txt;





