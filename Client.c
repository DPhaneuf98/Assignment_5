#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define ERR(msg)  {perror(msg);exit(1);}
#define MAXLINE 512


int main(int argc,char *argv[])
{
  struct sockaddr_in bba;
  struct hostent *hp;
  int slen;
  int s;
  int rfd;
  char ch[MAXLINE];
  int num_char=MAXLINE;
  int port;
  
  char portnum[20];
  char hostname[20];
  
  printf("\n hostname: ");
  scanf("%s",hostname);
  printf("\n portnum);
  
  memset(&bba,0,sizeof(bba));
  bba.sin_family = AF_INET;
  hp=gethostbyname(hostname);
  port=atoi(portnum);
  memcpy(&bba.sin_addr,hp->h_addr,hp->h_length);
  bba.sin_port=htons(port);
  
  
  s = socket(AF_INET,SOCK_STREAM,0);
  
  if(s==-1)
    ERR("socket error:");
  if(connect(s,struct sockaddr *)&bba,sizeof(bba)) !=0)
    ERR("Connection Error:")
    
  pid_t fork_return;
  fork_return =fork();
  
  if(fork_return == -1)
  {
    printf("Fork Error: \n");
    return 1;
  }
  else if(fork_return == 0)
  {
     while( (num_char=read(0,ch,MAXLINE)) > 0 )
    if ( write(s,ch,num_char) < num_char)
      OOPS("writing");
    close(s);
    return 1;
  }
  else if(fork_return > 0)
  {
    while((num_char=read(s,ch,MAXLINE))> 0)
        if (write(1,ch,num_char) < num_char)
           OOPS("writing");
      close(s);
    return 1;
  }
  return 0;
}
