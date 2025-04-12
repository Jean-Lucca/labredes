/* 
	simple TCP client
	usage: tcpclient <host> <port>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h> 

#define BUFSIZE 1024

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char **argv) {
	int sockfd, portno, n;
	struct sockaddr_in serveraddr;
	struct hostent *server;
	char *hostname;
	char buf[BUFSIZE];

	/* check command line arguments */
	if (argc != 3) {
		fprintf(stderr,"usage: %s <hostname> <port>\n", argv[0]);
		exit(0);
	}
	hostname = argv[1];
	portno = atoi(argv[2]);

	/* socket: create the socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) 
		die("socket");

	/* gethostbyname: get the server's DNS entry */
	server = gethostbyname(hostname);
	if (server == NULL)
		die("gethostbyname");

	/* build the server's Internet address */
	memset((char *) &serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(portno);

	if (inet_aton(hostname, &serveraddr.sin_addr) == 0)
		die("inet_aton");

	/* connect: create a connection with the server */
	if (connect(sockfd, (const struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
		die("connect");

	while (1) {
		/* get message line from the user */
		printf("enter message: ");
		memset(buf, 0, BUFSIZE);
		fgets(buf, BUFSIZE, stdin);
		
		if (!strcmp(buf, "exit")) break;

		/* send the message line to the server */
		n = write(sockfd, buf, strlen(buf));
		if (n < 0) 
			die("write");

		/* print the server's reply */
		memset(buf, 0, BUFSIZE);
		n = read(sockfd, buf, BUFSIZE);
		if (n < 0) 
			die("read");
		printf("echo from server: %s", buf);
	}
	close(sockfd);
	
	return 0;
}
