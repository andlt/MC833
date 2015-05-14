CC=gcc
CFLAGS= -Wall -lm
DEBUGFLAGS= -g -O0 -fno-inline
OPTFLAGS= -O2

client: clean
client: COMPILER=$(CC)
client: CFLAGS += $(OPTFLAGS)
client: client

client-debug: clean
client-debug: COMPILER=$(CC)
client-debug: CFLAGS += $(DEBUGFLAGS)
client-debug: client

client: client.o
	$(COMPILER) $(CFLAGS) -o client client.o

client.o: 
	$(COMPILER) $(CFLAGS) -o client.o -c client.c

server: clean
server: COMPILER=$(CC)
server: CFLAGS += $(OPTFLAGS)
server: server

server-debug: clean
server-debug: COMPILER=$(CC)
server-debug: CFLAGS += $(DEBUGFLAGS)
server-debug: server

server: server.o
	$(COMPILER) $(CFLAGS) -o server server.o

server.o: 
	$(COMPILER) $(CFLAGS) -o server.o -c server.c


clean: 
	rm -rf *.o
