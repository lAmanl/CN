#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int client_socket, a, b;
    struct sockaddr_in server_address;
    char message[1024], op;
    memset(message, 0, sizeof(message));
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    printf("Enter first number: ");
    scanf("%d", &a);
    printf("Enter second number: ");
    scanf("%d", &b);
    printf("Enter operator: ");
    scanf(" %c", &op);
    sprintf(message, "%d %c %d", a, op, b);

    ssize_t sent_bytes = sendto(client_socket, message, strlen(message), 0, (struct sockaddr*) &server_address, sizeof(server_address));
    if (sent_bytes < 0) {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    socklen_t server_address_len = sizeof(server_address);
    ssize_t received_bytes = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) &server_address, &server_address_len);
    if (received_bytes < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    printf("%d %c %d = %s\n", a, op, b, buffer);
    close(client_socket);
    return 0;
}

