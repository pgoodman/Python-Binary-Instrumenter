
#include <cstdio>
#include "gc/pthread.h"

int fib(int i) {
    if(0 == i) {
        return 1;
    } else if(1 == i) {
        return 1;
    } else {
        return fib(i - 1) + fib(i - 2);
    }
}

int program_main(int argc, char **argv) throw() {
    (void) argc;
    (void) argv;

    printf("fib = %d\n", fib(argc));
    return 0;
}
