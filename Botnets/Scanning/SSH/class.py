#!/usr/bin/python -w
from random import randint
import time

range = raw_input("Range: ")

def host():
    return '%s.255.%i.%i' % (range, rand(), rand())

def rand():
    return randint(1, 254)
	
while True:
	print 'scanning %s' % (host())