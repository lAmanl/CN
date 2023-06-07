#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define PORT 8080
int main(){
    int serverfd;
    struct sockaddr_in server_addr;
    int addrlen = sizeof(server_addr);
    char buffer[1024]= "Hello from client side";
    if((serverfd = socket(AF_INET,SOCK_STREAM, 0))<0){
        perror("Socket failed");
        exit(EXIT_FAILURE;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(connect(serverfd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    send(sock, buffer, 1024);
    printf("Message sent: %s\n", buffer);
    return 0;
}