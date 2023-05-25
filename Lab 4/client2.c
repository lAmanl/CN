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

    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    char message[1024];
    memset(message, 0, sizeof(message));

    int num1, num2;
    char operator;

    printf("Enter the first number: ");
    scanf("%d", &num1);

    printf("Enter the second number: ");
    scanf("%d", &num2);

    printf("Enter an operator (+, -, *, /, or %%): ");
    scanf(" %c", &operator);

    sprintf(message, "%d %d %c", num1, num2, operator);

    ssize_t sent_bytes = sendto(client_socket, message, strlen(message), 0,
                                (struct sockaddr*) &server_address, sizeof(server_address));

    if (sent_bytes < 0) {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }

    memset(message, 0, sizeof(message));

    socklen_t server_address_len = sizeof(server_address);
    ssize_t received_bytes = recvfrom(client_socket, message, sizeof(message), 0,
                                       (struct sockaddr*) &server_address, &server_address_len);

    if (received_bytes < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    int result;
    sscanf(message, "%d", &result);

    printf("Result: %d\n", result);

    close(client_socket);

    return 0;
}

