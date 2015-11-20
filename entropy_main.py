#!/usr/bin/python

"""entropy_main.py: Backend/parsing for our entropy tests for ASLR."""

__author__      = "Riccardo Mutschlechner <riccardo@cs.wisc.edu>, John Detter <jdetter@wisc.edu>"

import collections
import os
import cPickle as pickle
import subprocess
#from entropy_graph import graph

# From: http://www.python-course.eu/forking.php
cnt = collections.Counter()

def entropy1():
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


      #print cnt.most_common(10)

   asints = []

   for key, val in cnt.iteritems():
	# print "DEBUG: ", key, " ", val
   	for i in range(val):
            #print long(key, 16),
            asints.append(long(key, 16))
   
   # print "DEBUG: len():", len(cnt) 
   #serialize data
   pickle.dump(asints, open("entropy1.p", "wb"))

   #graph(asints)

def entropy2():
   for i in range(10000):
      proc = subprocess.Popen(['./entropy2'], 
                        stdout=subprocess.PIPE,
                        )

      stdout_value = proc.communicate()[0]
      #print stdout_value

      # tokenize stdout from test and add to counter
      for str in stdout_value.split():
         #print "adding ", str, " to counter"
         cnt[str] += 1




   #print cnt.most_common(10)
   asints = []
   for key in cnt.keys():
      asints.append(long(key, 16))
   
   #serialize data
   pickle.dump(asints, open("entropy2.p", "wb"))

   #graph(asints)

def entropy3():
   for i in range(10000):
      proc = subprocess.Popen(['./entropy3'], 
                        stdout=subprocess.PIPE,
                        )

      stdout_value = proc.communicate()[0]
      #print stdout_value

      # tokenize stdout from test and add to counter
      for str in stdout_value.split():
         #print "adding ", str, " to counter"
         cnt[str] += 1


      #print cnt.most_common(10)

   asints = []

   for key in cnt.keys():
      asints.append(long(key, 16))
   
   #serialize data
   pickle.dump(asints, open("entropy3.p", "wb"))

   #graph(asints)

def main():
   #print "Starting entropy1"
   entropy1()

   #print "Starting entropy2"
   entropy2()

   #print "Starting entropy3"
   entropy3()

main()
