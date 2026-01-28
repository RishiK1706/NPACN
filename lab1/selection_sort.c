#include<stdio.h>

int main() {
    int n, i, j, temp, pos;
    int arr[20];

    printf("Enter number of inputs: ");
    scanf("%d", &n);

    printf("Enter inputs:\n" );
    for (i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    for(i=0;i<n-1;i++){
        pos=i;
        for (j=i+1;j<n;j++) {
            if (arr[j] < arr[pos]) {
                pos = j;
            }
        }

        temp = arr[pos];
        arr[pos] = arr[i];
        arr[i] = temp;
    }

    printf("Sorted array:\n");
    for (i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    return 0;
}
