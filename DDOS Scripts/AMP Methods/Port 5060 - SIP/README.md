# SIP (Session Initiation Protocol)

## Port: 5060

## Proto: UDP

## Amplification factor: 15x

---

### Malformed requests

- Another UDP service that responds with a large error if a single byte is sent. Many devices simply dont respond but those that do are large error texts as seen below.

- Example response:

  > ~# echo -ne '\xaa' |nc -u 210.89.80.84 5060

  ```
  SIP/2.0 400 Bad Request
  Content-Length: 364
  Content-Type: text/plain
  Missing Via header.
  Missing or invalid Call-Id header
  Missing or invalid CSeq header
  Missing or invalid From header
  Missing or invalid To header
  Missing or invalid status line
  Missing Via header.
  Missing or invalid Call-Id header
  Missing or invalid CSeq header
  Missing or invalid From header
  Missing or invalid To header
  Missing or invalid status line
  ```

### Well-formed requests

- "OPTIONS" Example response:

  ```
  SIP/2.0 200 OK
  Contact: <sip:7830736@37.196.71.33:5060>
  To: <sip:>;tag=7a82b2b7
  From: <sip:>
  Call-ID:
  CSeq: 0 UNKNOWN
  User-Agent: CareIP 7830736 v1.3.0.5
  Content-Type: application/sdp
  Content-Length: 189
  v=0
  o=- 0 0 IN IP4 37.196.71.33
  s=CareTech RTP-stack
  c=IN IP4 37.196.71.33
  t=0 0
  m=audio 10000 RTP/AVP 8 101
  a=rtpmap:8 PCMA/8000
  a=rtpmap:101 telephone-event/8000
  a=fmtp:101 0-15
  ```

### Documentation

- https://blog.counterpath.com/port-5060
- https://www.ietf.org/rfc/rfc3261.txt
