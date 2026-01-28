#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 100

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAX];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter a string: ");
    fgets(buffer, MAX, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    sendto(sockfd, buffer, strlen(buffer), 0,
           (struct sockaddr *)&servaddr, sizeof(servaddr));

    len = sizeof(servaddr);
    recvfrom(sockfd, buffer, MAX, 0,
             (struct sockaddr *)&servaddr, &len);

    printf("Server response: %s\n", buffer);

    close(sockfd);
    return 0;
}
