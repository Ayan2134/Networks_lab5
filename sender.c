#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>

#define MAX_PACKET_SIZE 65507 // Maximum size for a UDP packet

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <packet size> <destination IP> <spacing (ms)> <number of packet pairs>\n", argv[0]);
        exit(1);
    }

    int packet_size = atoi(argv[1])/8; //converting bits to bytes
    char *dest_ip = argv[2];
    int spacing_ms = atoi(argv[3]);
    int num_pairs = atoi(argv[4]);

    if (packet_size <= 0 || packet_size > MAX_PACKET_SIZE) {
        fprintf(stderr, "Packet size must be between 1 and %d bits.\n", MAX_PACKET_SIZE);
        exit(1);
    }

    int sockfd;
    struct sockaddr_in dest_addr;

    // Create the socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(12345); // Arbitrary port number

    // Convert IP address
    if (inet_pton(AF_INET, dest_ip, &dest_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(1);
    }

    char packet[packet_size];
    struct timeval tv;

    for (int i = 0; i < num_pairs; i++) {
        // Fill the packet with the packet number and any other required information
        memset(packet, 0, packet_size);
        snprintf(packet, packet_size, "Pkt %d", i * 2 + 1);

        // Send the first packet
        if (sendto(sockfd, packet, packet_size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
            perror("Send failed");
            exit(1);
        }
        // Immediately prepare and send the second packet
        snprintf(packet, packet_size, "Pkt %d", i * 2 + 2);
        if (sendto(sockfd, packet, packet_size, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
            perror("Send failed");
            exit(1);
        }
        // Wait for the specified spacing before sending the next pair
        usleep(spacing_ms * 1000);
    }

    close(sockfd);
    return 0;
}
