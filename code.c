int g = 5-3/2*(1+3);

int add(int a, int b) {
    return a+b;
}

int main() {
    int a = 0, b = 1;
    int c = 0;
    if (a + b > 1) {
        c = add(a, b);
    } else {
        c = a;
    }
    return 0;
}
