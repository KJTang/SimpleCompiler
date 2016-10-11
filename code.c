var func = function(a) {
    // var a = 1;
    a = 0;
    return true;
};

var num = 0+1+2+3;
var str = "hello" + " world";

var func2 = function(a) {
    func = function(b) {
        return a + b;
    }
    return func;
}

// func(function(){}, array[1000])

// arr = array[100];
// arr[0] = 1;

// t = table{};
// t.arr = arr;
// t.func = func;

// 1 + 2 * 3.0 / 4 - 5;
// b = "hello" + " world" + (12-3) + null;
// c = true || false || 1 > 2;

// factorial = function(num) {
//     if (num == 1) {
//         return 1;
//     }
//     return factorial(num - 1);
// }

// main = function() {
//     num = 0;
//     a = 1 + 2 * 3 / 4 - 5;
//     Input(num);
//     Output(factorial(num));
// }

// main();

// if (a + 1) {
//     //
//     b = [100];
// } else {
//     while (true) {
//         c + 1;
//     }
// }

// func = function () {
//     arr = [(100 / a) - b + c(d)];
// }


// var = arr[0] + arr[1].func(1, b[1]*2/c.d, a = 1);

// n.a[0] + 5 / (1 - n.a[100-99]) * n.a[2].b;

// n = null;
// a = 0;
// b = 1.2;
// c = "test";

// d = [100];
// d[0] = a;
// d[1] = b;
// d[2] = c;
// d[3] = function () {
//     //
// };
// d[4] = [5];
// d[4][0] = true;

// class e {
//     a = 0;
//     b = 1;
//     c = function () {
//         print("1");
//     }
// }

// class f : e {
//     c = function () {
//         print("2");
//     }
// }

// main = function () {
//     f1 = new f();
//     f1.c();
// }

// main();