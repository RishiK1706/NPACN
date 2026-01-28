#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 100

void sort(int arr[], int n, int asc) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if ((asc && arr[i] > arr[j]) || (!asc && arr[i] < arr[j])) {
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server waiting for connection...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    printf("Client connected.\n");

    while (1) {
        int choice, n, arr[MAX], key;
        recv(new_socket, &choice, sizeof(choice), 0);

        if (choice == 5) {
            printf("Client exited.\n");
            break;
        }

        recv(new_socket, &n, sizeof(n), 0);
        recv(new_socket, arr, sizeof(int) * n, 0);

        char result[1024] = "";
        char buffer[64];

        if (choice == 1) {
            recv(new_socket, &key, sizeof(key), 0);
            int found = 0;
            for (int i = 0; i < n; i++) {
                if (arr[i] == key) {
                    found = 1;
                    break;
                }
            }
            sprintf(result, found ? "Element %d found." : "Element %d not found.", key);

        } else if (choice == 2 || choice == 3) { 
            sort(arr, n, choice == 2);
            strcat(result, "Sorted array: ");
            for (int i = 0; i < n; i++) {
                sprintf(buffer, "%d ", arr[i]);
                strcat(result, buffer);
            }

        } else if (choice == 4) { 
            strcat(result, "Even numbers: ");
            for (int i = 0; i < n; i++) {
                if (arr[i] % 2 == 0) {
                    sprintf(buffer, "%d ", arr[i]);
                    strcat(result, buffer);
                }
            }
            strcat(result, "\nOdd numbers: ");
            for (int i = 0; i < n; i++) {
                if (arr[i] % 2 != 0) {
                    sprintf(buffer, "%d ", arr[i]);
                    strcat(result, buffer);
                }
            }
        }

        send(new_socket, result, sizeof(result), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
