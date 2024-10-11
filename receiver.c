#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAX_PACKET_SIZE 65507 // Maximum size for a UDP packet

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <output file>\n", argv[0]);
        exit(1);
    }

    char *output_file = argv[1];
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[MAX_PACKET_SIZE];
    struct timeval t1, t2;
    FILE *file = fopen(output_file, "w");

    if (!file) {
        perror("File opening failed");
        exit(1);
    }

    // Create socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(12345); // Port number matching the sender

    // Bind the socket to the address and port
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    int counter = 0;
    while (1) {
        len = sizeof(cliaddr);
        ssize_t packet_size;

        // Receive the first packet and determine its size
        packet_size = recvfrom(sockfd, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&cliaddr, &len) * 8; //converting bytes back to bits
        printf("pkt size : %zd \t",packet_size);
        if (packet_size < 0) {
            perror("Receive failed");
            continue;   // using continue so that when one of the packets is not received nothing will be printed in the file
        } 
        gettimeofday(&t1, NULL);

        // Receive the second packet and determine its size
        packet_size = recvfrom(sockfd, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&cliaddr, &len) * 8; //converting bytes back to bits
        printf("pkt size : %zd \n",packet_size);
        if (packet_size < 0) {
            perror("Receive failed");
            continue;
        }
        gettimeofday(&t2, NULL);
        counter++;

        // Calculate the time difference in microseconds
        long time_diff = (t2.tv_sec - t1.tv_sec) * 1000000L + (t2.tv_usec - t1.tv_usec);
        double bottleneck_speed = (double)(packet_size) / (double)time_diff; 

        // Write the result to the file
        fprintf(file, "%lf\n", bottleneck_speed);
        fflush(file);
    }

    fclose(file);
    close(sockfd);
    printf("Total packet pairs received : %d",counter);
    return 0;
}
