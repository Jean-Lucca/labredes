/*
    Simple udp client
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "person.h"
 
#define BUFLEN			512
#define PORT			8888
#define UDP_TIMEOUT_SEC		0			/* socket timeout (in sec) */
#define UDP_TIMEOUT_USEC	100000			/* socket timeout (in usec) */
 
void die(char *s)
{
	perror(s);
	exit(1);
}
 
int main(int argc, char **argv)
{
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char buf2[30];
	struct person_s *person = (struct person_s *)&buf;

	if (argc != 3) {
		printf("Usage: %s <server ip> <port>\n", argv[0]);
		
		return -1;
	}
 
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket");

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(atoi(argv[2]));
	

	if (inet_aton(argv[1], &si_other.sin_addr) == 0)
		die("inet_aton");
 
	while (1) {
		printf("Name: ");
		fgets(person->name, sizeof(person->name), stdin);
		person->name[strlen(person->name) - 1] = '\0';		// remove line feed from string
		printf("Birth year: ");
		fgets(buf2, sizeof(buf2), stdin);
		person->birth_year = htons(atoi(buf2));
		printf("Occupation: ");
		fgets(person->occupation, sizeof(person->occupation), stdin);
		person->occupation[strlen(person->occupation) - 1] = '\0';
         
		/* send the message */
		if (sendto(s, buf, sizeof(struct person_s), 0, (struct sockaddr *) &si_other, slen) == -1)
			die("sendto()");
	}

	close(s);
	
	return 0;
}
