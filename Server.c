#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORTNUM
#define BBSD_FILE
#define ERR(msg)


int main()
{
  struct sockaddr_in saddr;
  struct hostent *hp;
  char hostname[256];
  socklen_t slen;
  int s;
  int sfd;
  char ch[MAXLINE];
  FILE *sf;
  int bbf;
  int num_char=MAXLINE;
  
  
  memset(&saddr,0,sizeof(saddr));
  saddr.sin_family = AF_INET;
  gethostname(hostname,sizeof(hostname));
  hp = gethostbyname(hostname);
  memcpy(&saddr.sin_addr,hp->h_addr,hp->h_length);
  saddr.sin_port = htons(PORTNUM);
  
  s = socket(AF_INET, SOCK_STREAM,0);
  if(s==-1)
    ERR("socket");
    
  if(bind(s,(struct sockaddr *)&saddr,sizeof(saddr)) != 0)
    ERR("bind");
    
  slen = sizeof(saddr);
  getsockname(s,(struct sockaddr *)&saddr,&slen);
  printf(Socket assigned: %d\n",ntohs(saddr.sin_port));
  
  if(listens(s,1)!=0)
    ERR("listen");
    
  for(;;)
  {
    sfd = accepts(s,NULL,NULL);
    if(sfd == -1)
      ERR("accepts");
      
     pid_t fork_return;
     fork_return = fork();
     
     if(fork_return==-1)
     {
        printf("Fork error: \n");
        return 1;
     }
     else if(fork_return == 0)
     {
        while((num_char=read(0,ch,MAXLINE))>0)
          if(write(sfd,ch,nu_char)<num_char)
            ERR("writing");
         close(s);
           
         return 1;
     }
     else if(fork_return >0)
     {
        while((num_char=read(sfd,ch,MAXLINE))> 0)
        if (write(1,ch,num_char) < num_char)
           OOPS("writing");
      close(bbf);
    
    close(sfd);

     }
     return 0;
  } 
  }
