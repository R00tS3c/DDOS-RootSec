echo "[!] Filter started!"
for i in $(cat list)
do

echo "Scanning [$i]"


exec 3<>/dev/udp/$i/11211 && python -c "print '\0\x01\0\0\0\x01\0\0gets a b c d e f g h j k l m n o p q r s t w v u x y a\r\n'" >&3 && timeout 2 cat <&3 > /tmp/scan

echo "$i $(wc --bytes < /tmp/scan)" >> output
rm -rf /tmp/scan

done

echo "[!] Filter finished"



