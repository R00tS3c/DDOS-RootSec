# Modbus Applications

## Port: 502, 3074, 44444-44446

## Proto: UDP

## Amplification factor: 4x

---

Standard modbus PLC applications dont really have an amplification, however there are multiple vendors using proprietary implementations that do. Example request/reponse specification for modbus over TCP: https://www.eztcp.com/en/download/pds_files/an_modbus_tcp_en.pdf

### Proprietary Usages

- Port 3074, 44444:
  - Hidden SRV-INFO alias on some implementations:
    - Sending "Hello World" to ports 44444-44446 returns device information.
- CVE-2018-5400

### Mitigation Notes

- Whether unrelating to original Modbus usage, certain application seem to be responding on port 3074 with device information, the vendor is the attached MW-2060AP in the documentation section.

### Documentation

- http://www.modbus.org/docs/Modbus_Messaging_Implementation_Guide_V1_0b.pdf
- https://fccid.io/MMC-MW-2060AP
