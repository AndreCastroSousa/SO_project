########################Makefile##############################
server: mythreads.o moving_in_screen.o reading_a_file.o
	gcc -o server server_datagram.c moving_in_screen.o reading_a_file.o mythreads.o -pthread

moving_in_screen.o: 
	gcc -c moving_in_screen.c  #criado os ficheiros .o que são chamados pelo executável 

reading_a_file.o:
	gcc  -c reading_a_file.c 

mythreads.o:
	gcc -c mythreads.c

client: moving_in_screen.o
	gcc -o client client_datagram.c moving_in_screen.o

clean:
	rm -f moving_in_screen.o reading_a_file.o mythreads.o # apagar os ficheiros criados, antes de fazer o make vai.

