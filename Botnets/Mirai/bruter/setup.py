# little mirai-bruter dependency installer made by scarface for all the skids

import subprocess,os,sys,random,time,base64,subprocess

def run(cmd):
    subprocess.call(cmd, shell=True)

print("Updating system")
time.sleep(5)

print("System updated")
print ("Installing zmap && other shit")
time.sleep(5)
run ("apt-get install zmap -y")

print ("Installing stuff to compile zmap on centos")
time.sleep(5)
run ("yum install gcc cmake gmp gmp-devel libpcap-devel gengetopt byacc flex -y")

run ("yum install json-c-doc.noarch json-c.i686 json-c.x86_64 json-c-devel.i686 json-c-devel.x86_64 -y")

run ("yum install epel-release -y")

run ("um install gengetopt -y")

print ("Downloading zmap source & compiling it")
time.sleep(5)
run ("wget https://github.com/zmap/zmap/archive/v2.1.0.tar.gz")

run ("tar -xvf v2.1.0.tar.gz")

run ("cd zmap-2.1.0")

run ("flex -o \"src/lexer.c\" --header-file=\"src/lexer.h\" \"src/lexer.l\"")

run ("byacc -d -o \"src/parser.c\" \"src/parser.y\" ")

run ("mkdir /etc/zmap")

run ("cp conf/* /etc/zmap")

run ("cmake -DENABLE_HARDENING=ON")

run ("make")

run ("make install")

print ("All set")
time.sleep(5)