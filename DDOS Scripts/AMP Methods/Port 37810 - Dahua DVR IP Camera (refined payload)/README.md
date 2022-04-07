# DVR DHCPDiscover (multiple CCTV products) UDP-MIB

## Port: 37810

## Proto: UDP

## Amplification factor: ~30x

## Reflector count: ~235,000 (Aug 2020)

---

Multiple DVR/XVR/NVR products implement multiple UDP ports for different SNMP-like protocols.
37810 (this finding) is specifically for IP Search and DHCP Discovery JSON.
37777 is the default port for remote connections, etc.

### Example Request / Response

- Request Size: Any non-null single byte (e.g. \xff)

  - > ~# echo -ne '\xff'|nc -u 218.5.136.140 37810

- Avg Response Size: ~800 bytes

  - `    DHIP                    { "mac" : "4c:11:bf:c2:0b:9d", "method" : "client.notifyDevInfo", "params" : { "deviceInfo" : { "AlarmInputChannels" : 0, "AlarmOutputChannels" : 0, "DeviceClass" : "IPC", "DeviceType" : "IPC-HDW5421S", "HttpPort" : 80, "IPv4Address" : { "DefaultGateway" : "218.5.136.1", "DhcpEnable" : false, "IPAddress" : "218.5.136.140", "SubnetMask" : "255.255.255.0" }, "IPv6Address" : { "DefaultGateway" : "2001:250:3000:1::1:1", "DhcpEnable" : false, "IPAddress" : "2001:250:3000:1::1:2\/112", "LinkLocalAddress" : "fe80::4e11:bfff:fec2:0b9d\/64" }, "MachineName" : "1F006E4PAX00075", "Manufacturer" : "Private", "Port" : 37777, "RemoteVideoInputChannels" : 0, "SerialNo" : "1F006E4PAX00075", "Vendor" : "Dahua", "Version" : "2.400.0.8", "VideoInputChannels" : 1, "VideoOutputChannels" : 16 } } }`

### Documentation

- <https://ipvm.com/reports/dahua-oem> (OEM Listing and article about US bans)
- <https://community.boschsecurity.com/t5/Security-Video/What-Ports-need-to-be-open-for-LAN-and-WAN-Access-for-Divar-AN/ta-p/536> (Ports)
- <https://www.dahuasecurity.com/?us> (A large number of responders are this manufacturer)
- <https://www.hybrid-analysis.com/sample/0a6d12fc58871c387c2e322b099a545b981a91f6719c5251adf6a79c4bd851b7?environmentId=120> (Potential Dahua windows client analysis)

### Mitigations

- Stop allowing Internet access, restrict to LAN as default configuration (vendor issue)!
- Similar to WSD SOAP Discovery Queries
- <https://www.csoonline.com/article/3089298/iot-botnet-25-513-cctv-cameras-used-in-crushing-ddos-attacks.html>
