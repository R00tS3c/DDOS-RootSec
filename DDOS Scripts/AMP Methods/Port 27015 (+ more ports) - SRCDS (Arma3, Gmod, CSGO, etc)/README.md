# SRCDS (Source game Dedicated Server)

## Observed Games and Ports:

* Arma3 - **2302-2303, 2602**
* HL2, Gmod, TF2, ARK: Survival Evolved (Steam) - **27015-27021**
  * Counter-Strike 1.6 (Classic) - **27302, 45983**
  * Counter-Strike: Source - **29392**
  * Counter-Strike: Global Offensive - **26742, 52084**
* Starbound - **28015, 21025**
* Rustafied (Rust) - **25244**

## Proto: UDP

## Amplification factor: ~5x

## Reflector count: ~25,000

---

Use HLSW to query these Source engine games for players and server stats.

Download HLSW here: <http://www.hlsw.org/hlsw/download/>.

Amplification factor varies depending on server name, server slot fullness and player names (random data).

The TSource Engine query (A2S_INFO) has trivially been used as a direct UDP attack against game servers for many years, however by using it to reflect off other game servers, the data returned can typically be an amplification itself.

- Hexadecimal query: `ffffffff54536f7572636520456e67696e6520517565727900`

### Documentation

- <https://developer.valvesoftware.com/wiki/Source_Dedicated_Server> (Other ports in use)
- <https://developer.valvesoftware.com/wiki/Server_queries#A2S_INFO>
- <https://github.com/BerntA/SRCDSMonitor>

### Mitigations

- <https://github.com/blastehh/SourceQueryCacheMono> (For SRCDS themselves)
- <https://stackoverflow.com/questions/825481/iptable-rule-to-drop-packet-with-a-specific-substring-in-payload> (With ^0xffffffff)
- <http://www.dedicated-server.ru/vbb/archive/index.php?t-27617.html>
