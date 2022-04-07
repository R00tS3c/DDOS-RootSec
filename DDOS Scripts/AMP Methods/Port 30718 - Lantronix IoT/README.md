# Lantronix Gateway Discovery

## Port: 30718

## Proto: UDP

## Amplification factor: ~5x

## Reflector count: ~9,200

---

- Original Lantronix discovery is only a 30 byte response (see documentation) via the query `\0\0\0\xf6` to respond with packet prefix `\0\0\0\xf7`.
- However I have observed what appears to be `AlarmLock` Wireless Locks and other Physical Access Management systems that return system configuration bits and potential custom lock names.
  - Read the documentation PDFs for yourself and raise an `Issue` on this repo if you find anything else!

### Example Request / Response

- Request: 4 bytes

  - `> ~# echo -ne '\x00\x00\x00\xf8'|nc -u 50.246.41.242 30718 -w1`

- Response: 124 bytes

  - ASCII: `�xmenL'�L'�-` (xmen is lock name?)
  - Raw Bytes: `000000f90a01140a00000800786d656e0a0114014c0200001127000000000000c000000000000000000000000000000000000000000000000000000000000000000000004c0800001227000000000000c000002d00000000000000000000000000000000000000000000000000000000000000000000000000000000`

### Documentation

- Original Lantronix Response - <http://wiki.lantronix.com/developer/Lantronix_Discovery_Protocol](http://wiki.lantronix.com/developer/Lantronix_Discovery_Protocol>
- A sectalk on taking control of Lantronix devices - <https://www.slideshare.net/kost/vk-exploringtreasuresof77-fehlantronixconfidence2014>
- Vendor with internet accessible lantronix devices - <https://www.alarmlock.com/>
- "Trilogy Networx" is another vendor selling these - <https://www.gokeyless.com/pa_brand/trilogy-networx/>
- Software vendor that runs on these lantronix supported devices - <https://cicaccess.com/>
  - CA4K TCP/IP Ports https://tech.napcosecurity.com/index.php/techlibrary/downloadfile/id/2232

### Mitigations

- Stop allowing connectionless UDP on physical security devices please.
- Response data ALWAYS consists of lantronix headers `\x00\x00\x00\xf9`.
- Data will always be 30 or 124 bytes, `\xf7` mode is 30, `\xf9` is 124.
