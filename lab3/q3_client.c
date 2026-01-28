#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAX];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("Input string: ");
    fgets(buffer, MAX, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    write(sockfd, buffer, strlen(buffer));

    printf("\nOutput from server:\n");

    while (read(sockfd, buffer, MAX) > 0) {
        printf("%s", buffer);
        memset(buffer, 0, MAX);
    }

    close(sockfd);
    return 0;
}
