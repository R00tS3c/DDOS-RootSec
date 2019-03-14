#!/usr/bin/python
import sys, time, os, ssl, socket
from threading import Thread
if len(sys.argv) < 3:
	print "Usage: python "+sys.argv[0]+" <input> <threads> <output>"
	sys.exit()
ips = map(lambda s: s.strip(), open(sys.argv[1], "r").readlines())
threads = int(sys.argv[2])
thread_count = len(ips) / threads
thread_chunks = [ips[x:x+thread_count] for x in xrange(0, len(ips), thread_count)]
output = sys.argv[3]
found = 0
cons = 0
fails = 0
proc = 0
port = 81
buf = 4096

headers = "GET /ftptest.cgi HTTP/1.0\r\n\r\n"

def checkhost_headers(host):
    global found
    global cons
    global fails
    host = host.strip("\n")
    cons += 1
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            sock.settimeout(5)
            sock.connect((host, port))
        except:
            failed += 1
            sock.close()
        sock.send(headers)
        time.sleep(2)
        resp = sock.recv(1024)
        if "GoAhead-Webs" in resp:
            file = open(output, "a+")
            file.write(host+"\n")
            file.close()
            found += 1
        sock.close()
        cons -= 1
    except:
        cons -= 1
        fails += 1
        pass
def worker(count):
    global cons
    global failed
    global sent
    global proc
    global cons
    count = int(count)
    for i in thread_chunks[count]:
        try:
            proc += 1
            checkhost_headers(i)
        except:
            pass
for x in xrange(threads):
    try:
        t = Thread(target=worker, args=(x,))
        t.start()
    except KeyboardInterrupt:
        sys.exit()
    except:
        pass
while True:
    try: #prints the information like how many devices its tried
        i = found
        sys.stdout.write("\r\033[33mProccessed \033[92m[\033[93m"+str(proc)+"\033[92m]\033[33m || \033[33mFound \033[92m[\033[93m"+str(i)+"\033[92m]\033[33m || Conns \033[92m[\033[93m"+str(cons)+"\033[92m] || Fails \033[92m[\033[93m"+str(fails)+"\033[92m]\033[0m")
        sys.stdout.flush()
        time.sleep(0.25)
    except KeyboardInterrupt:
        sys.exit("Exiting On User Input")
    except:
        pass
