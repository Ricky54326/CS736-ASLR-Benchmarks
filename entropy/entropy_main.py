#!/usr/bin/python

"""entropy_main.py: Backend/parsing for our entropy tests for ASLR."""

__author__      = "Riccardo Mutschlechner <riccardo@cs.wisc.edu>, John Detter <jdetter@wisc.edu>"

import collections
import os
import subprocess

# From: http://www.python-course.eu/forking.php
cnt = collections.Counter()

def child():
   # print 'A new child ',  os.getpid( )
   proc = subprocess.Popen(['ls'], 
                        stdout=subprocess.PIPE,
                        )
   stdout_value = proc.communicate()[0]
   #print stdout_value

   # tokenize stdout from test and add to counter
   for str in stdout_value.split():
      cnt[str] += 1
   os._exit(0)

def parent():

   for i in range(1000):
      newpid = os.fork()
      if newpid == 0:
         child()
      else:
         pids = (os.getpid(), newpid)
         #print "parent: %d, child: %d" % pids
         os.wait()


   print cnt.most_common(10)


parent()