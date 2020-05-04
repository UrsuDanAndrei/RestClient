CC=gcc
CFLAGS=-I.

build: client

client: client.o requests.o helpers.o buffer.o parson.o stdin_commands.o message_parser.o server_commands.o
	$(CC) -o client client.o requests.o helpers.o buffer.o parson.o stdin_commands.o message_parser.o server_commands.o -Wall

client.o: client.c
	$(CC) -c -Wall client.c

requests.o: requests.c
	$(CC) -c -Wall requests.c

helpers.o: helpers.c
	$(CC) -c -Wall helpers.c

buffer.o: buffer.c
	$(CC) -c -Wall buffer.c

parson.o: parson.c
	$(CC) -c -Wall parson.c

stdin_commands.o: stdin_commands.c
	$(CC) -c -Wall stdin_commands.c

message_parser.o: message_parser.c
	$(CC) -c -Wall message_parser.c

server_commands.o: server_commands.c
	$(CC) -c -Wall server_commands.c

run: client
	./client

clean:
	rm -f *.o client
