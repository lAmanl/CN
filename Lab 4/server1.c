#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#define PORT 8080
int main(){
    int server_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    if((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);
    char message[1024];
    //memset(message, 0, sizeof(message));
    ssize_t received_bytes = recvfrom(server_socket, message, sizeof(message), 0,
                                      (struct sockaddr*) &client_address, &client_address_len);
    if (received_bytes < 0) {
        perror("recv from failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to %s\n", inet_ntoa(client_address.sin_addr));
    printf("Reverse of message received from client: ");
    for(int i = strlen(message) - 1; i >= 0; i--){
        printf("%c", message[i]);
    }
    printf("\n");
    ssize_t sent_bytes = sendto(server_socket, message, strlen(message), 0,
                                (struct sockaddr*) &client_address, client_address_len);
    if (sent_bytes < 0) {
        perror("send failed");
        exit(EXIT_FAILURE);
    }
    close(server_socket);
    return 0;
}
