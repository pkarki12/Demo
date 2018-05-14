#!/usr/bin/env python

import sys

def main():
	for line in sys.stdin:
		line = line.strip().split()
		for i in line[1:]:  # line[1:] contains all the friends
			for j in line[1:]:
				if i != j:
					print line[0] + " " + i + " " + j  # line[0] is the subject
					print i + " " + line[0] + " " + j  # Checks if friends are mutual

main()
