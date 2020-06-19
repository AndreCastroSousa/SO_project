#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include "moving.h"
#include "reading.h"
#include "mythreads.h"
#define SIZE 4

char buffer[SIZE];
int count = 0, putIndex = 0, getIndex = 0;
int **maze; // apontador para o apontador da matriz
int *pos; // apontador para o endereço que guarda a posição do cursor
int surroundings[8];
//initialization
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // Um objeto que só pode pertencer a uma thread num dado momento
pthread_cond_t command_input = PTHREAD_COND_INITIALIZER; // Existência de dados no buffer

void put(char c){
  Pthread_mutex_lock(&lock);
  while (count == SIZE) {
    Pthread_cond_wait(&command_input,&lock);
  }
  count ++;
  buffer[putIndex] = c;
  putIndex++;
  if (putIndex == SIZE) {
    putIndex = 0;
  }
  Pthread_cond_signal(&command_input);
  Pthread_mutex_unlock(&lock);
}

char get(){
  char c;
  Pthread_mutex_lock(&lock);
  while (count == 0){
    Pthread_cond_wait(&command_input,&lock);
  }
  count--;
  c = buffer[getIndex];
  getIndex++;
  if (getIndex == SIZE) {
    getIndex = 0;
  }

  Pthread_cond_signal(&command_input);
  Pthread_mutex_unlock(&lock);
  return c;
}

void *myproducer(char *arg){
  put(*(char *) arg);
  return NULL;
}

void *myconsumer(void *arg){
  pos = mov_screen(maze, pos, get());
  cbreak(); // Colocar o terminal no modo cbreak ou rare
  cursor_pos(pos); // posiciona o cursor na posição atual
  nocbreak(); // Por o terminal de volta no modo cooked
  return NULL;
}

/*
* The include file <netinet/in.h> defines sockaddr_in as:
* struct sockaddr_in {
* short sin_family;
* u_short sin_port;
* struct in_addr sin_addr;
* char sin_zero[8];
* };
* This program creates a datagram socket, binds a name to it, then
* reads from the socket.
*/
void main(int argc, char* argv[])
{
  int sock, length;
  struct sockaddr_in name;
  char buf[1024];

  /* Create socket from which to read. */
  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock == -1) {
    perror("opening datagram socket");
    exit(1);
  }
  /* Create name with wildcards. */
  name.sin_family = AF_INET;
  name.sin_addr.s_addr = INADDR_ANY;
  //name.sin_addr.s_addr = inet_addr("127.0.0.1");
  //name.sin_port = 0;
  name.sin_port = htons(8010);
  if (bind(sock,(struct sockaddr *)&name, sizeof name) == -1) {
    perror("binding datagram socket");
    exit(1);
  }
  /* Find assigned port value and print it out. */
  length = sizeof(name);
  if (getsockname(sock,(struct sockaddr *) &name, &length) == -1) {
    perror("getting socket name");
    exit(1);
  }
  printf("Socket port #%d\n", ntohs(name.sin_port));
  /* Read from the socket. */
  if ( read(sock, buf, 1024) == -1 ) perror("receiving datagram packet");

  int n, len;
  struct sockaddr_in client;
  len = sizeof(client);

  char answer[10] = "ok"; // mensagem a ser enviada ao socket do cliente


  clearscreen(); // Limpar o ecrã
  cbreak(); // Colocar o terminal no modo cbreak ou rare

  maze = ler(argc, argv); //retorna a matriz
  pos = mapa(maze); // retorna a posição inical do cursor utilizando a matriz
  cursor_pos(pos); // posiciona o cursor na posição atual
  nocbreak(); // Por o terminal de volta no modo cooked
  cbreak(); // Colocar o terminal no modo cbreak ou rare

  //Read from the socket, collect info about the client and return message.
  do{
    pthread_t consumer_1,producer_1;

    // recebe no buf o input do utilizador, neste caso será algo com "W", "A", "S", "D"
    n = recvfrom(sock, (char *)buf, 1024,
                  MSG_WAITALL, ( struct sockaddr *) &client,
                  &len);
    buf[n] = '\0';

    char* comando_cliente = malloc(sizeof(char));
    strcpy(comando_cliente,buf);
    Pthread_create(&producer_1, NULL, myproducer, comando_cliente);
    Pthread_create(&consumer_1, NULL, myconsumer, NULL);

    //join waits for the threads to finish ....
    Pthread_join(consumer_1,NULL);
    //movimenta o cursor atendendo ao input introduzido pelo utilizador
    Pthread_join(producer_1,NULL);

    sense(maze,pos,surroundings);

    //Condição de vitória
    if (maze[pos[1]-1][pos[0]-1] == 3){  //Se chegar à posição do G o jogo acaba
      printf("\033[34;0HYOU WIN!");
      strcpy(answer, "You Win");
    }

    //Depois que todos os passos são executados com sucesso, o server retorna um "ok" para o utilizador
    sendto(sock, &surroundings, sizeof(int)*8,
        MSG_CONFIRM, (const struct sockaddr *) &client,
            len);

  }while(buf[0]!='q' && buf[0]!= 'Q');  //Condição de saída do loop, caso o utilizador seleciona a letra Q o programa é finalizado.

  nocbreak(); // Por o terminal de volta no modo cooked
  clearscreen(); // Limpar o ecrã

  close(sock);
  exit(0);
}
