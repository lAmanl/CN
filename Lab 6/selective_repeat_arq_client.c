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
    Frame frames[WINDOW_SIZE];
    int frame_sent[WINDOW_SIZE];
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
            // Sending frames in the window
            for (int i = 0; i < WINDOW_SIZE; i++) {
                if (frame_id + i < frame_id + WINDOW_SIZE) {
                    frames[i].sq_no = frame_id + i;
                    frames[i].frame_kind = 1;
                    frames[i].ack = 0;

                    printf("Enter Data: ");
                    scanf("%s", buffer);
                    strcpy(frames[i].packet.data, buffer);

                    sendto(sockfd, &frames[i], sizeof(Frame), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
                    printf("[+]Frame Sent: %d\n", frames[i].sq_no);
                    frame_sent[i] = 1;
                }
            }

            // Waiting for ACKs
            int addr_size = sizeof(serverAddr);
            for (int i = 0; i < WINDOW_SIZE; i++) {
                if (frame_sent[i] == 1) {
                    int f_recv_size = recvfrom(sockfd, &frames[i], sizeof(Frame), 0, (struct sockaddr*)&serverAddr, &addr_size);
                    if (f_recv_size > 0 && frames[i].sq_no == 0 && frames[i].ack == frame_id + i + 1) {
                        printf("[+]ACK Received: %d\n", frames[i].ack);
                        frame_sent[i] = 0;
                    } else {
                        printf("[-]ACK Not Received: %d\n", frames[i].ack);
                        ack_recv = 0;
                    }
                }
            }
        } else {
            printf("[-]Resending Frames\n");
            // Resending frames in the window
            for (int i = 0; i < WINDOW_SIZE; i++) {
                if (frame_sent[i] == 1) {
                    sendto(sockfd, &frames[i], sizeof(Frame), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
                    printf("[+]Frame Resent: %d\n", frames[i].sq_no);
                }
            }

            // Waiting for ACKs
            int addr_size = sizeof(serverAddr);
            for (int i = 0; i < WINDOW_SIZE; i++) {
                                if (frame_sent[i] == 1) {
                    int f_recv_size = recvfrom(sockfd, &frames[i], sizeof(Frame), 0, (struct sockaddr*)&serverAddr, &addr_size);
                    if (f_recv_size > 0 && frames[i].sq_no == 0 && frames[i].ack == frame_id + i + 1) {
                        printf("[+]ACK Received: %d\n", frames[i].ack);
                        frame_sent[i] = 0;
                    } else {
                        printf("[-]ACK Not Received: %d\n", frames[i].ack);
                        ack_recv = 0;
                    }
                }
            }
        }

        // Move the window
        if (ack_recv == 1) {
            frame_id += WINDOW_SIZE;
        }
    }
    close(sockfd);
}

