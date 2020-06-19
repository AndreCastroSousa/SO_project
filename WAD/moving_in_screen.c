#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#define COLUMNS 67
#define ROWS 32

struct termios oldtc;
struct termios newtc;


//   vetor info -> [x, y, direcao]
//   i.e:
//   | x | y | 10 |, lembrando que pos=[x,y]

int info[4];

//int surroundings[8];  // vetor usado para guardar informação sobre o que se passa /////////////////////////////
                    // à volta do rato
                    // [pos atual, direcao, frente, esquerda, direita, atras]
                    // Valores:               1: parede
                    //                        2: start
                    //                        3: finish
                    //                        0: vazio
                    //                      x,y: posicao atual
                    //   Mapeamento coordenadas: N: 10, S: 20, E: 30, O: 40
                    //                 formato:  | y | x | direcao | ESTE | OESTE | NORTE | SUL |
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Cria estruturas termios para armazenar os parametros e
informações do terminal*/

void clearscreen(){
  /*Esta função faz uso das ANSI escape sequences,
  sequências de bytes que ao serem impressas,
  são interpretadas pelo terminal como comandos
  e não como caratéres.*/
  //printf("\033[2J");
  system("clear");  // foi utilizado este com o objetivo de corrigir um bug

}

void cbreak() {
  /*
  Esta função coloca o terminal no modo cbreak ou rare,
  no qual o terminal interpreta um carater de cada vez,
  mas continua a reconhecer combinações de teclas de controlo,
  como Ctrl-C.
  */
  tcgetattr(STDIN_FILENO, &oldtc);
  /*Obtẽm os parâmetros/atributos associados ao
  terminal e coloca-os na struct termios oldtc*/
  newtc = oldtc; //Atribui o valor de oldtc a newtc.
  newtc.c_lflag &= ~(ICANON | ECHO);
  /*Modifica a flag c_lflag que controla o modo do terminal,
  e efetua um bitwise-and com o bitwise-not do bitwise-or das constantes ICANON
  e ECHO, efetivamente definindo o modo não-canónico e a não-ecoação dos carateres
  introduzidos.
  Com o modo canónico desativado, a input do utilizador é dada caratér a carater,
  sem necessidade de delimitadores como newline, entre outras coisas.
  Com ECHO desativado, os carateres introduzidos não são ecoados, ou escritos,
  no ecrã.*/
  tcsetattr(STDIN_FILENO, TCSANOW, &newtc);
  /*Define os atributos do terminal tal como definidos em newtc,
  ou seja, desativa o modo canónico e o eco*/
}

void nocbreak() {
  tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
  /*Repõe os atributos do terminal para aqueles obtidos no início do programa e
  guardados em oldtc.*/
  fflush(stdout);
}



////////////////////    funcao para calcular surroundings a medida que se movimenta/////////////////////////////
//                                                                                                            //
// Valores:               1: parede                                                                           //
//                        2: start                                                                            //
//                        3: finish                                                                           //
//                        0: vazio                                                                            //
//                      x,y: posicao atual                                                                    //
//                                                                                                            //
//   Mapeamento coordenadas: N: 10, S: 20, E: 30, O: 40                                                       //
//  formato:  | y | x | direcao | frente | atras | esquerda | direita |                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void sense(int **maze, int *pos, int *surroundings){

  int y = pos[0];
  int x = pos[1];

  surroundings[0] = pos[0];
  surroundings[1] = pos[1];
  surroundings[2] = pos[2];
  surroundings[3] = maze[x-1][y+1-1];
  surroundings[4] = maze[x-1][y-1-1];
  surroundings[5] = maze[x+1-1][y-1];
  surroundings[6] = maze[x-1-1][y-1];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char cursor_init(int * pos){
  printf("\033[%d;%dH^",ROWS+2-pos[1],pos[0]);
  return 10;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int *left(int **maze, int *pos, int *info){

  if(info[2] == 10){
    info[2] = 40;
    printf("\033[%d;%dH<",ROWS+2-pos[1],pos[0]);
  }
  else if(info[2] == 20){
    info[2] = 30;
    printf("\033[%d;%dH>",ROWS+2-pos[1],pos[0]);
  }
  else if(info[2] == 30){
    info[2] = 10;
    printf("\033[%d;%dH^",ROWS+2-pos[1],pos[0]);
  }
  else{
    info[2] = 20;
    printf("\033[%d;%dHv",ROWS+2-pos[1],pos[0]);
  }
  info[0] = pos[0];
  info[1] = pos[1];
  return info;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int *right(int **maze, int *pos, int *info){

  if(info[2] == 10){
    info[2] = 30;
    printf("\033[%d;%dH>",ROWS+2-pos[1],pos[0]);
  }
  else if(info[2] == 20){
    info[2] = 40;
    printf("\033[%d;%dH<",ROWS+2-pos[1],pos[0]);
  }
  else if(info[2] == 30){
    info[2] = 20;
    printf("\033[%d;%dHv",ROWS+2-pos[1],pos[0]);
  }
  else{
    info[2] = 10;
    printf("\033[%d;%dH^",ROWS+2-pos[1],pos[0]);
  }
  info[0] = pos[0];
  info[1] = pos[1];
  return info;
}

/////////////info/////////////////////////////////////////////////////////////////////////////////////////

int *forward(int **maze, int *pos, int *info){

  if(info[2] == 10){
    pos[1]++;                            //soma 1 à posição anterior
    if (maze[pos[1]-1][pos[0]-1] != 1){
      pos[1]--;
      printf("\033[%d;%dH ",ROWS+2-pos[1],pos[0]);
      pos[1]++;
      printf("\033[%d;%dH^",ROWS+2-pos[1],pos[0]);             // movimenta para cima
    }
    else{
      pos[1]--;                          // caso a posição seguinte for uma parede é subtraido 1
    }                                    // à posição atual
    info[0] = pos[0];
    info[1] = pos[1];
    return info;                          // retorna a posição atual
  }
//////////////////////////////////////////////////////

  else if(info[2] == 20){
    pos[1]--;
    if (maze[pos[1]-1][pos[0]-1] != 1){
      pos[1]++;
      printf("\033[%d;%dH ",ROWS+2-pos[1],pos[0]);
      pos[1]--;
      printf("\033[%d;%dHv",ROWS+2-pos[1],pos[0]);            // movimenta para baixo
    }
    else{
      pos[1]++;
    }
    info[0] = pos[0];
    info[1] = pos[1];
    return info;
  }
//////////////////////////////////////////////////////

  else if(info[2] == 30){
    pos[0]++;
    if (maze[pos[1]-1][pos[0]-1] != 1){
      pos[0]--;
      printf("\033[%d;%dH ",ROWS+2-pos[1],pos[0]);
      pos[0]++;
      printf("\033[%d;%dH>",ROWS+2-pos[1],pos[0]);             // movimenta para a direita
    }
    else{
      pos[0]--;
    }
    info[0] = pos[0];
    info[1] = pos[1];
    return info;
  }
////////////////////////////////////////////////////////

  else{
    pos[0]--;
    if (maze[pos[1]-1][pos[0]-1] != 1){
      pos[0]++;
      printf("\033[%d;%dH ",ROWS+2-pos[1],pos[0]);
      pos[0]--;
      printf("\033[%d;%dH<",ROWS+2-pos[1],pos[0]);           // movimenta para a esquerda
    }
    else{
      pos[0]++;
    }
    info[0] = pos[0];
    info[1] = pos[1];
    return info;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int *mapa(int **maze){
  int *pos;
  pos = malloc(sizeof(int*) * 2);

  for(int i=ROWS;i>=0;i--){           // calcula a posição do S (posição inicial)
    for(int j = 0; j < COLUMNS; j++){
      if (maze[i][j] == 2){
        pos[0] = i+2;
        pos[1] = j;
      }
    }
  }
  info[0] = pos[0];
  info[1] = pos[1];
  info[2] = cursor_init(pos);
  return info;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cursor_pos(int *pos){
    printf("\033[%d;%dH",ROWS+2-pos[1],pos[0]);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int *mov_screen(int ** maze, int * pos, char buf){
  switch(buf){
    case 'w'://"cima"
    case 'W':
      pos = forward(maze, pos, info);
      // em falta fazer clear da posicao anterior do cursor
      break;
    case 'd'://"direita"
    case 'D':
      pos = right(maze, pos, info);
      // em falta fazer clear da posicao anterior do cursor
      break;
    case 'a'://"esquerda"
    case 'A':
      pos = left(maze, pos, info);
      // em falta fazer clear da posicao anterior do cursor
      break;
    }
    return pos;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
