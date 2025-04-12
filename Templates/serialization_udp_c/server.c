/*
    Simple udp server 
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "person.h"

#define BUFLEN	512		// Max length of buffer

void die(char *s)
{
	perror(s);
	exit(1);
}
 
int main(int argc, char **argv)
{
	struct sockaddr_in si_me, si_other;
	int s, slen = sizeof(si_other) , recv_len;
	char buf[BUFLEN];
	struct person_s *person = (struct person_s *)&buf;
	
	if (argc != 2) {
		printf("Usage: %s <port>\n", argv[0]);
		
		return -1;
	}
     
	/* create a UDP socket */
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket");
    
	/* zero out the structure */
	memset((char *) &si_me, 0, sizeof(si_me));
     
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(atoi(argv[1]));
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
	/* bind socket to port */
	if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
		die("bind");
     
	/* keep listening for data */
	while (1) {
		memset(buf, 0, sizeof(buf));
		printf("Waiting for data...");
		fflush(stdout);
         
		/* try to receive some data, this is a blocking call */
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t *)&slen)) == -1)
			die("recvfrom()");

		/* print details of the client/peer and the data received */
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Name: %s, Birth year: %d, Occupation: %s\n", person->name, ntohs(person->birth_year), person->occupation);
	}

	close(s);
	return 0;
}
