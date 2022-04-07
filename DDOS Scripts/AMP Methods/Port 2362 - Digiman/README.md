# Digiman (Advanced Digi Discovery Protocol - ADDP)

## Port: 2362

## Proto: UDP

## Amplification factor: 5x

## Reflector count: ~9,000 (1/1/2020)

---

### Example request/response

- Request: 14 bytes
- > ~# echo -ne '\x44\x49\x47\x49\x00\x01\x00\x06\xff\xff\xff\xff\xff\xff' |nc -u 88.28.197.205 2362
	<pre>
	DIGI�X������X��
	Connect WAN 3G (RS232 serial)P:=�P:=�
	Version 82001532_M 01/03/2013@���j$�
	</pre>
- Response: 167 bytes

### Mitigations

- <https://lists.internet2.edu/sympa/arc/wg-multicast/2010-01/msg00000.html>

### Documentation

- <https://patents.google.com/patent/US7035257>
- <ftp://ftp1.digi.com/support/utilities/9300307A.txt>
