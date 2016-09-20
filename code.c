
n = null;
a = 0;
b = 1.2;
c = "test";

d = [100];
d[0] = a;
d[1] = b;
d[2] = c;
d[3] = () {
    //
};
d[4] = [5];
d[4][0] = true;

class e {
    a = 0;
    b = 1;
    c = () {
        print("1");
    }
}

class f : e {
    c = () {
        print("2");
    }
}

main = () {
    f1 = new f();
    f1.c();
}

main();
