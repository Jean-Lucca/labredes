CC      = gcc
CFLAGS  = -O2 -Wall

all:
	$(CC) $(CFLAGS) simple_udp_server.c -o simple_udp_server
	$(CC) $(CFLAGS) simple_udp_server2.c -o simple_udp_server2
	$(CC) $(CFLAGS) simple_udp_client.c -o simple_udp_client
	$(CC) $(CFLAGS) simple_udp_client2.c -o simple_udp_client2
clean:
	-rm -f *.o *~ simple_udp_server simple_udp_server2 simple_udp_client simple_udp_client2
