# Ex: python me.py ips.txt 80 new.txt
import sys, os

port = int(sys.argv[2])
out_file = sys.argv[3]
ips = open(sys.argv[1], "r").readlines()

for ip in ips:
    ip = ip.strip("\r\n")
    ip = ip+":"+str(port)
    cmd = "echo '%s' >> %s" % (ip, out_file)
    os.system(cmd)