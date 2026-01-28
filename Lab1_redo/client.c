#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 100

int main() {
    int sockfd = 0;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        int choice, n, arr[MAX], key;
        char buffer[1024];

        printf("\nMenu:\n");
        printf("1. Search\n");
        printf("2. Sort Ascending\n");
        printf("3. Sort Descending\n");
        printf("4. Split Odd & Even\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        send(sockfd, &choice, sizeof(choice), 0);

        if (choice == 5) {
            printf("Exiting client.\n");
            break;
        }

        printf("Enter number of elements: ");
        scanf("%d", &n);
        printf("Enter elements:\n");
        for (int i = 0; i < n; i++)
            scanf("%d", &arr[i]);

        send(sockfd, &n, sizeof(n), 0);
        send(sockfd, arr, sizeof(int) * n, 0);

        if (choice == 1) {
            printf("Enter element to search: ");
            scanf("%d", &key);
            send(sockfd, &key, sizeof(key), 0);
        }

        recv(sockfd, buffer, sizeof(buffer), 0);
        printf("\nServer Response:\n%s\n", buffer);
    }

    close(sockfd);
    return 0;
}
