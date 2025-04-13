/*
    UDP Client - envia arquivo para o servidor
*/
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
	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];

	if (argc != 4) {
		printf("Uso: %s <IP do servidor> <porta> <arquivo.txt>\n", argv[0]);
		return -1;
	}

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		die("socket");

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(atoi(argv[2]));

	if (inet_aton(argv[1], &si_other.sin_addr) == 0)
		die("inet_aton");

	FILE *fp = fopen(argv[3], "rb");
	if (!fp) die("fopen");

	while (!feof(fp)) {
		int bytes_read = fread(buf, 1, BUFLEN, fp);
		if (sendto(s, buf, bytes_read, 0, (struct sockaddr *) &si_other, slen) == -1)
			die("sendto");
		usleep(1000); // delay opcional
	}

	// Envia "EOF" para indicar fim do arquivo
	strcpy(buf, "EOF");
	if (sendto(s, buf, strlen(buf), 0, (struct sockaddr *) &si_other, slen) == -1)
		die("sendto - EOF");

	printf("Arquivo enviado com sucesso.\n");

	fclose(fp);
	close(s);
	return 0;
}
