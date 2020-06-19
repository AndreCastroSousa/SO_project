#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "moving.h"
#include "reading.h"

//Sending an Internet Domain Datagram
/*
* Here I send a datagram to a receiver whose name I get from the
* command line arguments. The form of the command line is:
* dgramsend hostname portnumber
*/
void main(int argc, char* argv[])
{
  int sock;
  struct sockaddr_in name;
  struct addrinfo *hp,hints;
  int surroundings[8];

  //struct hostent *hp, *gethostbyname();
  /* Create socket on which to send. */
  sock = socket(AF_INET,SOCK_DGRAM, 0);
  if (sock == -1) {
    perror("opening datagram socket");
    exit(1);
  }
  /*
  * Construct name, with no wildcards, of the socket to ‘‘send’’
  * to. gethostbyname returns a structure including the network
  * address of the specified host. The port number is taken from
  * the command line.
  */
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;

  int res = getaddrinfo(argv[1],argv[2],&hints,&hp);
  if (res != 0) {
    fprintf(stderr, "%s: getting address error\n", argv[1]);
    exit(2);
  }

  char input[2]; // variável que guarda o input feito pelo utilizador
  int n,len;
  char buffer[1024];
  struct sockaddr_in server;

  /*
  * Loop de modo a que o utilizador execute a quantidade de vezes que pretende
  * até que seja inserido "q"
  */
  do{

    cbreak(); // Colocar o terminal no modo cbreak ou rare
    input[0] = getchar(); // recebe o input dado pelo utilizador no terminal
    nocbreak(); // Por o terminal de volta no modo cooked
    input[sizeof(input)] = '\0';

    /* Send message. */
    if (sendto(sock,input, sizeof(input) ,0, hp->ai_addr,hp->ai_addrlen) == -1)
      perror("sending datagram message");

    //receiving a message from the server...
    n = recvfrom(sock, (int *) surroundings, 64,
                  MSG_WAITALL, (struct sockaddr *) &server,
                  &len);
    char ** imprimir = malloc(sizeof(char *)*2);

    imprimir[0]= "vazio";
    imprimir[1]= "parede";

    printf(">>>\nNORTE:%s\nSUL:%s\nOESTE:%s\nESTE:%s\n>>>\n", imprimir[surroundings[5]], imprimir[surroundings[6]],imprimir[surroundings[4]],imprimir[surroundings[3]]);



  }while(input[0]!='q' && input[0] != 'Q'); //Condição de saída do loop, caso o utilizador seleciona a letra Q o programa é finalizado.


  close(sock);
  exit(0 );
}
