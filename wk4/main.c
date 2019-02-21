#include <stdio.h>
#include "class.h"

extern int arr[];

int main() {
    arr[0] = 3;
    arr[1] = 2;
    arr[2] = 1;
    insertion_sort();
    int i;
    for (i = 0; i < 3; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}