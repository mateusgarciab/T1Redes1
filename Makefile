CC      = gcc
CFLAGS  = -Wall
LDFLAGS = -lm

	objects = ConexaoRawSocket.o mensagem.o arquivo.o rede.o

all: server user

server: server.c $(objects)
	$(CC) $(CFLAGS) server.c -o server $(objects) $(LDFLAGS)

user: user.c $(objects)
	$(CC) $(CFLAGS) user.c -o user $(objects)

ConexaoRawSocket.o:
	$(CC) $(CFLAGS) -c ConexaoRawSocket.c 

mensagem.o:
	$(CC) $(CFLAGS) -c mensagem.c

arquivo.o:
	$(CC) $(CFLAGS) -c arquivo.c

mapa.o:
	$(CC) $(CFLAGS) -c mapa.c

rede.o:
	$(CC) $(CFLAGS) -c rede.c

purge: clean
	rm -f server user teste arquivo

clean:
	rm -f $(objects)