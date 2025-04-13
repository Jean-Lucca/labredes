#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8000
#define BUFLEN 1024

void die(char *msg) {
    perror(msg);
    exit(1);
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFLEN];

    FILE *output = fopen("recebido_tcp.txt", "w");
    if (!output) die("fopen");

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) die("socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        die("bind");

    if (listen(server_sock, 1) < 0) die("listen");

    printf("Aguardando conexão TCP na porta %d...\n", PORT);
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
    if (client_sock < 0) die("accept");

    printf("Cliente conectado: %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    ssize_t received;
    while ((received = recv(client_sock, buffer, BUFLEN, 0)) > 0) {
        fwrite(buffer, 1, received, output);
    }

    printf("Arquivo recebido com sucesso.\n");

    fclose(output);
    close(client_sock);
    close(server_sock);
    return 0;
}
