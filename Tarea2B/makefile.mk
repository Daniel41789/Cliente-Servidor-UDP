CC=gcc
CFLAGS=-Wall

all: client_udp server_udp

client_udp: client_udp.c
	$(CC) $(CFLAGS) -o client_udp client_udp.c

server_udp: server_udp.c
	$(CC) $(CFLAGS) -o server_udp server_udp.c

clean:
	rm -f client_udp server_udp
