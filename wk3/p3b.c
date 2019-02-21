#include <stdlib.h>

double rand_double() {
    double ret = (double)rand();
    return ret/(RAND_MAX + 1);
}

int sample_geometric_rv(double p) {
    double q = rand_double();
    int n = 1;
    while (q >= p) {
        q = rand_double();
        n++;
    }
    return n;
}
