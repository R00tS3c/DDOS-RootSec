import subprocess, datetime, random, socket, time, sys, os, re
from multiprocessing import Process
import binascii

listnerOutputFile = 'listener.tmp'
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
os.system('clear')
transmits = 1
timeout = 4

try:
    targetHost = sys.argv[1]
    targetPort = int(sys.argv[2])
    targetPayloadArg = bytes(' '.join(sys.argv[3:]), 'utf-8')
    targetPayload = targetPayloadArg.decode('unicode-escape').encode('utf-8')
except:
    print("Usage: python3 {0} <ip> <port> <payload>".format(sys.argv[0]))
    print()
    print("""Examples: - python3 {0} 1.1.1.1 123 '\\x17\\x00\\x03\\x2a\\x00\\x00\\x00\\x00'""")
    print("""          - python3 {0} 1.1.1.1 37810 'y'""")
    print()
    print("NOTE: Please make sure you put quotes around your payload!")
    print()

targetPayloadHex = str(binascii.hexlify(targetPayload)).strip("'b").upper()
payloadSize = len(targetPayload)
payloadSizeReal = 28 + len(targetPayload)

def printError(error):
    print("Error: {0}".format(error))
    exit(0)

def dataListener():
    try:
        server_address = ('0.0.0.0', listeningPort)
        s.bind(server_address)
        print("Listneing on port  -  {0}".format(listeningPort))
        with open(listnerOutputFile, 'w', 1) as file:
            while True:
                data, address = s.recvfrom(8192)
                file.write("{0},{1},{2}\n".format(address[0], address[1], len(data)))
    except Exception as e:
        printError("Failed binding to listening port")

def dataSender(line):
    ip = re.findall(r'(?:\d{1,3}\.)+(?:\d{1,3})', line)
    vector = ip[0]
    if vector:
        for _ in range(transmits):
            s.sendto(targetPayload, (vector, targetPort))
            if transmits > 1:
                time.sleep(1 / transmits)
        print("Sent payload       -  {0}:{1}".format(vector, targetPort))
        time.sleep(.01)

def killListener():
    try:
        listener.kill()
    except:
        listener.terminate()

def cleanFiles():
    fileList = listnerOutputFile
    for file in fileList:
        try:
            os.remove(file)
        except:
            pass

#Random listening port
listeningPort = random.randint(1024, 65500)

#Starting UDP listener thread
listener = Process(target = dataListener)
listener.start()
time.sleep(1)

#Send data to target, sleep, then kill listener thread
dataSender(targetHost)
print('=========================================================')
for x in range(timeout):
    timeLeft = timeout - x
    print("Sleeping for {0} more seconds...".format(timeLeft), end="\r")
    time.sleep(1)
print('\r                                        ')
killListener()

ampData = []
packetCount = 0
sizeCount = 0
sizeCountReal = 0
with open(listnerOutputFile, 'r') as results:
    for line in results.readlines():
        ip, port, size = line.split(',')
        if ip == targetHost:
            packetCount += 1
            sizeCountReal += (int(size) + 28)
            sizeCount += int(size)

amplificationFactorReal = sizeCountReal // payloadSizeReal
amplificationFactor = sizeCount // payloadSize
print("                ***  Vector results ***                 ")
print()
print('=========================================================')
print("Vector host:            {0}".format(targetHost))
print("Vector port:            {0}".format(targetPort))
print('=========================================================')
print("Payload:                {0}".format(str(targetPayload)[2:][:-1]))
print("Payload (hex):          {0}".format(targetPayloadHex))
print("Payload size:           {0} bytes".format(payloadSize))
print("Payload size (real):    {0} bytes".format(payloadSizeReal))
print('=========================================================')
print("Packets recieved:       {0}".format(packetCount))
print("Data recieved:          {0} bytes".format(sizeCount))
print("Data recieved (real):   {0} bytes".format(sizeCountReal))
print('=========================================================')
print("Amplification:          {0}x".format(amplificationFactor))
print("Amplification:          100 Mbit/s  -->  {0} Mbit/s".format(100 * amplificationFactor))
print('=========================================================')
print("Amplification (real):   {0}x".format(amplificationFactorReal))
print("Amplification (real):   100 Mbit/s  -->  {0} Mbit/s".format(100 * amplificationFactorReal))
print('=========================================================')
print("Coded by @oParoxysm :)")
print()

#Cleaning garbage
cleanFiles()
