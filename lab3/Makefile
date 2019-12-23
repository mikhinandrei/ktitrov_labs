CC= gcc
CFLAGS= -Wall

targets: client server

message.o: message.h message.c
	$(CC) $(CFLAGS) -c message.c -o $@

client.o: client.c message.h
	$(CC) $(CFLAGS) -c client.c -o $@

server.o: server.c message.h
	$(CC) $(CFLAGS) -c server.c -o $@


client: client.o message.o message.h
	$(CC) $(CFLAGS) client.o message.o -o $@

server: server.o message.o message.h
	$(CC) $(CFLAGS) server.o message.o -o $@
