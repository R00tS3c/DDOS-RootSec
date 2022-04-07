# Bittorrent DHT

## Port: 6881

## Proto: UDP

## Amplification factor: 3x

## Reflector count: ~2,100,000

---

This protocol uses bencode over UDP to track magnet links and is obviously volatile due to the nature of how DHT makes trackers out of peers (e.g. Home connections). For example:

- After 24 hours a scan was re-made on the 2 million responses from 24 hours prior.
- Only 8000 responded once more.

### Example requests

- Original Json for get peers: {"t":"a", "y":"q", "q":"get_peers", "a": {"id":"12345678901234567890", "info_hash":"12345678901234567890"}}
  - Get Peers bencoded: d1:ad2:id20:123456789012345678909:info_hash20:12345678901234567890e1:q9:get_peers1:t1:a1:y1:qe
- Find Node bencoded: d1:ad2:id20:abcdefghij01234567896:target20:mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe
- BTSync: BSYNC\x00d1:m4:ping4:peer20:123456789012345678905:share20:12345678901234567890e

### Mitigations

- <https://jeremyverda.net/qnap-multiple-outgoing-connection-udp-port-6881/>
- <https://community.sophos.com/products/unified-threat-management/f/network-protection-firewall-nat-qos-ips/40748/closing-port-6881>

### Documentation

- <https://en.wikipedia.org/wiki/Bencode>
- <https://wiki.wireshark.org/BitTorrent>
- <https://serverfault.com/questions/140336/a-lot-of-connections-to-port-6881-some-new-attacks-or-what>
