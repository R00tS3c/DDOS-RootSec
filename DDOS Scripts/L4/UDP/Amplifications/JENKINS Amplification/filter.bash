echo "[!] Filter started!"
for i in $(cat list)
do

echo "Scanning [$i]"


exec 3<>/dev/udp/$i/33848 && python -c "print '.'" >&3 && timeout 2 cat <&3 > /tmp/scan

echo "$i $(wc --bytes < /tmp/scan)" >> output
rm -rf /tmp/scan

done

echo "[!] Filter finished"



