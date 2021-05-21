#!/usr/bin/python

import os
import sys
import random

if __name__ == "__main__":
	if len(sys.argv) != 4:
		print "%s <graph infile> <graph outfile> <vertex number>" % (sys.argv[0])
		sys.exit(-1)
	file = open(sys.argv[1], 'r')
	file2 = open(sys.argv[2], 'w')
	file2.write("graph_for_greach\n")
	file2.write(sys.argv[3]+'\n')
	list = []
	lastid = 0
	for line in file.readlines():
		list = line.split()
		for i in range(lastid+1, int(list[0])):
			file2.write(str(i)+': #\n')
		file2.write(list[0]+': ')
		lastid = int(list[0])
		for i in range(1, len(list)):
			file2.write(list[i]+' ')
		file2.write('#\n')
		
	file.close()
	file2.close()
	sys.exit(0)

