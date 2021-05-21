#!/usr/bin/python

import os
import sys
import random

if __name__ == "__main__":
	if len(sys.argv) != 3:
		print "%s <vertex number> <out vertex pair file>" % (sys.argv[0])
		sys.exit(-1)
	file = open(sys.argv[2], 'w')
	maxid = int(sys.argv[1])
	for i in range(1000000):
		file.write(str(random.randint(1, maxid)) + ' ' + str(random.randint(1, maxid)) +' ' + str(random.randint(0 ,1)) + '\n')
		
	file.close()
	sys.exit(0)

