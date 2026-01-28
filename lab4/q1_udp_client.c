#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 9090

struct request {
    int option;
    char data[50];
};

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    struct request req;
    char buffer[512];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    printf("1. Registration Number\n2. Name\n3. Subject Code\n");
    printf("Enter option: ");
    scanf("%d", &req.option);

    printf("Enter value: ");
    scanf("%s", req.data);

    sendto(sockfd, &req, sizeof(req), 0,
           (struct sockaddr *)&servaddr, sizeof(servaddr));

    len = sizeof(servaddr);
    recvfrom(sockfd, buffer, sizeof(buffer), 0,
             (struct sockaddr *)&servaddr, &len);

    printf("\n--- Server Response ---\n%s\n", buffer);

    return 0;
}
