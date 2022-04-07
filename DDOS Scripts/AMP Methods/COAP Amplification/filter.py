#!usr/bin/python
# -*- coding: utf-8 -*-
import socket
import sys

if len(sys.argv) < 2:
    print '[!] EasyFilter 1.0 by Alemalakra.'
    print '[!] Usage: filter.py <INPUT> <OUTPUT> <PROTOCOL> <MIN BYTES> <OUTPUT SYNTAX>'
    print '[!] Protocols: chargen, ntp, quake, ssdp, ldap, dns, snmp, mdns, tftp, portmap, netbios, coap'
    print '[!] Output syntax variables: [space], [ip], [bytes]'
    sys.exit()

s = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
s.settimeout(0.05) # Fast list/scan of course.

# PayLoads.
chargen = 'A'#lemalakra
ntp = '\x17\x00\x03\x2a\x00\x00\x00\x00'
quake = '\xFF\xFF\xFF\xFF\x67\x65\x74\x73\x74\x61\x74\x75\x73\x10'
ssdp = "\x4d\x2d\x53\x45\x41\x52\x43\x48\x20\x2a\x20\x48\x54\x54\x50\x2f\x31\x2e\x31\x0D​\x0A\x48\x6f\x73\x74\x3a\x32\x33\x39\x2e\x32\x35\x35\x2e\x32\x35\x35\x2e\x32\x35​\x30\x3a\x31\x39\x30\x30\x0D\x0A\x53\x54\x3a\x73\x73\x64\x70\x3a\x61\x6c\x6c\x0D​\x0A\x4d\x61\x6e\x3a\x22\x73\x73\x64\x70\x3a\x64\x69\x73\x63\x6f\x76\x65\x72\x22​\x0D\x0A\x4d\x58\x3a\x33\x0D\x0A\x0D\x0A"
snmp = '\x30\x37\x02\x01\x01\x04\x06\x70\x75\x62\x6c\x69\x63\xa5\x2a\x02\x04\x06\x29\x07\x31\x02\x01\x00\x02\x01\x0a\x30\x1c\x30\x0b\x06\x07\x2b\x06\x01\x02\x01\x01\x01\x05\x00\x30\x0d\x06\x09\x2b\x06\x01\x02\x01\x01\x09\x01\x03\x05\x00'
dns = '\xc4\x75\x01\x00\x00\x01\x00\x00\x00\x00\x00\x01\x00\x00\xff\x00\x01\x00\x00\x29\x23\x28\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
ldap = '\x30\x84\x00\x00\x00\x2d\x02\x01\x01\x63\x84\x00\x00\x00\x24\x04\x00\x0a\x01\x00\x0a\x01\x00\x02\x01\x00\x02\x01\x00\x01\x01\x00\x87\x0b\x6f\x62\x6a\x65\x63\x74\x63\x6c\x61\x73\x73\x30\x84\x00\x00\x00\x00\x00'
mdns = '\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x09\x5F\x73\x65\x72\x76\x69\x63​\x65\x73\x07\x5F\x64\x6E\x73\x2D\x73\x64\x04\x5F\x75\x64\x70\x05\x6C\x6F\x63\x61​\x6C\x00\x00\x0C\x00\x01'
tftp = '\x00\x01\x2f\x78\x00\x6e\x65\x74\x61\x73\x63\x69\x69\x00'
portmap = '\x65\x72\x0A\x37\x00\x00\x00\x00\x00\x00\x00\x02\x00\x01\x86\xA0\x00\x00\x00\x02​\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00​'
netbios = '\xE5\xD8\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x20\x43\x4B\x41\x41\x41\x41\x41​\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41\x41​\x41\x41\x41\x41\x41\x00\x00\x21\x00\x01'
coap = '\x40\x01\x7d\x70\xbb\x2e\x77\x65\x6c\x6c\x2d\x6b\x6e\x6f\x77\x6e\x04\x63\x6f\x72\x65'

file1 = sys.argv[1] #Input
file2 = sys.argv[2] #Output
proto = sys.argv[3]
size = int(sys.argv[4])
syntax = sys.argv[5]
with open(file1) as f:
    list = f.read().splitlines()
newfile = open(file2, 'w')
if proto == 'tftp':
    port = 69
    payload = tftp
elif proto == 'portmap':
    port = 111 
    payload = portmap
elif proto == 'netbios':
    port = 137
    payload = netbios
elif proto == 'ntp':
    port = 123
    payload = ntp
elif proto == 'mdns':
    port = 5353
    payload = mdns
elif proto == 'chargen':
    port = 19
    payload = chargen
elif proto == 'snmp':
    port = 161
    payload = snmp
elif proto == 'dns':
    port = 53
    payload = dns
elif proto == 'quake':
    port = 27960
    payload = quake
elif proto == 'ssdp':
    port = 1900
    payload = ssdp
elif proto == 'ldap':
    port = 389
    payload = ldap
elif proto == 'coap':
    port = 5683
    payload = coap
else:
    print '[!] Protocol is not available'
    sys.exit()
# Start Loop
c = 0
servers = []
while c < len(list):
    split = list[c].split(" ") # Bytes ones, and non bytes ones
    s.sendto(payload, (split[0], port))
    try:
        data, addr = s.recvfrom(65500)
        syntax_output = ""
        if len(data) >= size:
            if any(addr[0] in s for s in servers):
                # Already Exists
                rip = 0
            else:
                # Add so.
                print '[!] Recived Working Server: %s Recived Bytes: %i' % ( addr[0],len(data) )
                syntax_output = syntax.replace("[space]", " ", 5) # 5 is fine.
                syntax_output = syntax_output.replace("[bytes]", str(len(data)), 5) # 5 is fine.
                syntax_output = syntax_output.replace("[ip]", str(addr[0]), 5) # 5 is fine.
                newfile.write(str(syntax_output)+'\n')
                servers.append(addr[0])
    except Exception and socket.error and socket.timeout:
        I = 0
    c += 1

newfile.close()
with open(file2) as e:
    count = e.read().splitlines()
print '[!] Finished Filter saved to %s with %i working servers.' % ( file2,len(count) )
