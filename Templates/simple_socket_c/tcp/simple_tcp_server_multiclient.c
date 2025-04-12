/*
	Simple tcp server (connect to multiple clients)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#define BUFLEN	1024	//Max length of buffer

void die(char *s)
{
	perror(s);
	exit(1);
}
 
int main(int argc, char **argv)
{
	struct sockaddr_in si_me, si_other;

	int s, slen = sizeof(si_other), recv_len, conn, portno;
	char buf[BUFLEN];
	pid_t pid;
	
	/* check command line arguments */
	if (argc != 2) {
		fprintf(stderr,"usage: %s <port>\n", argv[0]);
		exit(0);
	}
     
	/* create a TCP socket */
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		die("socket");
    
	/* zero out the structure */
	memset((char *) &si_me, 0, sizeof(si_me));
	portno = atoi(argv[1]);
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(portno);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
	/* bind socket to port */
	if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
		die("bind");
	
	/* allow 5 requests to queue up */ 
	if (listen(s, 5) == -1)
		die("listen");
     
	/* keep listening for data */
	while (1) {
		printf("Waiting a connection...");
		fflush(stdout);
		
		conn = accept(s, (struct sockaddr *) &si_other, (socklen_t *)&slen);
		if (conn < 0)
			die("accept");
			
		if ((pid = fork()) < 0)
			die("fork");
		
		/* child process, keep a connection to a client */
		if (pid == 0) {
			printf("Client connected: %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

			while (1) {
				memset(buf, 0, sizeof(buf));
				
				/* try to receive some data, this is a blocking call */
				recv_len = read(conn, buf, BUFLEN);
				if (recv_len < 0)
					break;
					
				/* print details of the client/peer and the data received */
				if (recv_len > 0) {
					printf("Data: %s\n" , buf);
				 
					/* now reply the client with the same data */
					if (write(conn, buf, strlen(buf)) < 0)
						break;
				}
			}
			
			/* close the connection */
			close(conn);
		}
	}

	close(s);
	return 0;
}
