#!/bin/bash

usage()
{
cat <<EOF

Calculates PPS, BPS, and percentage of line-rate (LR).
from Linux kernel statistics by reading from procfs.

      Options:

      -i interface(s)	Interface or interfaces (sep:,) e.g. \`\`-i eth0,eth1,eth2''
      -t <int>		Time interval in seconds (def: 1)

EOF
}

argcheck() {
if [ $ARGC -lt $1 ]; then
  echo "Please specify an argument!, try $0 -h for more information"
  exit 1
fi
}

INTERVAL=1
END='\e[m'
RED='\e[0;31m'
BLUE='\e[0;34m'
ORANGE='\e[0;33m'
ARGC=$#

# Check for dependency filesystems
if ! [ -d /sys ] || ! [ -d /proc ]; then
  echo "$0 requires sysfs and procfs"
  exit $UNKNOWN
fi

# Print warning and exit if less than n arguments specified
argcheck 1

# option and argument handling
while getopts "hi:t:" OPTION
do
  case $OPTION in
    h)
      usage
      exit
      ;;
    i)
      INTERFACES=$(echo $OPTARG | sed 's/,/ /g')
      ;;
    t)
      INTERVAL=$OPTARG
      ;;
    *)
      usage
      exit
      ;;
  esac
done

echo -e "${BLUE}^C to exit${END}"

while true; do
  for INT in $INTERFACES; do
    # Get speed of NIC
    speed=$(cat /sys/class/net/$INT/speed)
    
    # Get number of packets for interface
    rxppsold=$(awk "/$INT/ "'{ sub(":", " "); print $3 }' /proc/net/dev)
    txppsold=$(awk "/$INT/ "'{ sub(":", " "); print $11 }' /proc/net/dev)
    
    # Get number of bytes for interface
    rxbytesold=$(awk "/$INT/ "'{ sub(":", " "); print $2 }' /proc/net/dev)
    txbytesold=$(awk "/$INT/ "'{ sub(":", " "); print $10 }' /proc/net/dev)
    
    sleep $INTERVAL
    
    # Get number of packets for interface again and subtract from old
    rxppsnew=$(awk -v rxppsold="$rxppsold" "/$INT/ "'{ \
    	sub(":", " "); rxppsnew = $3; print rxppsnew - rxppsold }' /proc/net/dev)
    txppsnew=$(awk -v txppsold="$txppsold" "/$INT/ "'{ \
    	sub(":", " "); txppsnew = $11; print txppsnew - txppsold }' /proc/net/dev)
    
    # Get number of bytes for interface again and subtract from old
    rxbytesnew=$(awk -v rxbytesold="$rxbytesold" "/$INT/ "'{ \
    	sub(":", " "); rxbytesnew = $2; print rxbytesnew - rxbytesold }' /proc/net/dev)
    txbytesnew=$(awk -v txbytesold="$txbytesold" "/$INT/ "'{ \
    	sub(":", " "); txbytesnew = $10; print txbytesnew - txbytesold }' /proc/net/dev)
    
    # Calculate percentage of line-rate from number of bytes per second.
    rxlinerate=$(($rxbytesnew / 125000 / $speed * 100))
    txlinerate=$(($txbytesnew / 125000 / $speed * 100))
    
    # Format line-rate values by truncating after the 1000th decimal place.
    rxlr=$(printf "%1.3f" $rxlinerate)
    txlr=$(printf "%1.3f" $txlinerate)
    
    # Print the results
    echo -e "${ORANGE}Int${END}: $RED${INT}$END ${BLUE}|${END} $ORANGE[$END${RED}RX${END}$ORANGE]$END ${ORANGE}PPS${END}: $RED${rxppsnew}$END ${BLUE}|${END} ${ORANGE}BPS${END}: $RED${rxbytesnew}$END ${BLUE}|${END} ${ORANGE}% of LR${END}: ${RED}$rxlr${END} ${BLUE}--${END} $ORANGE[$END${RED}TX${END}$ORANGE]$END ${ORANGE}PPS${END}: $RED${txppsnew}$RED ${BLUE}|${END} ${ORANGE}BPS${END}: ${RED}$txbytesnew${END} ${BLUE}|${END} ${ORANGE}% of LR${END}: ${RED}$txlr${END}"

  done
done
