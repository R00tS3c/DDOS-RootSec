# SSDP (Simple Service Discovery Protocol)

## Port: 1900

## Proto: UDP

## Amplification factor: 32x

---

### Updated Q1 2020

---

### Bloaty "Public" payloads commonly honeypotted

<pre>
M-SEARCH * HTTP/1.1\r\nHost: 239.255.255.250:1900\r\nMan: \"ssdp:discover\"\r\nMX: 3\r\nST: upnp:all\r\n\r\n
M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 2\r\nST: ssdp:all\r\n\r\n
M-SEARCH * HTTP/1.1\r\nHOST: 239.255.255.250:1900\r\nMAN: \"ssdp:discover\"\r\nMX: 2\r\nST: upnp:discover\r\n\r\n
</pre>

### Refined payload

The `required` header is router set in most cases and not required, sure you'll get a few HTTP 412 precondition failed responses, but those are removed from metrics when you remove <300 byte responses.

Most devices only look for `M-SEARCH`, `ST:ssdp:all` and `MAN:"ssdp:discover"`, thus you can get multi-packet full responses with a payload such as:

```
M-SEARCH\nST:ssdp:all\nMAN:"ssdp:discover"\n
```

Which is only 41 bytes, bringing average amplification slightly above 30x in Q1 2020.

### Sorting multi-packet responses

```
cat capture | sort | awk '$2 >= 210' | awk '{print $1}' | uniq -c | awk '$1 >= 7' | awk '{print $2}' > SSDP_output.txt
```
