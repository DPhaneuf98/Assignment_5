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


#define PORTNUM         0
#define BBSD_FILE       "./test.bbs.file"
#define OOPS(msg)       {perror(msg); exit(1);}
#define MAXLINE 512

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
  memcpy(&saddr.sin_addr, hp->h_addr, hp->h_length);
  saddr.sin_port = htons(PORTNUM);
    
    
  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == -1)
    OOPS("socket");

 
  if(bind(s,(struct sockaddr *)&saddr,sizeof(saddr)) != 0)
    OOPS("bind");


  slen = sizeof(saddr);
  getsockname(s,(struct sockaddr *)&saddr,&slen);
  printf("Socket assigned: %d\n",ntohs(saddr.sin_port));

  /* Tell socket to wait for input.  Queue length is 1. */
  if(listen(s,1) != 0)
    OOPS("listen");

  /* Loop indefinately and wait for events. */
  for(;;)
  {
    /* Wait in the 'accept()' call for a client to make a connection. */
    sfd = accept(s,NULL,NULL);
    if(sfd == -1)
      OOPS("accept");
      
    /* Open our file for copying to the socket. */
    
  pid_t fork_return;
  fork_return = fork();

  if(fork_return>0)
  {
      /*Read from file, write to socket*/
      while((num_char=read(sfd,ch,MAXLINE))> 0)
        if (write(1,ch,num_char) < num_char)
           OOPS("writing");
      close(bbf);
    
    close(sfd);
  }
  else if(fork_return==0)
  {
  /* read from the socket, write to the screen */
      printf("New user has joined: \n");
  while( (num_char=read(0,ch,MAXLINE)) > 0 )
    if ( write(sfd,ch,num_char) < num_char)
      OOPS("writing");
    close(s);

    return 1;
  }
   else if(fork_return==-1)
  {
    printf("ERROR: \n");
    return 1;
  }
}
  return 0;
} 
