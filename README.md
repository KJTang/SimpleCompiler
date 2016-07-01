### Simple Compiler ###

#### Intro ####
this is a simple compiler for a c-like language written 
in c++.  
this compiler has no error check system, and make no optimizion
at all, it's just a intro to compiler learning.

#### Feature ####

* Comment:  
    c++ style comment:
``` c++
    // comment
```

* Variable Type:  
    only support integers(int)
``` c++
    int a;
    int b = 0;
    int c = d = 1;
    a = d;
```

* Flow Control:
``` c++
    // if 
    if (cond1) {
        // do something
    } else if (cond2) {
        // do something
    } else {
        // do something
    }

    // while 
    while (cond) {
        // do something
    }
```

* Function:
``` c++
    // define
    int add(int a, int b) {
        return a+b;
    }
    // call
    int c = add(1, 5);
```

* System Functions:
``` c++
    // prototype: input(int)
    // usage: input a integer , and save it to variable 'a'
    input(a);

    // prototype: output(int)
    // usage: output a integer 'a'
    output(a);
```

#### Example ####
download the codes, build and run:
``` shell
$make
$./a.out code.c
```
"a.out" is our simple compiler;  
"code.c" is the file our simple compiler will compile and run:
``` c++
// code.c
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

```
input 1, get output: 1  
input 5, get output: 5  
input 9, get output: 34  


#### License ####
MIT