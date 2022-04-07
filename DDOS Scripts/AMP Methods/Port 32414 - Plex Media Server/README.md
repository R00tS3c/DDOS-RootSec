# Plex Media Server

## Port: **32414**, **32412**

## Proto: **UDP**

## Amplification factor: **~10x**

## Reflector count: **~24,550 (Feb 2021 Post-patch)**

---

## Patch released! <https://forums.plex.tv/t/plex-media-server/30447/396>

---

### Malformed requests

- Plex Media Server seems to respond with full HTTP 200 response when even a single character "M" from the Simple Service Discovery well-formed probe "M-SEARCH \* HTTP/1.1" is sent.
- This leads to **only 1 byte request responding with an average of 260 bytes**, leading to an amplification factor of ~10x.

### Example request/response

- > ~# echo -ne '\x4d' |nc -u 75.133.123.68 32414
      	<pre>
      	HTTP/1.0 200 OK
      	Content-Type: plex/media-server
      	Host: 5751fff746f9471faf39a071936549c4.plex.direct
      	Name: @Raflix
      	Port: 32400
      	Resource-Identifier: 496050be21439b3f8f6b2026152ff976433b8e18
      	Updated-At: 1570103260
      	Version: 1.18.0.1846-f62172e99
      	</pre>

### Mitigations

- https://github.com/RasPlex/RasPlex/issues/95
- https://forums.plex.tv/t/plex-flooding-32414-and-32412/65443/2
