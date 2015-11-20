#!/usr/bin/python

__author__      = "Riccardo Mutschlechner <riccardo@cs.wisc.edu>, John Detter <jdetter@wisc.edu>"


import argparse
import cPickle as pickle
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

def graph(data):
	#data = [d - 140733193388032L for d in data]
	mu, sigma = np.mean(data), np.std(data)
	min_data, max_data = np.min(data), np.max(data)

	#x = mu + sigma*np.random.randn(10000)
	x = data#[9900:]
	#print x

	# the histogram of the data
	n, bins, patches = plt.hist(x, bins=len(x),facecolor='green', alpha=0.75)
	print bins[:10]
	#print 
	#n, bins, patches = plt.hist(x, 3,  facecolor='green', alpha=0.75)


	# add a 'best fit' line
	# y = mlab.normpdf( bins, mu, sigma)
	# l = plt.plot(bins, y, 'r--', linewidth=1)

	plt.xlabel('Address')
	plt.ylabel('# Accesses')
	plt.title('Histogram of Memory Locations:')
	plt.axis([np.min(data),np.max(data), 0, max(n)])
	plt.grid(True)

	params = np.linspace(np.min(data), np.max(data), num=3)
	axes = plt.gca()
	axes.get_xaxis().set_major_locator(ticker.FixedLocator(params))
	axes.get_xaxis().set_major_formatter(ticker.FormatStrFormatter("%x"))

	plt.show()


if __name__ == "__main__":

	# parse CL args
	parser = argparse.ArgumentParser(description="Entropy data histogram graphing")
	parser.add_argument("-f", "--file", help="the pickled data to graph", default="data.p")
	args = vars(parser.parse_args())
	filename = args["file"]

	# load data first
	data = pickle.load(open(filename, "rb"))

	# call graph() as usual
	graph(data)
