#!/usr/bin/python
try:
	fr = open("resources.csv", "r")
	line=fr.readline()
	resources=line.replace(","," ").split()
	fr.close()

	fw= open("resources.csv", "w")
	fw.write(resources[0] + "," + resources[1] + ",0")
	fw.close

except IOError:
	print "error"
