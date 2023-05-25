#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_socket;
    struct sockaddr_in server_address, client_address;
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);
    char message[1024];
    memset(message, 0, sizeof(message));

    socklen_t client_address_len = sizeof(client_address);
    ssize_t received_bytes = recvfrom(server_socket, message, sizeof(message), 0, (struct sockaddr*) &client_address, &client_address_len);
    if (received_bytes < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }

    int a, b;
    char op;
    sscanf(message, "%d %c %d", &a, &op, &b);

    int result;
    if(op == '+')
    	result = a+b;
    else if(op == '-')
    	result = a-b;
    else if(op == '/')
    	result = a/b;
    else if(op == '*')
    	result = a*b;
    else if(op == '%')
    	result = a%b;
    else{
    	perror("invalid operation");
        exit(EXIT_FAILURE);
    }
    char response[1024];
    memset(response, 0, sizeof(response));
    sprintf(response, "%d", result);

    ssize_t sent_bytes = sendto(server_socket, response, strlen(response), 0, (struct sockaddr*) &client_address, client_address_len);
    if (sent_bytes < 0) {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }
    close(server_socket);
    return 0;
}

