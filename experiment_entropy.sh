#!/bin/bash
# runs entropy experiments

# ENTROPY 1
echo "Entropy 1"
./run_test ./entropy1;
uniq entropy1-output.txt > entropy1-output-uniq.txt;
COUNT1=$(diff entropy1-output.txt entropy1-output-uniq.txt | wc -l);
echo $COUNT1 >> entropy-output.txt;


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





