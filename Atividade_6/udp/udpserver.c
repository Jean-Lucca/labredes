#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFLEN 512

void die(char *s)
{
	perror(s);
	exit(1);
}

int main(int argc, char **argv)
{
	struct sockaddr_in si_me, si_other;
	int s, slen = sizeof(si_other), recv_len;
	char buf[BUFLEN];

	if (argc != 2) {
		printf("Uso: %s <porta>\n", argv[0]);
		return -1;
	}

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket");

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(atoi(argv[1]));
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
		die("bind");

	FILE *fp = fopen("recebido_udp.txt", "wb");
	if (!fp) die("fopen");

	printf("Aguardando dados UDP...\n");

	while (1) {
		memset(buf, 0, BUFLEN);
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t *)&slen)) == -1)
			die("recvfrom");

		// Verifica se é o fim da transmissão
		if (strncmp(buf, "EOF", 3) == 0) {
			printf("Fim da transmissão.\n");
			break;
		}

		fwrite(buf, 1, recv_len, fp);
		printf("Recebido %d bytes de %s:%d\n", recv_len, inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
	}

	fclose(fp);
	close(s);
	return 0;
}
