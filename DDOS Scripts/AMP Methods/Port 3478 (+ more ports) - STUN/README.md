# STUN/TURN Open Source Implementations

## Port: 3478, also 8088 + 37833 (Cisco STUN - *thanks Sylvia*)

## Proto: UDP

## Amplification factor: ~3x

## Reflector count: ~180,000 (20/3/2021)

---

- Restund, CoTURN, reTurnServer all implement RFC5389 with advanced features for servers.
- However they suffer from being able to amplify blank 20-byte STUN messages with their version information and other STUN initialization bytes.
- 20-byte STUN packet construction:
```|0 0|     STUN Message Type     |         Message Length        |                         Magic Cookie                          |                     Transaction ID (96 bits)                  |```
  - Message type is encoded into the first 16 bits (2 bytes) and the magic cookie is: `\x21\x12\xa4\x42`
  - The Message itself is not included (we are just sending the 20-byte header) thus the length remains `\x00`. The transaction id (last 12 bytes) is left blank.
  - Read `Structure` documentation below to see how this 20-bytes is constructed in a legitimate use-case.

### Example Request / Response

- Request: 20 bytes

  - `> ~# echo -ne '\x00\x01\x00\x00\x21\x12\xa4\x42\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'|nc -u 3.88.177.39 3478 -w1`

- Response: Avg 100 bytes (differs depending on open source implementation)

  - ASCII: `P!�B �	�������R��+��Y�,��Y�"Coturn-4.5.0.4 'dan Eider'`
  - Raw Bytes: `010100502112a442000000000000000000000000002000080001b10982bef6cc000100080001901ba3ac528e802b000800010d96ac1f59eb802c000800010d97ac1f59eb8022001a436f7475726e2d342e352e302e34202764616e204569646572270000`

### Documentation

- Restund open source project - http://www.creytiv.com/restund.html
- Coturn open source project - https://github.com/coturn/coturn
- Structure for STUN_codec messages - https://docs.rs/stun_codec/0.1.12/stun_codec/struct.Message.html
- STUN RFC - https://tools.ietf.org/html/rfc5389

### Mitigations

- Use STUN over TCP by default.
- Response data for the amplified connection request will always be message type `\xa4\x42` on bytes[7:8].
- Data size is random but srcport 3478 and ASCII `turn` or `STUN` can be sufficient to block.
