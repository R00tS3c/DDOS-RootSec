# QUIC (Quick UDP Internet Connection)

## Port: **80**, **443**

## Proto: **UDP**

## Data Amplification factor: **~2.2**

## Packet Amplification factor: **1x**

## Reflector count: ~51100 (Port 443), ~21500 (Port 80)

---

QUIC is an evolving standard and has many different implementations, some of which are vulnerable to being used as amplification vectors.

#### Public Reset (PRST)

The Transport Area Working Group has an edition of QUIC that as part of an error handling special packet called the [Public Reset Packet](https://tools.ietf.org/html/draft-tsvwg-quic-protocol-02#section-6.2.2), will transmit unauthenticated (un-tokenized) responses with a 2.2x amplification factor. **Not all QUIC capable servers will respond like this**.

Being received over port 80 and 443 would mean a simple port block would not be a feasible mitigation approach.

If the request first byte is a malformed or RST Public Flag, say `0x0E` (in the example below):

* This is the means by which to trigger the Public Reset response which contains the Nonce Proof (RNON) and Connecting Address (CADR) .
* In binary the Public Flag corresponds to `0 0 0 0 1 1 1 0` which indicates RST and an 8 byte connection ID, which we provide as all `0x00`'s. We must provide an 8 byte connection ID unless negotiated otherwise (0,1,4,8 byte options available after nego).

If the first byte is not a reset or malformed public flag, say `0x01`:

* The response will be an encryption session establishment packet which is slightly smaller in size to the PRST packet (41 bytes with `0x70` in byte position 0).

### PRST Example Requests / Responses

- Request: **9 data bytes**

  - ASCII: `         `

  - `> ~# echo -ne '\x0e\x00\x00\x00\x00\x00\x00\x00\x00'|nc -u 101.33.10.93 80 -w2|xxd -p`

- Response: **First packet 49 bytes**, **Transmitted once**

  - ASCII character space: `\0e\00\00\00\00\00\00\00\00PRST\02\00\00\00RNON\08\00\00\00CADR\10\00\00\00\b5i\0f\00\00\00\00\00\02\00n!\0f|\8d\d4`

  - Raw bytes (example only because of randomness from nonce and client ID): `0e00000000000000005052535402000000524e4f4e080000004341445210000000b5690f000000000002003ed2d983aa01`
  - Other example: `0000000300000000696e76616c69642072657175657374` (~15.500 of total reflectors on port 80)

### Documentation

- Working group QUIC protocol draft: <https://tools.ietf.org/html/draft-tsvwg-quic-protocol-02>
- QUIC SSH Transport: <https://tools.ietf.org/html/draft-bider-ssh-quic-00>
- IETF QUIC transport draft: <https://tools.ietf.org/html/draft-ietf-quic-transport-29>
- Chromium development on this vulnerable QUIC version: <https://chromium.googlesource.com/chromium/src/net/+/f82cd8e75065e3ba0e712e27ac72054b589dadb6/quic/quic_protocol.h>
- QUIC Overview: <https://www.ietf.org/proceedings/98/slides/slides-98-edu-sessf-quic-tutorial-00.pdf>

### Mitigations

- Further additions on draft-tsvwg-quic-protocol-02 to complete the "TODO" section and include the tokenization process on PRST.
- Block traffic that matches 49 bytes with `0x0E` at byte position 0.
