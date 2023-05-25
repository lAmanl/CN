#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define PORT 8080
int main(){
    int client_socket;
    struct sockaddr_in server_address;
    if((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    char message[1024] = "Hello, from client";
    ssize_t sent_bytes = sendto(client_socket, message, strlen(message), 0, (struct sockaddr*) &server_address, sizeof(server_address));
    printf("Message sent by the client: %s\n", message);     
    if (sent_bytes < 0) {
        perror("sendto failed");
        exit(EXIT_FAILURE);
    }
    char reversed_message[1024];
    memset(reversed_message, 0, sizeof(reversed_message));
    ssize_t received_bytes = recvfrom(client_socket, reversed_message, sizeof(reversed_message), 0, NULL, NULL);
    if (received_bytes < 0) {
        perror("recvfrom failed");
        exit(EXIT_FAILURE);
    }
    close(client_socket);
    return 0;
}
