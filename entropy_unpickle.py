#!/usr/bin/python

"""entropy_unpickle.py: unpickle data for R graphing."""

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
   parser = argparse.ArgumentParser(description="Entropy data histogram cleaning")
   parser.add_argument("-f", "--file", help="the data to clean", default="data.p")
   args = vars(parser.parse_args())
   filename = args["file"]

   # load data first
   f = pickle.load(open(filename, "rb"))

   # get each addr
   #with open(filename) as f:
   with open(filename.replace(".txt", "")+"-unpickled.txt", "w") as nf:
      for line in f:
         nf.write(str(line) + "\n")



main()
