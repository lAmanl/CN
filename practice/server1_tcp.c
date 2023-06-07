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
    char buffer[1024];
    if((serverfd = socket(AF_INET,SOCK_STREAM, 0))<0){
        perror("Socket failed");
        exit(EXIT_FAILURE;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(serverfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }
    if(listen(serverfd, 3) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    if((new_socket = accept(serverfd, (struct sockaddr*)&server_addr, (socklen_t*)&addrlen)) < 0){
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    valueread = read(new_socket, buffer, 1024);
    printf("Received MessageL %s\n", buffer);
    return 0;
}