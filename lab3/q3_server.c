#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX 1024

void sort_asc(char *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] > arr[j]) {
                char t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
}

void sort_desc(char *arr, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (arr[i] < arr[j]) {
                char t = arr[i];
                arr[i] = arr[j];
                arr[j] = t;
            }
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in servaddr;
    char buffer[MAX];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(server_fd, 5);

    printf("Server waiting for connection...\n");

    client_fd = accept(server_fd, NULL, NULL);

    read(client_fd, buffer, MAX);
    printf("Received string: %s\n", buffer);

    pid_t pid = fork();

    if (pid == 0) {
        /* CHILD PROCESS → digits ascending */
        char digits[MAX];
        int k = 0;

        for (int i = 0; buffer[i]; i++)
            if (buffer[i] >= '0' && buffer[i] <= '9')
                digits[k++] = buffer[i];
        digits[k] = '\0';

        sort_asc(digits, k);

        char msg[MAX];
        sprintf(msg,
                "Child PID %d: Sorted digits (ascending): %s\n",
                getpid(), digits);

        write(client_fd, msg, strlen(msg));
        exit(0);
    } else {
        /* PARENT PROCESS → characters descending */
        char chars[MAX];
        int k = 0;

        for (int i = 0; buffer[i]; i++)
            if ((buffer[i] >= 'a' && buffer[i] <= 'z') ||
                (buffer[i] >= 'A' && buffer[i] <= 'Z'))
                chars[k++] = buffer[i];
        chars[k] = '\0';

        sort_desc(chars, k);

        char msg[MAX];
        sprintf(msg,
                "Parent PID %d: Sorted characters (descending): %s\n",
                getpid(), chars);

        write(client_fd, msg, strlen(msg));
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
