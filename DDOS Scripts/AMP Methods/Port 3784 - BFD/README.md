# BFD Control (Diagnostic)

## Port: 3784

## Proto: UDP

## Amplification factor: ~5x

## Reflector count: ~700

---

- Bidirectional Forwarding Detection (BFD) is used to rapidly detect path transit failures, RFC 5880 is the standard and RFC 5881 is the adendum for IPv4/v6 network layer.
  - It's control packets are often used to replace tranditional slower failure detection methods such as Open Shortest Path First (OSPF)'s implementation.

- BFD itself (RFC 5880) is not at fault for CWE-406, the BFD encapsulation over IPv4/v6 with UDP (RFC 5881) is.

- Vulnerable to CWE-406 as per usual in this repo.

### Example Requests / Responses

- Request: **36 data bytes**

  - ASCII: `nothing human readable`

  - `> ~# echo -ne '\x56\xc8\xf4\xf9\x60\xa2\x1e\xa5\x4d\xfb\x03\xcc\x51\x4e\xa1\x10\x95\xaf\xb2\x67\x17\x67\x81\x32\xfb\x57\xfd\x8e\xd2\x22\x72\x03\x34\x7a\xbb\x98'|nc -u 70.51.197.238 3784 -w2`

- Response: **262 bytes**, **340 bytes average**

  - ASCII character space: `nothing human readable and it's random on every request`

  - Raw bytes (example only because of random discriminator): `77e409fced58ee2530d90d765350d1d92f3c850c29539da252a64ca94c8125ca46a35ae15d3f33278fe58323e19109786c9b448f7ac54c73308d10e7b3cab2b8fda886f84b2df5c6baf7fd0bbf16e787060a0cc3d4987367aa7d0fddbd17e627a1333392d7c91c4ea919a6b61610d1853ef4db4004ea6bee906ec660de231a1270cc9c10893988b7ea9c0ebe1e608e48f93af000467ca8e0983861b2516a2bcb3dca8728003af317659c25fdd98605686193397a72a62d401a90efae62807545dc7143d70f45c4e3999ec506b0fcacc0f7d81899b7701973bfb511c1f0551e5fdb8f4eb7eac84b85dd51d8e01823d8c026e9bc0ddfa14fe68460ca84de360ce04f9134262dda`

### Documentation

- RFC 5880: <https://tools.ietf.org/html/rfc5880>
- RFC 5881: <https://tools.ietf.org/html/rfc5881>
- Learn about BFD's use: <https://www.cisco.com/en/US/technologies/tk648/tk365/tk480/technologies_white_paper0900aecd80244005.html>

### Mitigations

- Tricky due to the randomisation done on the discriminator payload, you can map out the BFD Control message header and detect specific bytes, however by blocking it you may unintentially break some applications and networks that rely on it for IP link failover.
- Also to note BFD echo occurs on port 3785, not 3784.
