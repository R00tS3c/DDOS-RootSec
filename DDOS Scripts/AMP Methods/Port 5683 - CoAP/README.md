# Constrained Application Protocol (CoAP)

## Port: 5683

## Proto: UDP

## Amplification factor: ~20x

---

Other ".well-known/core" resource retrieval request constructions (UID bit changes):

- `40017d70bb2e77656c6c2d6b6e6f776e04636f7265`
- `400101cebb2e77656c6c2d6b6e6f776e04636f7265`
- `40017257bb2e77656c6c2d6b6e6f776e04636f7265`
- `40014b70bb2e77656c6c2d6b6e6f776e04636f7265`
- `40016570bb2e77656c6c2d6b6e6f776e04636f7265`
- `40010101bb2e77656c6c2d6b6e6f776e04636f7265`
- `40015477bb2e77656c6c2d6b6e6f776e04636f7265`

All ".well-known/core" query responses are the same and will be 989 or 991 bytes.

### Example Request / Response

- Californium CoAP RFC Version Request: `44010f3cd19796c1c13cff0000`
  - `D<ї���<�`
- Hex Response: 458 bytes (not incl headers)
  - `64450f3cd19796c1c0ff2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a0a436f415020524643203732353220202020202020202020202020202020202020202020202020202020202020202020202020202020202020202020200a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a0a5468697320736572766572206973207573696e67207468652045636c697073652043616c69666f726e69756d202843662920436f4150206672616d65776f726b0a7075626c697368656420756e6465722045504c2b45444c3a20687474703a2f2f7777772e65636c697073652e6f72672f63616c69666f726e69756d2f0a0a28632920323031342c20323031352c203230313620496e7374697475746520666f722050657276617369766520436f6d707574696e672c20455448205a757269636820616e64206f74686572730a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a2a`

### Documentation

- Cf Core CoAP implementation: <https://www.eclipse.org/californium/>
  - Potential vendor for Cf Core enabled-by-default devices: <https://qlinkwireless.com/>

### Mitigations

- Well known responses are 989 or 991 bytes exactly and will contain Ascii `/.well-known/core`
- RFC version request reponses are 458 bytes exactly and will contain Ascii `CoAP RFC 7252`
