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

  /* Clear the data structure (saddr) to 0's. */
  memset(&bba,0,sizeof(bba));

  /* Tell our socket to be of the internet family (AF_INET). */
  bba.sin_family = AF_INET;

  /* Acquire the ip address of the server */
  hp=gethostbyname(hostname);

  /* Acquire the port #. */
  port=atoi(portnum);

  /* Copy the server's address to the socket address data structure. */
  memcpy(&bba.sin_addr, hp->h_addr, hp->h_length);

  /* Convert the integer Port Number to the network-short standard
   * required for networking stuff. This accounts for byte order differences.*/
  bba.sin_port=htons(port);
  
  /* Now that we have our data structure full of useful information,
   * open up the socket the way we want to use it.
   */

  

  s = socket(AF_INET, SOCK_STREAM, 0);
  if(s == -1)
    OOPS("socket");
  if(connect(s,(struct sockaddr *)&bba,sizeof(bba)) != 0)
    OOPS("connect");
 

  pid_t fork_return;
  fork_return = fork();
  if(fork_return ==0)
  {
   /* read from the socket, write to the screen */
    
      while( (num_char=read(0,ch,MAXLINE)) > 0 ){
          printf("User: %s\n",user);
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
