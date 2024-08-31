#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>  // For inet_addr
#include <sys/socket.h> // For socket functions
#include <unistd.h>     // For close function

void tcp_scan(const char *target) {
    int sock;
    struct sockaddr_in server;
    
    printf("Scanning TCP ports on target %s...\n", target);

    for (int port = 1; port <= 1024; port++) {
        // Create a TCP socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
            continue;
        }

        // Configure the server address structure
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = inet_addr(target);

        // Attempt to connect to the port
        int result = connect(sock, (struct sockaddr*)&server, sizeof(server));
        if (result == 0) {
            printf("Port %d: Open\n", port);
        }

        // Close the socket
        close(sock);
    }
}

void udp_scan(const char *target) {
    int sock;
    struct sockaddr_in server;
    char buffer[1024];

    printf("Scanning UDP ports on target %s...\n", target);

    for (int port = 1; port <= 1024; port++) {
        // Create a UDP socket
        sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            perror("Socket creation failed");
            continue;
        }

        // Configure the server address​⬤
