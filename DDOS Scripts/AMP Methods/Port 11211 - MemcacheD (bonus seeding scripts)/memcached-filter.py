# Mem filter by Phenomite
# pip install --pre scapy[basic]
#
# Initial idea
# - Initial scan all "stats items" and dumps all data from IPs in list to a file or array
#   - loop over items numbers for each ip to get an array of item keys (plaintext regex extraction?)
#       - loop over items in each item <key> using cachedump
#       - extract item name and size
#           - if size > specified argv[] limit
#           - or if largest item
#               - add to filter list with format "ip:key:size\n" or "ip key\n"?

import os
import sys
import socket
import re
import time
import threading
import requests
import memcache
from time import sleep
from scapy.all import *

print_lock = threading.Lock()
exit = False
memcachePort = 11211
qry_get_items = '\00\01\00\00\00\01\00\00stats items\r\n'
# Top 500 items, reduce return latency
qry_dump_cache = '\00\01\00\00\00\01\00\00stats cachedump {0} 300\r\n'
qry_dump_item = '\00\01\00\00\00\01\00\00get {0}\n'


def stats_items(ip):
    # Grabs the list of all items in the memcache server
    global exit
    search_items = []
    try:
        # E.g.
        # STAT items:27:number 1
        # STAT items:37:number 1
        # Seed server first
        print("Getting items from: " + ip)
        # data = mc.get_stats("items")
        data = sendData(ip, qry_get_items)
        # print(data)
        # E.g.
        # Would return ['27', '37']
        search_items = unique(re.findall(
            r'STAT items:(.*?):', data, re.M + re.I))
        print(str(search_items))
    except socket.error as e:
        # If no data is received, you get here, but it's not an error
        # Ignore and continue
        print("\nError occurred while querying: " + str(e))
        pass
    except UnicodeDecodeError:
        # Ignore non-utf8 responses and attempts at killing decoders (potential pots, unlikely though)
        pass
    except KeyboardInterrupt:
        exit = True
    return search_items


def stats_dump_keys(ip, array_items, min_size, max_items):
    global exit
    keys = {}
    best_keys = ""
    # Iterate over all items in their table (that we got back)
    for item in array_items:
        try:
            print("\nDumping item " + item)  # + " from: " + ip
            # Set query to dump list of item keys
            data = sendData(ip, qry_dump_cache.format(item))
            for keyline in data.splitlines():
                keyline = keyline.rstrip()  # Clean string
                if len(keyline) > 3:  # Avoid "END"
                    key_name = re.search(r'ITEM (.*) \[', keyline)
                    if key_name:  # Matched
                        # The group we want, 0 is full match, 1 is the subgroup
                        key_name = key_name.group(1)
                        key_size = re.search(r'\[(\d+) b; \d+ s\]', keyline)
                        if key_size:
                            # The group we want, 0 is full match, 1 is the subgroup
                            key_size = key_size.group(1)
                            if int(key_size) >= int(min_size):  # Initial size filter
                                keys[key_name] = int(key_size)
                                print("Key: " + str(key_name) +
                                      " -- Size: " + str(key_size))
                    else:
                        print(
                            "\nDidnt match string when I should have, fragment data was this: " + str(keyline))

        except UnicodeDecodeError:
            # Ignore non-utf8 responses and attempts at killing decoders (potential pots, unlikely though)
            pass
        except Exception as e:
                # If no data is received, you get here, but it's not an error
                # Ignore and continue
            print("\nError occurred while querying: " + str(e))
            pass
        except KeyboardInterrupt:
            exit = True
    if len(keys) > int(max_items):  # Limit amount of keys
        # Sort on each item's size and strip the top of the list to a limit of max items
        sorted_keys = sorted(
            keys.items(), key=lambda x: int(x[1]), reverse=True)
        extractor = []
        for k, s in sorted_keys[:int(max_items)]:
            extractor.append(k)
        print(str(extractor))
        best_keys = " ".join(extractor)
    elif len(keys):
        extractor = []
        for k, s in keys.items():
            extractor.append(k)
        best_keys = " ".join(extractor)
    # Throw string back to our main() and let it handle empty
    return best_keys


def get_best_items(ip, string_items, min_size):
    global exit
    overall_best_item = ""
    size_of_last_best_item = 0
    iterator = 0
    for item in string_items.split(" "):  # Bad yes but bite me
        iterator = iterator + 1
        try:
                # Set query to dump list of item keys
                print("\nItem: " + item)
                mc = memcache.Client([ip], debug=False)
                memcache_item_value = mc.get(item)

                item_size_returned = len(memcache_item_value)
                print("Reported size of item: " + str(item_size_returned))
                # item_size_returned = len(sendData(ip, qry_dump_item.format(item)))
                # print("Item size: " + str(item_size_returned))

                # Check the returned size is over our min threshold
                if item_size_returned >= int(min_size):  # and (item_size_returned > size_of_last_best_item)) or (len(item) < len(overall_best_item) and item_size_returned == size_of_last_best_item):
                    size_of_last_best_item=item_size_returned  # Int to store last largest item
                    overall_best_item += item + " "  # Set return val
                    print("Added this item to current best key combo for this IP")

            except UnicodeDecodeError:
                # Ignore non-utf8 responses and attempts at killing decoders (potential pots, unlikely though)
                pass
            except Exception as e:
                # If no data is received, you get here, but it's not an error
                # Ignore and continue
                print("\nError occurred while querying: " + str(e))
                pass
            except KeyboardInterrupt:
                exit=True
    if overall_best_item:
        return overall_best_item.strip()
    else:
        return ""


def unique(list_in):
    list_set=set(list_in)
    # convert the set to the list
    unique_list=(list(list_set))
    new_list_of_uniques=[]
    for x in unique_list:
        new_list_of_uniques.append(x)
    return new_list_of_uniques


def sendData(host, data):
    # global exit
    # response = sr1(IP(dst=host)/UDP(sport=55000,dport=memcachePort)/Raw(load=data),timeout=2)
    # if response == None:
    #     return ""
    # else:
    #     return str(response.payload)

    txSocket=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    # txSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    txSocket.setblocking(0)
    address=(host, memcachePort)
    data_to_send=str.encode(data)
    send=txSocket.sendto(data_to_send, address)
    sleep(0.6)
    # Attempt to receive the resp from the server
    data, addr=txSocket.recvfrom(65000)
    # sleep(0.1)
    return str(data.decode('utf8'))


def main(fileInput, fileOutput, min_size, max_items):
    global exit
    # Dict of ip and items to send to file
    dict_ips={}
    counter_lines=0

    with open(fileInput, 'r') as ips:
        for ip in ips:
            print("\n---------New-IP---------")
            ip=ip.rstrip()  # Clean random DOS CRLF cancer
            counter_lines += 1  # Store total processed for print at the end
            try:
                # Get items on the IP (within response time constraints [hardcoded for now])
                ip_items=stats_items(ip)
                if not ip_items:
                    continue  # Get the non responders out of here

                # Now process each item returned and gather items and sizes to compare
                ip_keys=stats_dump_keys(ip, ip_items, min_size, max_items)
                if not ip_keys:
                    continue  # Get the tiny responses out of here

                # Iterate over items and actually grab the item to see if it will respond
                # Return items that exceed min_size specified
                items_size_passed=get_best_items(ip, ip_keys, min_size)
                if not items_size_passed:
                    continue  # Get the fake items out of here

                # for ip, keys in dict_ips:
                print("\nWill write this to file: " +
                      str(ip) + "\t" + str(items_size_passed))
                f=open(fileOutput, "a+")
                f.write(str(ip) + "\t" + str(items_size_passed) + "\n")
                f.close()
                print()  # Spacer
            except Exception as e:
                print(str(e))  # debug
            except KeyboardInterrupt:
                exit=True

                break
       # print(str(dict_ip_items))

    print("\n---------Done---------")
    print("IPs with gathered ITEMS: " + str(len(dict_ips)))
    print("IPs queried total: " + str(counter_lines))
    print("\nFinished script\n")
    # Initial iterator to process file and return dictionary with {ip:[items]}
    # TODO Create the queue and threader


if __name__ == '__main__':
    print("-- MemcacheD Filter -- Phenomite --")
    if len(sys.argv) < 3:
        print("Argument 1: Input filename containing one UDP memcache responder IP per line!\n\tAND DONT USE CRLF YOU WINDOWS FOOL. `dos2unix inputfile` if you are unsure.")
        print("Argument 2: Output filename")
        print("Argument 3: Minimum size (to ignore less than)")
        print("Argument 4: If multiple keys exceed minimum size, how many to add to list. Will order in largest size key first.")
        quit()
    main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4])
