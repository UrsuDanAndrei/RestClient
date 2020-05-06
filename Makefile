CC=gcc
CFLAGS=-I.

build: client

client: client.o requests.o helpers.o buffer.o parson.o stdin_commands.o message_parser.o server_commands.o
	$(CC) -o client client.o requests.o helpers.o buffer.o parson.o stdin_commands.o message_parser.o server_commands.o -g -Wall

client.o: client.c
	$(CC) -c -Wall -g client.c

requests.o: requests.c
	$(CC) -c -Wall -g requests.c

helpers.o: helpers.c
	$(CC) -c -Wall -g helpers.c

buffer.o: buffer.c
	$(CC) -c -Wall -g buffer.c

parson.o: parson.c
	$(CC) -c -Wall -g parson.c

stdin_commands.o: stdin_commands.c
	$(CC) -c -Wall -g stdin_commands.c

message_parser.o: message_parser.c
	$(CC) -c -Wall -g message_parser.c

server_commands.o: server_commands.c
	$(CC) -c -Wall -g server_commands.c

run: client
	./client

clean:
	rm -f *.o client
