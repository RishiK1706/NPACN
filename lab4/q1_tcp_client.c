#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

struct request
{
    int option;
    char data[50];
};

int main()
{
    int sock;
    struct sockaddr_in serv_addr;
    struct request req;
    char buffer[512] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    printf("\n1. Registration Number");
    printf("\n2. Name of Student");
    printf("\n3. Subject Code");
    printf("\n4. Exit");
    printf("\nEnter option: ");
    scanf("%d", &req.option);

    if (req.option == 4)
    {
        send(sock, &req, sizeof(req), 0);
        printf("Client exiting...\n");
        close(sock);
        exit(0);
    }

    printf("Enter value: ");
    scanf("%s", req.data);

    send(sock, &req, sizeof(req), 0);
    read(sock, buffer, sizeof(buffer));

    printf("\n--- Server Response ---\n%s\n", buffer);

    close(sock);
    return 0;
}
