#!/usr/bin/env python

import sys

def main():
	current = None
	for line in sys.stdin:
		line = line.strip().split()
		if line == current:  # Checks if friends are mutual
			if int(line[1]) < int(line[2]):  # Makes sure friends are sorted
				print " ".join(str(i) for i in line)
		else:
			current = list(line)

main()
