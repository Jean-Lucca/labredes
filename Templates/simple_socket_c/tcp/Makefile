CC      = gcc
CFLAGS  = -O2 -Wall

all:
	$(CC) $(CFLAGS) simple_tcp_server.c -o simple_tcp_server
	$(CC) $(CFLAGS) simple_tcp_server_multiclient.c -o simple_tcp_server_multiclient
	$(CC) $(CFLAGS) simple_tcp_client.c -o simple_tcp_client
	$(CC) $(CFLAGS) simple_http_server.c -o simple_http_server
	$(CC) $(CFLAGS) simple_http_server_multiclient.c -o simple_http_server_multiclient
	
clean:
	-rm -f *.o *~ simple_tcp_server simple_tcp_server_multiclient simple_tcp_client simple_http_server simple_http_server_multiclient
