# SO_project
Projeto de SO (Grupo 2)

########################## Vídeo ##################################  
Link para o video : https://www.youtube.com/watch?v=tzhYW9T5Lbk

########################### Descrição ###############################  

Este projeto visa a construir o programa micromouse. O micromouse é um evento onde um robot resolve um labirinto 16x16, no nosso caso usamos labirintos maiores que se podem encontrar na Pasta "Maze" que está dentro da cada Pasta que corresponde aos diferentes modos de jogos (4 teclas e 3 teclas) respetivamente "WASD" e "WAD".
O programa usa sockets UDP que estão definidos como cliente ("client_datagram.c") e servidor("server_datagram.c"). Onde o cliente envia os comandos de movimento do cursor atraves do socket. 
O servidor funciona em dois modos sendo o modo habitual de 4 teclas(W,A,S,D) e o modo de 3 teclas (virar à direita(A), virar à esquerda(D) e ir em frente(W)). A execucação do servidor é feita com 2 threads onde são armazenados os comandos pela primeira thread  e onde são retirados para execução na segunda thread de modo atualizar o movimento do cursor.
Com os comandos de teclas no cliente observa-se o movimento do cursor no simulador do servidor, já na parte do cliente são recebidos os dados que identificam as casas ao redor do cursor, isto é paredes e espaço vazio.


###################### Funcionamento ##################################  
Como escolher o labirinto:
- O labirinto pode ser escolhido na pasta "mazes";
- O labirinto tem que ser renomeado para "test.txt" e colocado na pasta do projeto;

Como alternar entre os modos 3 e 4 teclas:
- Foram criadas duas pastas separadas, "WAD" e "WASD" para os dois diferentes modos, três e quatro direções respetivamente;

Como compilar o programa:

- Usar o comando "make server" para compilar o servidor;
- Usar o comando "make client" para compilar o cliente;

Como correr o servidor e cliente:

- Num dos terminais utilizar o comando "./server" para correr o servidor; (NOTA: A porta utilizada é sempre 8010)
- No outro terminal utilizar o comando "./client 127.0.0.1 8010" para conectar ao servidor;
- Pressionar a tecla "W" para efetuar a ligação;
- Desconectar utilizando "CTRL-Z" e reconectar utilizando o mesmo comando "./client 127.0.0.1 8010";
  

########################## Grupo ###############################  
Trabalho feito por: 
- André Sousa 
- Bruno Viveiros 
- Gonçalo Almeida
