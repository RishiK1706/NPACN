#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sockfd;
    char buffer[MAX];
    struct sockaddr_in servaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("\nEnter a string: ");
        fgets(buffer, MAX, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        sendto(sockfd, buffer, strlen(buffer) + 1, 0,
               (struct sockaddr *)&servaddr, sizeof(servaddr));

        if (strcmp(buffer, "Halt") == 0) {
            printf("Client terminated.\n");
            break;
        }

        recvfrom(sockfd, buffer, MAX, 0,
                 (struct sockaddr *)&servaddr, &len);

        printf("\n--- Server Response ---\n%s\n", buffer);
    }

    close(sockfd);
    return 0;
}
