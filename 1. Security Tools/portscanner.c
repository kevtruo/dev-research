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

        // Configure the server address​
 server.sin_family = AF_INET;
        server.sin_port = htons(port);
        server.sin_addr.s_addr = inet_addr(target);

        // Send a dummy packet to the port
        int sent_bytes = sendto(sock, "", 0, 0, (struct sockaddr*)&server, sizeof(server));
        if (sent_bytes < 0) {
            perror("Send failed");
            close(sock);
            continue;
        }

        // Set a timeout for receiving
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

        // Attempt to receive data
        socklen_t server_len = sizeof(server);
        int received_bytes = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&server, &server_len);
        if (received_bytes >= 0) {
            printf("Port %d: Open\n", port);
        }

        // Close the socket
        close(sock);
    }
}

int main() {
    char protocol[4];
    char target[100];

    printf("TCP or UDP?\n");
    scanf("%3s", protocol); // Limit input to prevent overflow

    // Convert input to uppercase
    for (int i = 0; protocol[i]; i++) {
        protocol[i] = toupper(protocol[i]);
    }

    if (strcmp(protocol, "TCP") == 0) {
        printf("TCP selected\n");
        printf("Enter target IP:\n> ");
        scanf("%99s", target);
        tcp_scan(target);
    } else if (strcmp(protocol, "UDP") == 0) {
        printf("UDP selected\n");
        printf("Enter target IP:\n> ");
        scanf("%99s", target);
        udp_scan(target);
    } else {
        printf("Invalid selection. Please choose TCP or UDP.\n");
    }

    return 0;
}
