[ $# -eq 0 ] && { echo "Usage: $0 90 10 10 0-255 THREADS OUTPUT";echo "scanning the ip range 90 would be 90 0-255 0-255 0-255"; exit 1; }
[ $# -eq 1 ] && { echo "Forgot Ip range"; exit 1; }
[ $# -eq 2 ] && { echo "Forgot Ip range"; exit 1; }
[ $# -eq 3 ] && { echo "Forgot Ip range"; exit 1; }
[ $# -eq 4 ] && { echo "Forgot threads"; exit 1; }
[ $# -eq 5 ] && { echo "Forgot output list :/"; exit 1; }
cat motd
nmap -oG - -T4 -p179 -v $1.$2.$3.$4 --min-parallelism $threads -host_timeout $timeout | grep "Ports: 179/filtered/tcp//bgp///" > tmp
grep -o '[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}' tmp > tmp2
sed -e 's/$/ 179/' -i tmp2
cp tmp2 $6
rm -rf tmp*
killall -9 nmap
echo "Saved as $6"
