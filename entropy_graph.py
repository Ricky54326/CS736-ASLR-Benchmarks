#!/usr/bin/python

__author__      = "Riccardo Mutschlechner <riccardo@cs.wisc.edu>, John Detter <jdetter@wisc.edu>"


import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

def graph(data):

	mu, sigma = np.mean(data), np.std(data)
	#x = mu + sigma*np.random.randn(10000)
	x = data
	#print x

	# the histogram of the data
	n, bins, patches = plt.hist(x, len(data),  facecolor='green', alpha=0.75)

	# add a 'best fit' line
	y = mlab.normpdf( bins, mu, sigma)
	l = plt.plot(bins, y, 'r--', linewidth=1)

	plt.xlabel('Address')
	plt.ylabel('# Accesses')
	plt.title('Histogram of Memory Locations:')
	plt.axis([np.min(data),np.max(data), 0, 10])
	plt.grid(True)

	plt.show()
