#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[BUF_SIZE];
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("172.18.175.71");

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Client connected to server\n");

    pid = fork();

    if (pid > 0) {
        printf("Client Parent: PID=%d, PPID=%d\n", getpid(), getppid());
        while (1) {
            fgets(buffer, BUF_SIZE, stdin);
            write(sockfd, buffer, strlen(buffer));
        }
    } else if (pid == 0) {
        printf("Client Child: PID=%d, PPID=%d\n", getpid(), getppid());
        while (1) {
            int n = read(sockfd, buffer, BUF_SIZE);
            if (n <= 0)
                break;
            buffer[n] = '\0';
            printf("Server: %s", buffer);
        }
    }

    close(sockfd);
    return 0;
}
