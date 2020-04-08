#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/* Display error message on stderr and then exit. */
#define OOPS(msg)       {perror(msg); exit(1);}

#define MAXLINE 512

int main(int argc, char *argv[])
{
  struct sockaddr_in bba; /* socket information */
  struct hostent *hp;     /* host information */
  int slen;               /* host computer */
  int s;                  /* length socket address */
  int rfd;
  char ch[MAXLINE];       /* character for i/o */
  int num_char=MAXLINE;   /* number of characters */
  int port;            /* port to connect to */
  char portnum[20];
  char hostname[20];
  char user[25];
    
  printf("\n hostname:");
  scanf("%s", hostname);

  printf("\n port number:");
  scanf("%s", portnum);
    
    printf("\n Enter a username: ");
    scanf("%s",user);

  // Creating the socket connection
  memset(&bba,0,sizeof(bba));
  bba.sin_family = AF_INET;
  hp=gethostbyname(hostname);
  port=atoi(portnum);
  memcpy(&bba.sin_addr, hp->h_addr, hp->h_length);
  bba.sin_port=htons(port);

  

  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == -1)
    OOPS("socket");
  if(connect(s,(struct sockaddr *)&bba,sizeof(bba)) != 0)
    OOPS("connect");
 

  pid_t fork_return;
  fork_return = fork();
  if(fork_return ==0) // Child
  {
      int first = 0;
      printf("Press enter to join \n");
      while( (num_char=read(0,ch,MAXLINE)) > 0 ){
          while(first==0)                    // displays username to the server
          {
              write(s,user,25);
              first = 1;
          }
    if ( write(s,ch,num_char) < num_char)
      OOPS("writing");
      }
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
   else if(fork_return==-1)
  {
    printf("ERROR: \n");
    return 1;
  }

  return 0;
}
