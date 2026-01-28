#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 100

void swap(char *x, char *y) {
    char temp = *x;
    *x = *y;
    *y = temp;
}

int shouldSwap(char str[], int start, int curr) {
    for (int i = start; i < curr; i++) {
        if (str[i] == str[curr])
            return 0;
    }
    return 1;
}

void permute(char str[], int l, int r) {
    if (l == r) {
        printf("%s\n", str);
        return;
    }

    for (int i = l; i <= r; i++) {
        if (shouldSwap(str, l, i)) {
            char temp = str[l];
            str[l] = str[i];
            str[i] = temp;

            permute(str, l + 1, r);

            temp =str[l];
            str[l] = str[i];
            str[i] = temp;
        }
    }
}


int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAX];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
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

    printf("UDP Server waiting for client...\n");

    len = sizeof(cliaddr);
    recvfrom(sockfd, buffer, MAX, 0,
             (struct sockaddr *)&cliaddr, &len);

    printf("\nReceived string: %s\n", buffer);
    printf("Permutations:\n");

    permute(buffer, 0, strlen(buffer) - 1);

    sendto(sockfd, "Permutations printed on server",
           strlen("Permutations printed on server"), 0,
           (struct sockaddr *)&cliaddr, len);

    close(sockfd);
    return 0;
}
