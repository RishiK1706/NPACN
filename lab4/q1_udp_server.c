#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define PORT 9090

struct request {
    int option;
    char data[50];
};

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    struct request req;
    char response[256];
    socklen_t len = sizeof(cliaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("UDP Server running...\n");

    while (1) {
        recvfrom(sockfd, &req, sizeof(req), 0,
                 (struct sockaddr *)&cliaddr, &len);

        pid_t pid = fork();

        if (pid == 0) {
            if (req.option == 1)
                sprintf(response,
                        "PID: %d\nName: Rahul Kumar\nAddress: Chennai",
                        getpid());
            else if (req.option == 2)
                sprintf(response,
                        "PID: %d\nDept: CSE\nSemester: 5\nSection: A",
                        getpid());
            else if (req.option == 3)
                sprintf(response,
                        "PID: %d\nMarks: 87",
                        getpid());

            sendto(sockfd, response, strlen(response), 0,
                   (struct sockaddr *)&cliaddr, len);
            exit(0);
        }
    }
}
