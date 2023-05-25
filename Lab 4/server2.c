#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 8080
int main(){
int server_socket, a, b, res;
char op, message[1024], response[1024];
memset(message, 0, sizeof(message));
memset(response, 0, sizeof(response));
struct sockaddr_in server_address, client_address;
if((server_socket = socket(AF_INET, SOCK_DGRAM, 0))<0){
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
server_address.sin_family = AF_INET;
server_address.sin_port = htons(PORT);
server_address.sin_addr.s_addr = htonl(INADDR_ANY);
if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) <0 ){
perror("bind failed");
exit(EXIT_FAILURE);
}
printf("Server is listening to port %d\n", PORT);
socklen_t client_address_len = sizeof(client_address);
ssize_t rec = recvfrom(server_socket, message, sizeof(message), 0, (struct sockaddr*) &client_address, &client_address_len);
if(rec < 0){
perror("recvfrom failed");
exit(EXIT_FAILURE);
}
printf("Connected by %s\n", inet_ntoa(client_address.sin_addr));
sscanf(message, "%c %d %d", &op, &a, &b);

if(op == '+')
res = a+b;
else if(op == '-')
res = a-b;
else if(op == '*')
res = a*b;
else if(op == '/')
res = a/b;
else if(op == '%')
res = a%b;
else{
perror("Invalid operator");
exit(EXIT_FAILURE);
}
sprintf(response, "%d", res);
ssize_t sent = sendto(server_socket, response, strlen(response), 0, (struct sockaddr*) &client_address, client_address_len);
if(sent<0){
perror("sendto failed");
exit(EXIT_FAILURE);
}
close(server_socket);
return 0;
}
