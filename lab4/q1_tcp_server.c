#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

struct request
{
    int option;
    char data[50];
};

void child1(int sock)
{
    char response[200];
    sprintf(response,
            "PID: %d\nName: Rishi Khandelwal\nAddress: Gurugram, Harayana",
            getpid());
    send(sock, response, strlen(response), 0);
}

void child2(int sock)
{
    char response[200];
    sprintf(response,
            "PID: %d\nDept: CCE\nSemester: 6\nSection: C\nCourses: NPACN , EEFM ,FC-1-CC",
            getpid());
    send(sock, response, strlen(response), 0);
}

void child3(int sock)
{
    char response[200];
    sprintf(response,
            "PID: %d\nMarks Obtained: 87",
            getpid());
    send(sock, response, strlen(response), 0);
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    struct request req;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 5);

    printf("TCP Server running...\n");

    while (1)
    {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        recv(new_socket, &req, sizeof(req), 0);

        if (req.option == 4)
        {
            printf("Client requested termination.\n");
            close(new_socket);
            continue;
        }

        pid_t pid = fork();

        if (pid == 0)
        { // Child
            if (req.option == 1)
                child1(new_socket);
            else if (req.option == 2)
                child2(new_socket);
            else if (req.option == 3)
                child3(new_socket);

            close(new_socket);
            exit(0);
        }
        close(new_socket);
    }
}
