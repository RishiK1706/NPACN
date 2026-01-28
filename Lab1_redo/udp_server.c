#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

int isPalindrome(char str[]) {
    int i = 0, j = strlen(str) - 1;
    while (i < j) {
        if (str[i] != str[j])
            return 0;
        i++;
        j--;
    }
    return 1;
}

int main() {
    int sockfd;
    char buffer[MAX];
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    printf("UDP Server running...\n");

    while (1) {
        len = sizeof(cliaddr);
        recvfrom(sockfd, buffer, MAX, 0,
                 (struct sockaddr *)&cliaddr, &len);

        if (strcmp(buffer, "Halt") == 0) {
            printf("Termination signal received.\n");
            break;
        }

        int vowels[5] = {0}; 
        int length = strlen(buffer);

        for (int i = 0; buffer[i]; i++) {
            char c = buffer[i];
            if (c == 'a' || c == 'A') vowels[0]++;
            else if (c == 'e' || c == 'E') vowels[1]++;
            else if (c == 'i' || c == 'I') vowels[2]++;
            else if (c == 'o' || c == 'O') vowels[3]++;
            else if (c == 'u' || c == 'U') vowels[4]++;
        }

        char result[MAX];
        sprintf(result,
                "String Length: %d\nPalindrome: %s\nVowel Count:\n"
                "a=%d e=%d i=%d o=%d u=%d",
                length,
                isPalindrome(buffer) ? "Yes" : "No",
                vowels[0], vowels[1], vowels[2], vowels[3], vowels[4]);

        sendto(sockfd, result, strlen(result) + 1, 0,
               (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}
