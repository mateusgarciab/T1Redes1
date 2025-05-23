CC      = gcc
CFLAGS  = -Wall
LDFLAGS = -lm

	objects = ./obj/ConexaoRawSocket.o ./obj/mensagem.o ./obj/arquivo.o ./obj/rede.o

all: server user

server: server.c $(objects)
	$(CC) $(CFLAGS) server.c -o server $(objects) $(LDFLAGS)

user: user.c $(objects)
	$(CC) $(CFLAGS) user.c -o user $(objects)

./obj/ConexaoRawSocket.o:
	$(CC) $(CFLAGS) -c ConexaoRawSocket.c -o ./obj/ConexaoRawSocket.o

./obj/mensagem.o:
	$(CC) $(CFLAGS) -c mensagem.c -o ./obj/mensagem.o

./obj/arquivo.o:
	$(CC) $(CFLAGS) -c arquivo.c -o ./obj/arquivo.o

./obj/mapa.o:
	$(CC) $(CFLAGS) -c mapa.c -o ./obj/mapa.o

./obj/rede.o:
	$(CC) $(CFLAGS) -c rede.c -o ./obj/rede.o

purge: clean
	rm -f server user teste arquivo

clean:
	rm -f $(objects)