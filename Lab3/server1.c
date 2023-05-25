#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, cli_addr;
    socklen_t cli_addr_len = sizeof(cli_addr);
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
    	perror("bind failed");
    	exit(EXIT_FAILURE);
    	}
    if (listen(server_socket, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);
    if ((client_socket = accept(server_socket, (struct sockaddr*)&cli_addr, &cli_addr_len)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected by %s\n", inet_ntoa(cli_addr.sin_addr));
    char message[1024];
    memset(message, 0, sizeof(message));
    recv(client_socket, message, sizeof(message), 0);
    for (int i = 0; i < strlen(message); i++) {
         if (message[i] >= 'a' && message[i] <= 'z')
             message[i] = message[i] - 32;
         else if (message[i] >= 'A' && message[i] <= 'Z')
             message[i] = message[i] + 32;
    }
    printf("Reversed message: %s\n", message);
    send(client_socket, message, strlen(message), 0);
    close(client_socket);
    close(server_socket);
    return 0;
}
