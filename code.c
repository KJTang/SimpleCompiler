int g1 = 5-3/2*(1+3);
int g2 = g1*2-3;

// int add(int a, int b) {
//     return a+b;
// }
int func() {
    return 1234;
}

int main() {
    if (g1 > g2) {
        g1 = 10; g2 = 10;
    } else {
        g1 = 100; g2 = 100;
    }
    int a = func()+g1+g2;
    return 0;
}
