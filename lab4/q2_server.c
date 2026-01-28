#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8050

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char domain[100], response[100];
    char db_domain[100], db_ip[100];
    int found;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);

    printf("DNS Server running...\n");

    while (1) {
        client_fd = accept(server_fd,
                            (struct sockaddr *)&client_addr,
                            &addr_len);

        read(client_fd, domain, sizeof(domain));
        printf("Query received: %s\n", domain);

        FILE *fp = fopen("database.txt", "r");
        found = 0;

        if (fp != NULL) {
            while (fscanf(fp, "%s %s", db_domain, db_ip) != EOF) {
                if (strcmp(domain, db_domain) == 0) {
                    strcpy(response, db_ip);
                    found = 1;
                    break;
                }
            }
            fclose(fp);
        }

        if (!found)
            strcpy(response, "Domain not found");

        write(client_fd, response, sizeof(response));
        close(client_fd);
    }
}
