# Netis / Netcore Command Interface

## Port: 53413

## Proto: UDP

## Amplification factor: 2x

---

Not only are Netcore/Netis routers vulnerable to becoming zombies in actual attacks but they can be reflectors for amplification as well.

### Example request/response

- Request: 1 byte

  - > ~# echo -ne '\x0a' |nc -u 49.89.164.8 53413

- Response: 45 bytes

  - `�no support commond line`
    <pre>Raw: 0a00000600010000ffffffff6e6f20737570706f727420636f6d6d6f6e64206c696e650a00</pre>

### Documentation

- <https://www.securityweek.com/easily-exploitable-vulnerability-found-netis-routers>
- <https://www.securitynewspaper.com/2016/11/22/security-firm-detects-57m-attempts-exploit-2-year-old-router-firmware-backdoor/>

### Mitigations

- Consumer dilligence
- <https://routersecurity.org/testrouter.php>
