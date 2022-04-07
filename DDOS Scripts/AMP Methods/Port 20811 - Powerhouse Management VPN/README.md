# "PHMGMT" Powerhouse Management, Inc - VyprVPN and Outfox

## Port: **20811**

## Proto: **UDP**

## Data Amplification factor: **~40x**

## Packet Amplification factor: **1-50x**

## Effective Amplification Average: **366x**

## Reflector count: **~1,520** (Feb 2021)

---

### Patching has been [rolled out by Powerhouse](https://www.vyprvpn.com/blog/vyprvpns-response-to-article-on-server-bug)

Updated scan result 2021-02-25 yields **no reflectors left active**! Well done.

---

Powerhouse Management products - either Outfox (a latency reduction VPN service) or VyprVPN (a general vpn service) are exposing an interesting port - port 20811 which provides a massive data and packet amplification factor when probed with any single byte request.

Not only does this mean Powerhouse servers can be used as a DDoS amplification source, but reveals all servers around the world that are running such potential VPN services - which removes the privacy factor somewhat.

In testing, Powerhouse Management ISP owned servers were not present on Shodan at all, however responded to our probes with varying degrees of amplification. All response IP's had the hostname of `"undefined.hostname.localhost"`

* Packet amplification = **1 : 1-50**
  * Very unstable, sometimes no response at all (not a rate limit)
  * Average was 9 packets across 100 test IPs
* Data amplification average = **~40x**
* Multi-packet average effective amplification = **366x**
  * *NTP vibes anyone?*

### Example Requests / Responses

* Request: **1 data bytes**

  * ASCII: `[NULLBYTE]`

  * `> ~# echo -ne '\x00'|nc -u 128.90.242.100 20811 -w2|xxd -p`

* Response: **One packet Avg 1030 bytes (multi-packet)**

  * ASCII character space:

    ```
    aA\a8\83RL\a4\02\c3}\1d \98\b4\f0 E\c4\86\95\e2\9e\d7\d7\a9\11\a6\00\99\fa-P\de\a9\ee\e6\f6\c8\de^\df4s\06\d9\10\d0\b3i \adi\91Z\88\a4\b8\f5d\ba\9a\eeb!q\a0t\c0\e1z\12\d7\be\01\7f\89XL\a0\f3R\8d4\cf\00\d5\87\0f\c4\ff\e1\tC\ac@\ee\f4\15\e6Rv\a7\85\cbbb\b4$\a3\bc\c6\16Vs%g\92l\1b\ef#\c1\82Z\bd\be\d4\81\ae\b4\cd\06X\c0\81/T\bf\8b\8c\01x\81f9fl-\de\f0T\1d\d9\fa"G\e5\cej\82\1f\b1\82\17\17\f0\ea\0b\15\f6\01MR\95e\f43\c1\e3,\c0\047dX\dd\99\8b\88\e0\b2\ea$~\96\85\e9k\86\90b\11"\bf\b7X|T\19\d54\ae\1a\f9K\90\94\c6 2\1e\f2m\da\db\ea\00Q\92\c9\06V6cX\91\93\b4\e1\e9?(\f3%\87\1dcFs\7f\bc\18\01\e9\e0;5\b3Hb9_\95\c9\d2\bd\ae\e5\e4SEI/\bb\f3\0c\t\c0}\af\ac\c9J\c5\bdNU\f5\f8\ac\bb\87\d4\a9\c6*\f5!\e2U\bb=\c7\aaa.\b9\9c\a4fT\d2\b9\c6\a5\ba\8c\c1]\1f\t%\n\c6\d1\a1\aeh-\d7\da\dd\f4\d5\bc\cf"\ea=4.OQ\b6\91\c7#J\14\a982W}\01\ca'Z4\df\ce"X\cf\98\f4\b2/\c8i\b58sY$r\c8\8d\d7\92\b7-j\d8*|\a4XB\fbV\03-\d8\f04\9a+\b8\bc\d1\ed\e0\86L\8fA\03\aa\d0\84+\d1sP\dc\d7p=*\cb,a\d0-\7f\1cm%\0f\f3<\fe\ec.\rv.\16\f6\n\r[\aa\17d\f2\f8p[\f0h\a2\9f\d4R=\fe\8b\01<H_\8e{=\f1\ff\f6$@'{$2\0eK\1e%\8a64&\ea\t)\88\b0W+\ab\ca\d1G\03\dc\06\e0\03k\19\c2\0b]\a7\00'b"\abv\d1|\b0\ac\cds\13\a6\8eV\c4p\a3\d7\d0\fa\bc\d1\06\een\fd-H\da\d5k\a4Rw\fb\81\c3|R}\da\d0\1at\82O\80\f1hE}\f8\08\ce\18W\aa\d2m\94R\b7\ddv\cem\ffF\e6\a8\8a\b3~]b\82F\f8\c9!\d0\d7i\dc\d51\c5\c3,\ffK\9f\aa\cah[i\8dh,4\fe\e6z\1c3\c62\f9!\9f\06\a5\8a\ae\bf\c8\c3\e5\b9\12\1fM\c0\b4&\1f*\9d\a5Y\a5\d7\af\ba|\d6\8fA\\8\83\f0\95g\d9"\84R\da\f9\d9\9f\e8\96\b73\7fp\ee\0e_+0HeKO\19\afK=\df\94\cd\0b6\r\b02\1da\95m\b9[w\cb=(\r9\9a\dbp\c42\a1$Z\db\14\0c\89\e1f+\bb9(\e8yQB>\d0\cclfs5:Y|\ba\02\a4!\b6\ab\1a\bf\f5IK\c2\e9\06]\ec?\ad\05\af\84\e9O\93\f1\c5\b2]\16p4Z\cc9\a5B\c3N\bab\02cF\eaHD\f7\cf\d7?\c0\02.\92\8c|0\de^\af\06_h\b7o\c7\f5O'\9f\80Z\87N\d1\10\90\1a\18\0f% \8f\1d\c5:\89\85>h\17-S\d3\8c7\9dR\9a\b8LLg\a3\f8\d4\87\16\dd\00\8e\88\a4\a1iX\93c\b7\c7\18\cc\87\df\a6\e73\ef\c4\a2\c2\a0\0fG\d1\1c\d7H\d8\8c\cbH\a2<\c4kp\f3\ed\b0\11\9c\0f;\dc\7f\bcu\16]\eaD|Q\0f\b8\t\81\08).vOw\c8\dd\9f\a3\bb4/\aep\81\cfI\04\a4\e2S\83}*\fe\83\80\11\b3@\8e\e7\e2(\c6b\95p\1e\8d\dd\c1W\df4S\9c\ab\02\d5\b3\8fg\c8\0c\ee\ebp\99\e7\98\af\e1x}\8d=\e3\e9\e7\81\94\8a\1e\11S\f9\fe'\d2d\rY\b7j6M\ce#,\b3\0f\b0\t\19ZL\b7X\b5\12\c6\ea5]\b0\02\adU\dd\a02\a0\c5\94\aa/\e8\0e/p\e2>\efE\ccO:\c4a\a9\ee\cfC\d4\b0\aeTc-\84YDl\e2w\c9\e0w\b9\1c^\93(\1b\db\f5\0f\a4\ea\af\89t-ADL\a67\95SS\0f\b4\99&\b7{\ba\ce\\L\f4\85\c4\1c\fb[\1c\93\e8\19qv8\a2\8b\n\bd\163X\cb\00/\bc\ee8\8bv\b2\14\ed\b3:5\f8\1e\84M\a6C\85k\19\ec\d04,Q\e5\15Ct\1c\0b;\d5\01(\deR\a3\fa\ef5B\a9\a3tJ\b6\b3\87sQ\83\16\92\e9\da [)\1b0R\t\ad^y\e2\bd\e5R\bbx\b9;\1e\10\n\fd*zQ\df\e9\d8\c7\19\14\ac\c1\9f4\fcnq\19\98\e7[~\d1(i\d1\9c&kN\f2\c5k5\cf0Vl\85\c5j\t\dd\03(\11\04\nG\bf\82\a2\8b\88\c1#\ff88\cbp\ce>#\ce\c2\b4\0e\87\c8\9b\13{\82g(*\14\ca\ffl0\cb\d1x\b8(\dd\d3\90\ee\ad\e5\f9\04\16
    ```

  * Raw bytes (example only because data is random - probably encrypted): 

    ```
    6141A883524CA402C37D1D2098B4F02045C48695E29ED7D7A911A60099FA2D50DEA9EEE6F6C8DE5EDF347306D910D0B36920AD69915A88A4B8F564BA9AEE622171A074C0E17A12D7BE017F89584CA0F3528D34CF00D5870FC4FFE10943AC40EEF415E65276A785CB6262B424A3BCC61656732567926C1BEF23C1825ABDBED481AEB4CD0658C0812F54BF8B8C0178816639666C2DDEF0541DD9FA2247E5CE6A821FB1821717F0EA0B15F6014D529565F433C1E32CC004376458DD998B88E0B2EA247E9685E96B8690621122BFB7587C5419D534AE1AF94B9094C620321EF26DDADBEA005192C906563663589193B4E1E93F28F325871D6346737FBC1801E9E03B35B34862395F95C9D2BDAEE5E45345492FBBF30C09C07DAFACC94AC5BD4E55F5F8ACBB87D4A9C62AF521E255BB3DC7AA612EB99CA46654D2B9C6A5BA8CC15D1F09250AC6D1A1AE682DD7DADDF4D5BCCF22EA3D342E4F51B691C7234A14A93832577D01CA275A34DFCE2258CF98F4B22FC869B5387359247260C88DD792B72D6AD82A7CA45842FB56032DD8F0349A2BB8BCD1EDE0864C8F4103AAD0842BD17350DCD7703D2ACB2C61D02D7F1C6D250FF33CFEEC2E0D762E16F60A0D5BAA1764F2F8705BF068A29FD4523DFE8B013C485F8E7B3DF1FFF62440277B24320E4B1E258A363426EA092988B0572BABCAD14703DC06E0036B19C20B5DA700276222AB76D17CB0ACCD7313A68E56C470A3D7D0FABCD106EE6EFD2D48DAD56BA45277FB81C37C527DDAD01A6074824F80F168457DF808CE1857AAD26D9452B7DD76CE6DFF46E6A88AB37E5D628246F8C921D0D769DCD531C5C32CFF4B9FAACA685B698D682C34FEE67A1C33C632F9219F06A58AAEBFC8C3E5B9121F4DC0B4261F2A9DA559A5D7AFBA7CD68F415C3883F09567D9228452DAF9D99FE896B7337F70EE0E5F2B3048654B4F19AF4B3DDF94CD0B360DB0321D61956DB95B77CB3D280D399ADB70C432A1245ADB140C89E1662BBB3928E87951423ED0CC6C606673353A597CBA02A421B6AB1ABFF5494BC2E9065DEC3FAD05AF84E94F93F1C5B25D1670345ACC39A542C34EBA62026346EA4844F7CFD73FC0022E928C7C30DE5EAF065F68B76FC7F54F279F805A874ED110901A180F25208F1DC53A89853E68172D53D38C379D529AB84C4C67A3F8D48716DD008E88A4A169589363B7C718CC87DFA6E733EFC4A2C2A00F47D11CD748D88CCB48A23CC46B70F3EDB0119C0F3BDC7FBC75165DEA447C510FB8098108292E764F77C8DD9FA3BB342FAE7081CF4904A4E253837D2AFE838011B3408EE7E228C66295701E8DDDC157DF34539CAB02D5B38F67C80CEEEB7099E798AFE1787D8D3DE3E9E781948A1E1153F9FE27D2640D59B76A364DCE232CB30FB009195A4CB758B512C6EA355DB002AD55DDA032A0C594AA2FE80E2F70E23EEF45CC4F3AC461A9EECF43D4B0AE54632D8459446CE277C9E077B91C5E93281BDBF50FA4EAAF89742D41444CA6379553530FB49926B77BBACE5C4CF485C41CFB5B1C93E819717638A28B0ABD163358CB002FBCEE388B76B214EDB33A35F81E844DA643856B19ECD0342C51E51543741C600B3BD50128DE52A3FAEF3542A9A3744AB6B3877351831692E9DA205B291B305209AD5E79E2BDE552BB78B93B1E100AFD2A7A51DFE9D8C71914ACC19F34FC6E711998E75B7ED12869D19C266B4EF2C56B35CF30566C85C56A09DD032811040A47BF82A28B88C123FF3838CB70CE3E23CEC2B40E87C89B137B8267282A14CAFF6C30CBD178B828DDD390EEADE5F90416
    ```

### Documentation

* Powerhouse Management Brands list: <https://www.phmgmt.com/#full-width-organism-3601>
* AS21926 Same Address as Powerhouse: <https://ipinfo.io/AS21926>
* AS22363 Powerhouse Inc and "Unus, Inc": <https://ipinfo.io/AS22363>

### Mitigations

* Attempt to get in contact with powerhouse to patch this intended or unintentional port leak.
* Block ranges associated with Powerhouse AS if under attack by this method, 128.90.0.0/16 is the most egregious reflector range.
* Block srcport 20811 as it is not assigned with IANA.
