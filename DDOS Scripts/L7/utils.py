# import modules
import os, requests, socket, socks
from random import randint, choice, shuffle, random
from string import ascii_letters, digits, ascii_uppercase
from urllib.parse import urlparse

ualist = []
with open('useragents.txt', 'r') as uafile:
    [ualist.append(line.strip('\n')) for line in uafile.readlines()]

reflist = []
with open('referers.txt', 'r') as reffile:
    [reflist.append(line.strip('\n')) for line in reffile.readlines()]

orlist = []
with open('open redirects.txt', 'r') as orfile:
    [orlist.append(line.strip('\n')) for line in orfile.readlines()]

keywords = []
with open('keywords.txt', 'r') as kwfile:
    [keywords.append(line.strip('\n')) for line in kwfile.readlines()]

accepts = [
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\n",
    "Accept-Encoding: gzip, deflate\r\n",
    "Accept-Language: en-US,en;q=0.5\r\nAccept-Encoding: gzip, deflate\r\n",
    "Accept: text/html, application/xhtml+xml, application/xml;q=0.9, */*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Charset: iso-8859-1\r\nAccept-Encoding: gzip\r\n",
    "Accept: application/xml,application/xhtml+xml,text/html;q=0.9, text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Charset: iso-8859-1\r\n",
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Encoding: br;q=1.0, gzip;q=0.8, *;q=0.1\r\nAccept-Language: utf-8, iso-8859-1;q=0.5, *;q=0.1\r\nAccept-Charset: utf-8, iso-8859-1;q=0.5\r\n",
    "Accept: image/jpeg, application/x-ms-application, image/gif, application/xaml+xml, image/pjpeg, application/x-ms-xbap, application/x-shockwave-flash, application/msword, */*\r\nAccept-Language: en-US,en;q=0.5\r\n",
    "Accept: text/html, application/xhtml+xml, image/jxr, */*\r\nAccept-Encoding: gzip\r\nAccept-Charset: utf-8, iso-8859-1;q=0.5\r\nAccept-Language: utf-8, iso-8859-1;q=0.5, *;q=0.1\r\n",
    "Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/webp, image/jpeg, image/gif, image/x-xbitmap, */*;q=0.1\r\nAccept-Encoding: gzip\r\nAccept-Language: en-US,en;q=0.5\r\nAccept-Charset: utf-8, iso-8859-1;q=0.5\r\n,"
    "Accept: text/html, application/xhtml+xml, application/xml;q=0.9, */*;q=0.8\r\nAccept-Language: en-US,en;q=0.5\r\n",
    "Accept-Charset: utf-8, iso-8859-1;q=0.5\r\nAccept-Language: utf-8, iso-8859-1;q=0.5, *;q=0.1\r\n",
    "Accept: text/html, application/xhtml+xml",
    "Accept-Language: en-US,en;q=0.5\r\n",
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Encoding: br;q=1.0, gzip;q=0.8, *;q=0.1\r\n",
    "Accept: text/plain;q=0.8,image/png,*/*;q=0.5\r\nAccept-Charset: iso-8859-1\r\n"
]

def parseurl(url):

    parsed = urlparse(str(url))

    return parsed

def randip():
    p1, p2, p3, p4 = str(randint(0, 256)),str(randint(0, 256)),str(randint(0, 256)),str(randint(0, 256))

    return f'{p1}.{p2}.{p3}.{p4}'

def randstr(strlen, chars=ascii_letters):
    return ''.join(choice(chars) for _ in range(strlen))

class HTTPAdapter(requests.adapters.HTTPAdapter):

    # stolen from stackoverflow xd
    def __init__(self, *args, **kwargs):
        self.socket_options = kwargs.pop("socket_options", None)
        super(HTTPAdapter, self).__init__(*args, **kwargs)

    def init_poolmanager(self, *args, **kwargs):
        if self.socket_options is not None:
            kwargs["socket_options"] = self.socket_options
        super(HTTPAdapter, self).init_poolmanager(*args, **kwargs)

def createsession():

    adapter = HTTPAdapter(socket_options=[
        (socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1), 
        (socket.IPPROTO_TCP, socket.TCP_KEEPIDLE, 5), 
        (socket.IPPROTO_TCP, socket.TCP_KEEPINTVL, 5), 
        (socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    ])

    session = requests.session()
    session.mount("http://", adapter)
    session.mount("https://", adapter)
    session.verify = False
    session.allow_redirects = False
    session.timeout = (5, 2)
    session.trust_env = False

    return session

def buildblock(url):
    '''
    Function to generate a block that gets appened to the target url
    '''
    
    block = '' if url.endswith('/') else ''

    block += randstr(randint(5, 10))
    for _ in range(randint(2, 10)):
        if randint(0, 1) == 1: block += f'/{randstr(randint(5, 10))}'
        else: block += f'/{randstr(randint(5, 10), chars=digits)}'
    
    block += f'?q={choice(keywords).replace(" ", "+")}'

    for _ in range(randint(2, 9)):
        if randint(0, 1) == 1: block += f'&{randstr(randint(5, 10))}={choice(keywords).replace(" ", "+")}'
        else: block += f'&{choice(keywords).replace(" ", "+")}={choice(keywords).replace(" ", "+")}'
    
    return block

def buildheaders(url):
    '''
    Function to generate randomized headers
    '''

    cache_controls = ['no-cache', 'max-age=0']
    accept_encodings = ['*', 'identity', 'gzip', 'deflate']
    accept_charsets = ['ISO-8859-1', 'utf-8', 'Windows-1251', 'ISO-8859-2', 'ISO-8859-15']
    content_types = ['multipart/form-data', 'application/x-url-encoded']
    accepts = ['*/*', 'application/json', 'text/html', 'application/xhtml+xml', 'application/xml', 'image/webp', 'image/*']
    # we shuffle em
    for toshuffle in [cache_controls, accept_encodings, accept_charsets, content_types, accepts]:
        shuffle(toshuffle)

    headers = {
        'User-Agent': choice(ualist),
        'Cache-Control': ', '.join(cache_controls[:randint(1,len(cache_controls))]),
        'Accept-Encoding': ', '.join(accept_encodings[:randint(1,len(accept_encodings))]),
        'Accept': ', '.join(accepts[:randint(1,len(accepts))])
    }

    # "random" headers
    if randint(0,1) == 1:
        charsets = f'{accept_charsets[0]},{accept_charsets[1]};q={str(round(random(), 1))},*;q={str(round(random(), 1))}'
        headers.update({'Accept-Charset': charsets})

    if randint(0,1) == 1:
        rand_referer = choice(reflist)
        headers.update({'Referer': rand_referer+buildblock(rand_referer)})
    
    if randint(0,1) == 1:
        headers.update({'Content-Type': choice(content_types)})
    
    if randint(0,1) == 1:
        headers.update({'Cookie': randstr(randint(60, 90), chars=ascii_uppercase+str(digits))})
    
    if randint(0,1) == 1:
        headers.update({'X-Forwarded-For': randip()})
    
    return headers