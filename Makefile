CC      = gcc
CFLAGS  = -Wall
LDFLAGS = -lm

	objects = ConexaoRawSocket.o 

all: server user

server: server.c ConexaoRawSocket.o
	$(CC) $(CFLAGS) server.c -o server ConexaoRawSocket.o $(LDFLAGS)

user: user.c ConexaoRawSocket.o
	$(CC) $(CFLAGS) user.c -o user ConexaoRawSocket.o 

ConexaoRawSocket.o:
	$(CC) $(CFLAGS) -c ConexaoRawSocket.c 

purge: clean
	rm -f server user

clean:
	rm -f $(objects)