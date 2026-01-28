#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void process_string(char *str, char *response) {
    int len = strlen(str);
    int i, is_palindrome = 1;
    int a=0, e=0, it=0, o=0, u=0;

    if(str[len-1] == '\n') {
        str[len-1] = '\0';
        len--;
    }

    for(i = 0; i < len; i++) {
        char c = TOLOWER(str[i]);
        if (c == 'a') a++;
        else if (c == 'e') e++;
        else if (c == 'i') it++;
        else if (c == 'o') o++;
        else if (c == 'u') u++;

        if(TOLOWER(str[i]) != TOLOWER(str[len - 1 - i])) {
            is_palindrome = 0;
        }
    }

    sprintf(response, "Length: %d\nPalindrome: %s\nVowels: A:%d, E:%d, I:%d, O:%d, U:%d", 
            len, is_palindrome ? "Yes" : "No", a, e, it, o, u);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("TCP Server waiting for connection...\n");
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        read(new_socket, buffer, BUFFER_SIZE);

        if (strcmp(buffer, "Halt") == 0) {
            printf("Termination signal received. Closing.\n");
            break;
        }

        char response[BUFFER_SIZE];
        process_string(buffer, response);
        send(new_socket, response, strlen(response), 0);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}