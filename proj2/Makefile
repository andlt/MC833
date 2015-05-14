CC=gcc
CFLAGS= -Wall -lm -std=c99
DEBUGFLAGS= -g -O0 -fno-inline -std=c99
OPTFLAGS= -O2

#client: clean
client: COMPILER=$(CC)
client: CFLAGS += $(OPTFLAGS)
client: client1

#client-debug: clean
client-debug: COMPILER=$(CC)
client-debug: CFLAGS += $(DEBUGFLAGS)
client-debug: client1

client1: client.o
	$(COMPILER) $(CFLAGS) -o client.o -c client.c

#server: clean
server: COMPILER=$(CC)
server: CFLAGS += $(OPTFLAGS)
server: server1

#server-debug: clean
server-debug: COMPILER=$(CC)
server-debug: CFLAGS += $(DEBUGFLAGS)
server-debug: server1

server1: server.o
	$(COMPILER) $(CFLAGS) -o server.o -c server.c

#clean: 
#	rm -rf *.o
