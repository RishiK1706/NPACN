#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

void reorder_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char data[5000];
    int n = fread(data, 1, sizeof(data), fp);
    fclose(fp);

    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (data[i] > data[j]) {
                char t = data[i];
                data[i] = data[j];
                data[j] = t;
            }

    fp = fopen(filename, "w");
    fwrite(data, 1, n, fp);
    fclose(fp);
}

int main() {
    int server_fd, new_sock;
    struct sockaddr_in addr;
    char buffer[MAX], filename[100];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    new_sock = accept(server_fd, NULL, NULL);

    recv(new_sock, filename, sizeof(filename), 0);
    FILE *fp = fopen(filename, "r");

    if (!fp) {
        send(new_sock, "File not present", 16, 0);
        close(new_sock);
        return 0;
    }
    send(new_sock, "OK", 2, 0);

    int choice;
    while (1) {
        recv(new_sock, &choice, sizeof(choice), 0);

        if (choice == 1) {
            char str[100], filedata[2000];
            int count = 0;
            recv(new_sock, str, sizeof(str), 0);

            fseek(fp, 0, SEEK_SET);
            fread(filedata, 1, sizeof(filedata), fp);

            char *ptr = filedata;
            while ((ptr = strstr(ptr, str))) {
                count++;
                ptr += strlen(str);
            }

            if (count)
                send(new_sock, &count, sizeof(count), 0);
            else
                send(new_sock, "String not found", 16, 0);
        }

        else if (choice == 2) {
            char s1[50], s2[50], data[2000], result[3000] = "";
            recv(new_sock, s1, sizeof(s1), 0);
            recv(new_sock, s2, sizeof(s2), 0);

            fseek(fp, 0, SEEK_SET);
            fread(data, 1, sizeof(data), fp);

            char *pos = data;
            int found = 0;
            while ((pos = strstr(pos, s1))) {
                found = 1;
                strncat(result, data, pos - data);
                strcat(result, s2);
                pos += strlen(s1);
                strcpy(data, pos);
                pos = data;
            }
            strcat(result, data);

            if (found) {
                fp = fopen(filename, "w");
                fputs(result, fp);
                fclose(fp);
                send(new_sock, "String replaced", 15, 0);
            } else {
                send(new_sock, "String not found", 16, 0);
            }
        }

        else if (choice == 3) {
            reorder_file(filename);
            send(new_sock, "File reordered", 14, 0);
        }

        else if (choice == 4) {
            break;
        }
    }
    close(new_sock);
    close(server_fd);
    return 0;
}
