#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFLEN 1024
#define PORT 8000

void die(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <IP do servidor> <arquivo.txt>\n", argv[0]);
        exit(1);
    }

    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFLEN];

    FILE *file = fopen(argv[2], "r");
    if (!file) die("fopen");

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) die("socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        die("connect");

    size_t bytes;
    while ((bytes = fread(buffer, 1, BUFLEN, file)) > 0) {
        send(sock, buffer, bytes, 0);
    }

    printf("Arquivo enviado com sucesso.\n");

    fclose(file);
    close(sock);
    return 0;
}
