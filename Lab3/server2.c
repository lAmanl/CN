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
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    if ((client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_length)) < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected by %s\n", inet_ntoa(client_address.sin_addr));

    // receive five subject marks from the client
    int marks[5];
    int i;
    for (i = 0; i < 5; i++) {
        char mark_str[4];
        recv(client_socket, mark_str, sizeof(mark_str), 0);
        marks[i] = atoi(mark_str);
    }
    int total = 0;
    for (i = 0; i < 5; i++) {
        total += marks[i];
    }
    char grade;
    int percentage = (total*100) / 500;
    if (percentage >= 90) {
    	grade = 'S';
        printf("Grade: %c\n", grade);
    } else if (percentage >= 80) {
        grade = 'A';
        printf("Grade: %c\n", grade);
    } else if (percentage >= 70) {
        grade = 'B';
        printf("Grade: %c\n", grade);
    } else if (percentage >= 60) {
        grade = 'C';
        printf("Grade: %c\n", grade);
    } else if (percentage >= 50) {
        grade = 'D';
        printf("Grade: %c\n", grade);
    } else {
        grade = 'E';
        printf("Grade: %c\n", grade);
    }
    send(client_socket, &grade, sizeof(grade), 0);
    close(client_socket);
    close(server_socket);

    return 0;
}

