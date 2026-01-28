#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sockfd, newfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    char buffer[BUF_SIZE];
    pid_t pid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(sockfd, 5);
    printf("Server listening on port %d\n", PORT);

    len = sizeof(cliaddr);
    newfd = accept(sockfd, (struct sockaddr *)&cliaddr, &len);
    if (newfd < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Server connected\n");

    pid = fork();

    if (pid > 0) {
        printf("Server Parent: PID=%d, PPID=%d\n", getpid(), getppid());
        while (1) {
            fgets(buffer, BUF_SIZE, stdin);
            write(newfd, buffer, strlen(buffer));
        }
    } else if (pid == 0) {
        printf("Server Child: PID=%d, PPID=%d\n", getpid(), getppid());
        while (1) {
            int n = read(newfd, buffer, BUF_SIZE);
            if (n <= 0)
                break;
            buffer[n] = '\0';
            printf("Client: %s", buffer);
        }
    }

    close(newfd);
    close(sockfd);
    return 0;
}
