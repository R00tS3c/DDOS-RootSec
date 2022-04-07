# OpenVPN (Session Initialisation P_CONTROL_HARD_RESET_CLIENT_V2)

## Port: 1194

## Proto: UDP

## Amplification factor: 2x

---

### Mitigation Notes

- Uses 1 byte opcode since the rest of the packet is just messageID and sessionID which isnt needed for initial connection.

### Documentation

- https://build.openvpn.net/doxygen/network_protocol.html
