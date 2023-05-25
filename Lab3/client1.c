#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }
    char message[1024] = "Hello, sent by the client!";
    send(client_socket, message, strlen(message), 0);
    char reversed_message[1024];
    memset(reversed_message, 0, sizeof(reversed_message));
    recv(client_socket, reversed_message, sizeof(reversed_message), 0);
    close(client_socket);
    return 0; 
}
