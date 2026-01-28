#include <stdio.h>
#include <string.h>

int main() {
    char msg[10];
    int i, len, palin = 1;

    printf("Enter a string:\n");
    gets(msg);

    len = strlen(msg);

    for (i = 0; i < len / 2; i++) {
        if (msg[i] != msg[len - 1 - i]) {
            palin = 0;
            break;
        }
    }

    if (palin == 1)
        printf("String is palindrome");
    else
        printf("String is not palindrome");

    return 0;
}
