#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>

#define PORT 8080

typedef struct packet {
    char data[1024];
} Packet;

typedef struct frame {
    int frame_kind, sq_no, ack;
    Packet packet;
} Frame;

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    socklen_t addr_size;

    int frame_id = 0;
    Frame frame_send;
    Frame frame_recv;
    int ack_recv = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    struct timeval timeout;
    timeout.tv_sec = 1; // Set the timeout to 1 second
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    while (1) {
        if (ack_recv == 1) {
            frame_send.sq_no = frame_id;
            frame_send.frame_kind = 1;
            frame_send.ack = 0;

            printf("Enter Data: ");
            scanf("%s", buffer);
            strcpy(frame_send.packet.data, buffer);

            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            printf("[+]Frame Sent\n");

            // Wait for ACK
            int addr_size = sizeof(serverAddr);
            int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, &addr_size);
            if (f_recv_size > 0 && frame_recv.sq_no == 0 && frame_recv.ack == frame_id + 1) {
                printf("[+]ACK Received\n");
                ack_recv = 1;
            } else {
                printf("[-]ACK Not Received\n");
                ack_recv = 0;
            }
        } else {
            printf("[-]Resending Frame\n");
            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

            // Wait for ACK
            int addr_size = sizeof(serverAddr);
            int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, &addr_size);
            if (f_recv_size > 0 && frame_recv.sq_no == 0 && frame_recv.ack == frame_id + 1) {
                printf("[+]ACK Received\n");
                ack_recv = 1;
            } else {
                printf("[-]ACK Not Received\n");
                ack_recv = 0;
            }
        }
        frame_id++;
    }
    close(sockfd);
}
