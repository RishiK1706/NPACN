#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8050

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char domain[100], response[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Enter domain name: ");
    scanf("%s", domain);

    write(sockfd, domain, sizeof(domain));
    read(sockfd, response, sizeof(response));

    printf("IP Address: %s\n", response);

    close(sockfd);
    return 0;
}
