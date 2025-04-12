CC      = gcc
CFLAGS  = -O2 -Wall

all:
	$(CC) $(CFLAGS) server.c -o server
	$(CC) $(CFLAGS) client.c -o client

clean:
	-rm -f *.o *~ server client
