int fib(int n) {
    if (n <= 0) {
        return 0;
    }
    int a = 1, b = 1;
    int result = 1;
    while (n > 2) {
        result = a + b;
        a = b;
        b = result;
        n = n - 1;
    }
    return result;
}

int main() {
    int n = 0;
    input(n);
    int num = fib(n);
    output(num);
    return 0;
}
