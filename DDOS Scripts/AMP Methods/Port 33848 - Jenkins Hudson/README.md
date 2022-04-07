# Jenkins (Hudson Agent) CI Server Discovery

## Port: 33848

## Proto: UDP

## Amplification factor: ~5-8x

## Reflector count: ~2,200 (March 2021), down from 16,900 in Q4 2020

---

- Jenkins has a default discovery mechanism that transmits version, id, and url metadata over unauthenticated UDP.
- Assigned to **CVE-2020-2100**, the problem is merely triggered by a UDP connection, no payload bytes are required for this metadata response.
  - **Please see documentation link 1**
- Some installations respond differently, some with only `version`, `url` and `server-id`. Whereas others have been modified by the owners and respond with all aforementioned plus `swarm` id and `slave-port`.
  - Example full response (usually over 200 bytes):
  - `<hudson><version>***</version><url>***</url><server-id>***</server-id><slave-port>***</slave-port><swarm>***</swarm></hudson>`

### Example Request / Response

- Request: 0 bytes (\n here is just to force netcat to transmit)

  - `> ~# echo -ne '\n'|nc -u 85.214.104.48 33848 -w1`

- Response: Avg 140 bytes (differs as stated earlier)

  - ASCII: `<hudson><version>2.193</version><url>http://vserver.wagar.de:8082/</url><server-id>67a6f6f1f0029dda390dac030c35777d</server-id><slave-port>36532</slave-port></hudson>`
  - Raw Bytes (Only ASCII character space): `3c687564736f6e3e3c76657273696f6e3e322e3139333c2f76657273696f6e3e3c75726c3e687474703a2f2f767365727665722e77616761722e64653a383038322f3c2f75726c3e3c7365727665722d69643e36376136663666316630303239646461333930646163303330633335373737643c2f7365727665722d69643e3c736c6176652d706f72743e33363533323c2f736c6176652d706f72743e3c2f687564736f6e3e`

### Documentation

- Jenkins security advisory (CVE-2020-2100) - <https://jenkins.io/security/advisory/2020-01-29/#SECURITY-1641>
- Securing your Jenkins - <https://wiki.jenkins.io/display/JENKINS/Securing+Jenkins>

### Mitigations

- Update your Jenkins master and agents to version 2.219 (LTS 2.204.2) or higher.
  - OR Set system variable (Jenkins start-up command line `java -D*option_here* -jar jenkins.war`) `hudson.udp` to equal `-1`. As well as `hudson.DNSMultiCast.disabled` to true.
  - E.g. `java -Dhudson.udp=-1 -jar jenkins.war`.
- Blatantly ACL block `<hudson>.*</hudson>` ASCII string or `sport 33848` (since their fix is to disable it by default anyway).
