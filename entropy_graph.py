#!/usr/bin/python

__author__      = "Riccardo Mutschlechner <riccardo@cs.wisc.edu>, John Detter <jdetter@wisc.edu>"


import cPickle as pickle
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

def graph(data):

	mu, sigma = np.mean(data), np.std(data)
	#x = mu + sigma*np.random.randn(10000)
	x = data
	#print x

	# the histogram of the data
	n, bins, patches = plt.hist(x, len(data),  facecolor='green', alpha=0.75)
	#n, bins, patches = plt.hist(x, 3,  facecolor='green', alpha=0.75)


	# add a 'best fit' line
	y = mlab.normpdf( bins, mu, sigma)
	l = plt.plot(bins, y, 'r--', linewidth=1)

	plt.xlabel('Address')
	plt.ylabel('# Accesses')
	plt.title('Histogram of Memory Locations:')
	plt.axis([np.min(data),np.max(data), 0, 10])
	plt.grid(True)

	# axes = plt.gca()
	# axes.get_xaxis().set_major_locator(ticker.MultipleLocator(1))
	# axes.get_xaxis().set_major_formatter(ticker.FormatStrFormatter("%x"))

	plt.show()


if __name__ == "__main__":

	# load data first
	data = pickle.load(open("data.p", "rb"))

	# call graph() as usual
	graph(data)