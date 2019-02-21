#include <string.h>

#define arraylen(arr) (sizeof(arr) == 0 ? 0 : sizeof(arr) / sizeof((arr)[0]))

int arr[3];

void insort(int *parr);

void insertion_sort(void);

void insertion_sort(void) {
    int *parr;
    int *parrend = arr + arraylen(arr);
    for (parr = arr + 1; parr < parrend; parr++) {
        if (*parr < *(parr - 1)) {
            // out of order
            insort(parr);
        }
    }
}

void insort(int *parr) {
    int val = *parr;
    while (parr != arr && *(parr - 1) > val) {
        *parr = *(parr - 1);
        parr--;
    }
    *parr = val;
}

int strpos(const char * str, const char ch) {
    char * strp;
    for (strp = (char *) str; *strp != '\0'; strp++) {
        if (*strp == ch) return (int) (strp - str);
    }
    return -1;
}

unsigned int strspn_(const char *str, const char *delims) {
    unsigned int i;
    size_t len = strlen(str);
    for (i = 0; i < len; i++) {
        if (strpos(delims, str[i]) == -1)
            return i;
    }
    return (unsigned int) len;
}

unsigned int strcspn_(const char * str, const char * delims) {
    unsigned int i;
    size_t len = strlen(str);
    for (i = 0; i < len; i++) {
        if (strpos(delims, str[i]) != -1)
            return i;
    }
    return (unsigned int) len;
}

void shift_element_by_gap(unsigned int i, unsigned int gap) {
    int val = arr[i];
    while (i >= gap && arr[i - gap] > val) {
        arr[i] = arr[i - gap];
        i -= gap;
    }
    arr[i] = val;
}

void shell_sort(void) {
    unsigned int gap, i, len = arraylen(arr);
    for (gap = len / 2; gap > 0; gap /= 2) {
        for (i = gap; i < len; i+=gap) {
            if (arr[i] < arr[gap]) {
                shift_element_by_gap(i, gap);
            }
        }
    }
}