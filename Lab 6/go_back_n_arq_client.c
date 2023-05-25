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
#define WINDOW_SIZE 4

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
    int base = 0;
    int next_seq_num = 0;

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
        if (next_seq_num < base + WINDOW_SIZE) {
            frame_send.sq_no = next_seq_num;
            frame_send.frame_kind = 1;
            frame_send.ack = 0;

            printf("Enter Data: ");
            scanf("%s", buffer);
            strcpy(frame_send.packet.data, buffer);

            sendto(sockfd, &frame_send, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
            printf("[+]Frame Sent: %d\n", frame_send.sq_no);

            if (base == next_seq_num) {
                // Start the timer for the oldest unacknowledged frame
                timeout.tv_sec = 1;
                timeout.tv_usec = 0;
                setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            }

            next_seq_num++;
        }

        int addr_size = sizeof(serverAddr);
        int f_recv_size = recvfrom(sockfd, &frame_recv, sizeof(Frame), 0, (struct sockaddr*)&serverAddr, &addr_size);
        if (f_recv_size > 0 && frame_recv.ack >= base) {
            printf("[+]ACK Received: %d\n", frame_recv.ack);

            base = frame_recv.ack + 1;

            if (base == next_seq_num) {
                // Stop the timer as all frames in the window have been acknowledged
                timeout.tv_sec = 0;
                timeout.tv_usec = 0;
                setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            } else {
                // Restart the timer for the next unacknowledged frame
                timeout.tv_sec = 1;
                timeout.tv_usec = 0;
                setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
            }
        } else {
            printf("[-]ACK Not Received or Invalid ACK\n");
        }
    }
    close(sockfd);
}

