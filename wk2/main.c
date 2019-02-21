#include <stdio.h>

int swap(int val) {
    int a = val >> 8;
    int b = ((int) (unsigned char) val) << 8; // better to replace with 0xFF |
    return a + b;
}

int rotate(int val) {
    int head = val << 12;
    return (unsigned short int) (head + (val >> 4));
}

int main() {
//    int val = 0xCAFE;
//    printf("%d", rotate(val) == 0xECAF);

    int x = 0xFF33;
    int MASK = 0xFF00;
    int c = x & MASK == 0;
    printf("%d", c == 0);
}

