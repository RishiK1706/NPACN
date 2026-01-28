#include <stdio.h>
#include <stdlib.h>

int main() {

    // File pointer
    FILE* fptr;

    // Creating file using fopen()
    // with access mode "w"
    fptr = fopen("file.txt", "w");

    // checking if the file is created
    if (fptr == NULL)
        printf("The file is not opened.");
    else
        printf("The file is created Successfully.");
    return 0;
}
