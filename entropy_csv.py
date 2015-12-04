#!/usr/bin/python

"""entropy_csv.py: Cleaning entropy data for R graphing."""

__author__      = "Riccardo Mutschlechner <riccardo@cs.wisc.edu>, John Detter <jdetter@wisc.edu>"

import collections
import os
import cPickle as pickle
import subprocess
import argparse
import math
#from entropy_graph import graph



def main():
   # parse CL args
   parser = argparse.ArgumentParser(description="Entropy data histogram graphing")
   parser.add_argument("-f", "--file", help="the data to clean", default="data.p")
   args = vars(parser.parse_args())
   filename = args["file"]

   # load data first
   f = pickle.load(open(filename, "rb"))



   #bits_of_entropy = 31 # TODO: Change this to be not manual
   #bytes_of_entropy = bits_of_entropy / 8.0
   high = 4
   low = 4

   # get each addr
   #with open(filename) as f:
   with open(filename+"-clean.csv", "w") as nf:

      # CSV HEADER
      nf.write("high,low\n")

      for line in f:
         line = hex(line)
         #print line
         hexnum = line.split("0x")[1]
         hexnum_len = len(hexnum)

         # LSB to low
         low_bytes = "0x" + hexnum[hexnum_len-low:hexnum_len]
         
         # low to high
         high_bytes = "0x" + hexnum[hexnum_len-low-high:hexnum_len-low]


         #print "orig: ", hexnum
         #print "high: ", high_bytes, " low: ", low_bytes

         if len(low_bytes) != len(high_bytes):
            print "Length mismatch! Need to adjust top calculations"
            exit()

         #write as CSV
         nf.write(high_bytes + "," + low_bytes+  "\n")
         





main()
