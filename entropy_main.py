#!/usr/bin/python

"""entropy_main.py: Backend/parsing for our entropy tests for ASLR."""

__author__      = "Riccardo Mutschlechner <riccardo@cs.wisc.edu>, John Detter <jdetter@wisc.edu>"

import collections
import os
import subprocess
from entropy_graph import graph

# From: http://www.python-course.eu/forking.php
cnt = collections.Counter()

def main():

   for i in range(10000):
      proc = subprocess.Popen(['./entropy1'], 
                        stdout=subprocess.PIPE,
                        )

      stdout_value = proc.communicate()[0]
      #print stdout_value

      # tokenize stdout from test and add to counter
      for str in stdout_value.split():
         #print "adding ", str, " to counter"
         cnt[str] += 1


   print cnt.most_common(10)

   asints = []
   for key in cnt.keys():
	asints.append(int(key, 16))
	
   graph(asints)


main()
