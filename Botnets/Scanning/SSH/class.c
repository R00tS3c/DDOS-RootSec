#include <libnet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <pcap.h> 
#include <time.h>

#define COLOR_RED         "\x1b[31m"
#define COLOR_GREEN       "\x1b[32m"
#define COLOR_YELLOW      "\x1b[33m"
#define COLOR_BLUE        "\x1b[34m"
#define COLOR_MAGENTA     "\x1b[35m"
#define COLOR_CYAN        "\x1b[36m"
#define COLOR_RESET       "\x1b[0m"

int main(int argc, char **argv) {
 libnet_t *l;
 libnet_ptag_t t; 
 unsigned short burst=50;
 unsigned short ct=0;
 char errbuff[LIBNET_ERRBUF_SIZE];
 unsigned long myip;
 struct in_addr sc;
 unsigned char tcpopt[]="\x02\x04\x05\xb4\x01\x01\x04\x02";

 unsigned short port;
 unsigned long usec;
 //unsigned char outstr[1024];
 char cc;
 int i;
 pid_t pid;
 pcap_t *handle;
 char *temp_char;
 bpf_u_int32 mask; 
 bpf_u_int32 net; 
 char errbuf[PCAP_ERRBUF_SIZE]; 
 char filter[1024];
 struct bpf_program cfilter;
 struct pcap_pkthdr header; 
 const unsigned char *packet; 
 struct in_addr ekkt; 
 unsigned char ip[50]; 

 unsigned long dstip=0;
 unsigned short sport;
 char *interface=NULL;
 unsigned char bclass=0;
 unsigned char aclass=0; 
 unsigned char rclass=1;
 unsigned int a=0,b=0,c=0,d=0;
 
 srand(time(NULL));
 sport=rand();
 usec=1000000;
 if(argc<2) {
   system("clear");
   printf("\x1b[33m============================================\n");
   printf("             *Nos Security Team*            \n");
   printf("============================================\x1b[0m\n");
   printf("============================================\n");
   printf("            Advanced TCP Port Scanner       \n");
   printf("============================================\n");
   printf("============================================\n");
   printf("   *Version  : 2.0 / 2016                   \n");
   printf("   *Made by  : Binary                       \n");
   printf("============================================\n");
   printf("   *Works on : Linux                        \n");
   printf("============================================\n");
   printf("\x1b[32m============================================\n");
   printf("   *Usage    : %s port <opt>                \n",argv[0]);
   printf("   *Example  : %s 22 -a 24 -s 10 -i eth0\n",argv[0]);
   printf("   *Example  : %s 22 -b 24.1 -s 10 -i eth0\n",argv[0]);
   printf("============================================\x1b[0m\n");
   printf("\x1b[35m   *Options  : -a Class A (ex: 24)          \n");
   printf("             : -b Class B (ex: 213.14)      \n");
   printf("             : -s speed (from 1 to 10)      \n");
   printf("             : -i interfce (ex: eth0)       \x1b[0m\n");
   printf("============================================\n");
   printf("\x1b[35m   *Bonus  : %s -fix Auto Filter          \x1b[0m\n",argv[0]);
   printf("============================================\n");
   exit(0x01);
 }
 for(i=1;i<argc;i++) {
    if(strstr(argv[i],"-s")) {
    if(i+1<argc) {
    switch (atoi(argv[i+1])) {
      case 1:usec=1000000;break;
      case 2:usec=500000;break;
      case 3:usec=250000;break;
      case 4:usec=125000;break;
      case 5:usec=60000;break;
      case 6:usec=30000;break;
      case 7:usec=10000;break;
      case 8:usec=1000;break;
      case 9:usec=100;break;
      case 10:usec=0;burst=65535;
   }
 } else {
      printf("\x1b[31m-s requires an argument\x1b[0m\n");
      exit(0x01);
   } 
 }
if(strstr(argv[i],"-fix")) {
  system("clear");
  printf("\x1b[34mFucking FILTERING shit nigga\x1b[0m\n");
  system("cat bios.txt | sort | uniq > mfu.txt");
  system("wc -l mfu.txt");
  exit(0x02);
}
 if(strstr(argv[i],"-i")) {
  if(i+1<argc) interface=argv[i+1];
 else {
     printf("\x1b[31m-i requires an argument\x1b[0m\n");
     exit(0x01);
  }
 }
 if(strstr(argv[i],"-a")) {
  if(i+1<argc) {
 aclass=1;
 bclass=0;
 rclass=0;
 a=atoi(argv[i+1]);
 b=0;
 c=0;
 d=0;
 //printf("%d\n",a);
 if((a<1) || (a>254)) {
  printf("\x1b[31mA must be between 1 and 254\x1b[0m\n");
  exit(0x02);
 }
 printf("\x1b[31;4mScanning network \e[32;1m%d.*.*.*\x1b[0m\n",a);
} else {
       printf("\x1b[31m-a requires an A network as argument\x1b[0m\n");
      exit(0x01);
   } 
 }
 if(strstr(argv[i],"-b")) {
  if(i+1<argc) {
 aclass=0;
 bclass=1;
 rclass=0;
 a=atoi(strtok(argv[i+1],"."));
 temp_char=strtok(NULL,".");
 if(temp_char==NULL) 
 b=0;else b=atoi(temp_char);
 c=0;
 d=0;
 //printf("%d\n",a);
 if((a<1) || (a>254)) {
  printf("\x1b[31mA must be between 1 and 254\x1b[0m\n");
  exit(0x02);
 }
     printf("\x1b[31;4mScanning network \e[32;1m%d.%d.*.*\x1b[0m\n",a,b);
  } else {
      printf("\x1b[31m-b requires an B network as argument(e.g. 192.168)\n");
      exit(0x01);
    } 
   } 
 }
 printf("\e[34;4mUsec: %ld, Burst packets %d\n",usec,burst);
 port=(unsigned short)atoi(argv[1]);
 if((port<1) || (port>65535)) 
   exit(printf("\x1b[31mPort numbers are in 1 .. 65535\x1b[0m\n"));
 if(interface!=NULL) 
 printf("\e[34;4mUsing inteface %s\x1b[0m\n",interface); 
 l=libnet_init(LIBNET_RAW4,interface,errbuff);
 if(!l) {
    printf("\x1b[31mERROR: %s\x1b[0m\n",errbuff);
    exit(0x02);
 }
 myip=libnet_get_ipaddr4(l);
 sc.s_addr=myip;
 sprintf(filter,"(tcp[tcpflags]=0x12) and (src port %d) and (dst port %d)",port,sport);
 printf("\e[34;4mUsing \"%s\" as pcap filter\n",filter);
 printf("\e[34;4mMy detected ip on %s is %s\n",l->device,inet_ntoa(sc));
 pcap_lookupnet(l->device, &net, &mask, errbuf);
 pid=fork();
 handle=NULL;
 handle = pcap_open_live(l->device, BUFSIZ, 1, 0, errbuf);
 if(handle==NULL) {
   printf("\x1b[31mERROR: pcap_open_live() : %s\x1b[0m\n",errbuff);
   exit(0x05);
 } 
 cc=pcap_compile(handle, &cfilter, filter, 0, net);
 if(cc!=0) {
  printf("\x1b[31mERROR: pcap_compile() failed!!!\x1b[0m\n");
  exit(0); 
 }
 cc=pcap_setfilter(handle, &cfilter);
 if(cc!=0) {
  printf("\x1b[31mERROR: pcap_setfilter() failed!!!\x1b[0m\n");
  exit(0);
 }
 if(pid==0) {
 /* sniff */
 while(1){
     packet = pcap_next(handle, &header);
     memcpy(&ekkt.s_addr,packet+26,4);
     printf("%s\n",inet_ntoa(ekkt));
     FILE * fp;
     fp=fopen("bios.txt","a+");
     fprintf(fp,"%s\n",inet_ntoa(ekkt));
     fclose(fp);
   }
 }
 if(pid > 0) {
  printf("\e[34;4mCapturing process started pid %d\x1b[0m\n",pid);
  usleep(500000);
  while(1) {
   t=LIBNET_PTAG_INITIALIZER;
   t=libnet_build_tcp_options(tcpopt, 8, l,0); 
   //t=LIBNET_PTAG_INITIALIZER;
   t=libnet_build_tcp(sport,port,rand(),rand(),TH_SYN,65535,0,0,LIBNET_TCP_H+8,NULL,0,l,0);
if(rclass) 
 dstip=rand();
if(aclass) {
 if(d==0) 
 printf("\x1b[31;4mScanning \e[32;1m%d.%d.%d.*\x1b[0m\n",a,b,c);
 d++;
 if(d>255) {c++;d=0;}
 if(c>255) {b++;c=0;}
 sprintf(ip,"%d.%d.%d.%d\n",a,b,c,d);
 //printf("%s\n",ip);
 if((b==255)&& (c==255) && (d==255)) {
  printf("\e[34;4mScanning has Stopped\x1b[0m\n");
  sleep(10); 
  kill(pid,2);
  return 0;
 }
    sc.s_addr=inet_addr(ip);
    dstip=sc.s_addr;
  }
  if(bclass) {
    if(d==0) 
    printf("\x1b[31;4mScanning \e[32;1m%d.%d.%d.*\x1b[0m\n",a,b,c);
    d++;
 if(d>255) {
    c++;d=0;
 }
 sprintf(ip,"%d.%d.%d.%d",a,b,c,d);
 if((c==255) && (d==255)) {
  printf("%s\n",ip);
  printf("\e[34;4mScanning has Stopped\x1b[0m\n");
  sleep(10); 
  kill(pid,2);
  return 0;
 }
 sc.s_addr=inet_addr(ip);
 dstip=sc.s_addr;
 }
  libnet_build_ipv4(LIBNET_TCP_H+LIBNET_IPV4_H+8,0,rand(),0,128,IPPROTO_TCP,0,myip,dstip,NULL,0,l,0);
  cc=libnet_write(l);
  if(cc<=0) 
  printf("\x1b[31mlibnet_write() wtf %d\x1b[0m\n",cc);
  libnet_clear_packet(l);
  if(ct==burst) {
         usleep(usec);
         ct=0;
       };
     ct++;
    }
 }
 if(pid<0) {
    printf("\x1b[31mCannot fork()\x1b[0m\n");
    exit(0x05);
 }
 return 0;
}

