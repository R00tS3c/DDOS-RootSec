# DTLS

## Port: 443

## Proto: UDP

## Amplification factor: 2-3x

## Reflector count: 1,002,850

---

DTLS responses change most bytes but the first on every reply, making this - along with source port 443 - an interesting protocol to mitigate.

There are many handshake types as denoted in `Documentation` IETF draft.

- Such as "ClientHello" with prefix \x01

Each of these packet constructions will yield different sizes depending on the handshake type.
49 bytes for ClientHello and 90 bytes for what I dub Not-present-in-enum-error for example.

The number of reflectors is large but thankfully the amplification factor is minimal.

### Example Malformed request/response

- Request: 9 bytes

  - > ~# echo -ne '\x0a\x00\x00\x00\x00\x00\x00\x00\x00' |nc -u 35.206.41.191 443

- Response: 80 bytes

  - `\0e\00\00\00\00\00\00\00\00PRST\03\00\00\00EPID\03\00\00\00RNON\0b\00\00\00CADR\1f\00\00\00GFE\b5i\0f\00\00\00\00\00\n\00\00\00\00\00\00\00\00\00\00\00\ff\ffn\97S\cd\be\da`
    <pre>Raw: 0E000000000000000050525354030000004550494403000000524E4F4E0B000000434144521F000000474645B5690F00000000000A0000000000000000000000FFFF6E9753CDBEDA</pre>

### Example ClientHello request/response

- Request: 9 bytes

  - > ~# echo -ne '\x01\x00\x00\x00\x00\x00\x00\x00\x00' |nc -u 35.206.41.191 443

- Response: 49 bytes

  - `pI5h9\c7P#E\0ef#\88\9f\98\e5\9f\df\92\96\87q\b1\fd\9b.v5\8d\94\cb\953\85\9ae@\fby\d0\0f`
    <pre>Raw: 7049356839C75023450E6623889F98E59FDF92968771B1FD9B2E76358D94CB9533859A6540FB79D00F</pre>

### Documentation

- https://tools.ietf.org/html/rfc6347
- https://tools.ietf.org/id/draft-ietf-tls-dtls13-02.html
- https://www.cs.ru.nl/bachelors-theses/2018/Niels_Drueten___4496604___Security_analysis_of_DTLS_1_2_implementations.pdf

### Mitigations

- ACL blocking on data, port, or saddr ranges wont be sufficient in most instances, check back here later.
- https://stackoverflow.com/questions/27351714/how-to-distinguish-a-dtls-packet-from-tcp-udp-packet
