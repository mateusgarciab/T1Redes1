CC      = gcc
CFLAGS  = -Wall
LDFLAGS = -lm

	objects = ConexaoRawSocket.o mensagem.o rede.o mapa.o cliente.o servidor.o

all: main 

main: main.c $(objects)
	$(CC) $(CFLAGS) main.c -o main $(objects) $(LDFLAGS)

ConexaoRawSocket.o:
	$(CC) $(CFLAGS) -c ConexaoRawSocket.c -o ConexaoRawSocket.o

mensagem.o:
	$(CC) $(CFLAGS) -c mensagem.c -o mensagem.o

mapa.o:
	$(CC) $(CFLAGS) -c mapa.c -o mapa.o

rede.o:
	$(CC) $(CFLAGS) -c rede.c -o rede.o

cliente.o:
	$(CC) $(CFLAGS) -c cliente.c -o cliente.o

servidor.o:
	$(CC) $(CFLAGS) -c servidor.c -o servidor.o

purge: clean
	rm -f main user teste arquivo

clean:
	rm -f $(objects)