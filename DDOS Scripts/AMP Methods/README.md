## Research on exotic UDP/TCP amplification vectors, payloads and mitigations

**The subfolders in this repository will contain the following:**

* Overview README.md
  * Name, Ports, Amplification factors, Update Info
  * Request <> Response Example with test IP (netcat yay!)
  * Potential official documentation
  * Potential mitigation strategies
* The raw payload (e.g. for use in zmap) OR potential scanning script (C).
* Raw socket flood script (C) for analysis to build flowspec or ACL mitigations.

## Who referenced this repository (Kudos!)

* Honeypot research shows variety of DDoS amplification methods (SRLabs 2021-07-30) - <https://www.srlabs.de/bites/honeypot-research-shows-variety-of-ddos-amplification-methods>
* DHCPDiscover Reflection/Amplification DDoS Attack Mitigation Recommendations | NETSCOUT (2021-07-07) - <https://www.netscout.com/blog/asert/dhcpdiscover-reflectionamplification-ddos-attack-mitigation>
* Powerhouse VPN Servers being abused (2021-02-22) - <https://www.zdnet.com/article/powerhouse-vpn-products-can-be-abused-for-large-scale-ddos-attacks/> 
* DVR reflection abused against Azure R6 and Ark Evolved servers (2021-02-04) - <https://azure.microsoft.com/en-au/blog/azure-ddos-protection-2020-year-in-review/>
* MS-RDPEUDP Scanning has begun by The Shadowserver Foundation (2021-01-25) - <https://www.shadowserver.org/news/scanning-for-accessible-ms-rdpeudp-services/>

## What is "amplification" in respect to Denial of Service? Give me an Example!

Amplification is where well-formed or malformed socket or application data requests elicit a response larger than the input data. This can then be abused to "amplify" a request, usually by means of Distributed Reflected Denial of Service (DRDoS) attacks. This distinction is usually lumped under the one banner of "DDoS"; however the former indicates that the traffic does not directly come from bots or single servers but is reflected off of usually benign services, thus typically rendering blacklists and simple firewall solutions useless.

Best way to show what this means is using the network protocol MSSQL over TCP/IP UDP port 1434 as an example.

### Example UDP response size from **1 byte** to a MSSQL (Microsoft SQL Server) listener

> `echo -ne '\x02' | nc -u -q 2 190.xx.xx.xx 1434|xxd -p|wc -c`
	<pre>629 bytes</pre>

That's an amplification factor of over **23** times.

### Example hex response from a discovery probe to an ARD (Apple Remote Desktop) listener

> `echo -ne '\x00\x14\x00\x01\x03' |nc -u 89.xx.xx.xx 3283|hexdump`
    <pre>
	0000000 0100 ea03 3100 0000 0000 0000 0000 0000
	0000010 0000 0000 0000 0000 0000 0000 0000 0000
	0000020 0000 0000 0000 0000 0100 0000 0000 0000
	0000030 0000 0000 0000 0000 0000 0000 0000 0000
	_
	0000050 0000 1200 0000 0000 0000 0000 0000 0000
	0000060 0000 0000 0000 0000 0000 0000 0000 0000
	0000070 0000 0000 0000 0000 0000 0000 0000 640a
	0000080 7461 6861 6565 6472 0034 0000 0000 0000
	0000090 0000 0000 0000 0000 0000 0000 0000 0000
	_
	00000c0 0000 0001 0000 0000 0000 0000 0000 0000
	00000d0 0000 0000 0000 9803 0000 0100 18f0 ed98
	00000e0 9288 0000 0000 0a00 6400 6100 7400 6100
	00000f0 6800 6500 6500 7200 6400 3400 0000 0000
	0000100 0000 0000 0000 0000 0000 0000 0000 0000
	</pre>

## Compiling the C code in this repo?

General C scripts:

```bash
gcc -pthread -O2 -o binary file.c
```

TCP scripts (requires 32bit compilation to avoid invalid checksum function return values):

```bash
gcc -m32 -pthread -O2 -o binary file.c
```

## Vulnerable reflectors

This repo is here to help everyone mitigate amplification vectors that have yet to be abused or are being actively abused with little related or consolidated information.


* Examples of cases include:
  * Infected hosts that need to be quickly added to a blacklist to get the attention of network owners.
  * Protocols that are devastating (e.g. MemcacheD) and require publicized lists to blackhole or to bulk contact network owners.
  * Because shodan already has you.
