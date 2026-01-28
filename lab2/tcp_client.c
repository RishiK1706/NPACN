#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int main() {
    int sock;
    struct sockaddr_in serv;
    char buffer[MAX], filename[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&serv, sizeof(serv));

    printf("Enter filename: ");
    scanf("%s", filename);
    send(sock, filename, sizeof(filename), 0);

    recv(sock, buffer, sizeof(buffer), 0);
    if (strcmp(buffer, "OK") != 0) {
        printf("File not present\n");
        return 0;
    }

    int choice;
    while (1) {
        printf("\n1.Search 2.Replace 3.Reorder 4.Exit\n");
        scanf("%d", &choice);
        send(sock, &choice, sizeof(choice), 0);

        if (choice == 1) {
            char str[50];
            printf("Enter string: ");
            scanf("%s", str);
            send(sock, str, sizeof(str), 0);

            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
        }

        else if (choice == 2) {
            char s1[50], s2[50];
            printf("Enter old string: ");
            scanf("%s", s1);
            printf("Enter new string: ");
            scanf("%s", s2);
            send(sock, s1, sizeof(s1), 0);
            send(sock, s2, sizeof(s2), 0);

            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
        }

        else if (choice == 3) {
            recv(sock, buffer, sizeof(buffer), 0);
            printf("%s\n", buffer);
        }

        else if (choice == 4) {
            break;
        }
    }
    close(sock);
    return 0;
}
