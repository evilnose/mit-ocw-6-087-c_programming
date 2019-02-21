#include <stdio.h>

int main() {
    enum sz{S=0, L=-3, XL};
    enum sz t = XL;
    printf("%d\n", t);
    return 0;
}

