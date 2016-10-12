var fabb = function(a) {
    if (a <= 1) {
        return 1;
    }
    return fabb(a - 1);
}

fabb(5);