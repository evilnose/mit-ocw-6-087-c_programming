int factorial(int n) {
    int i = 2;
    int ret = 1;
    while (i <= n) {
        ret *= i;
        i++;
    }
    return ret;
}
