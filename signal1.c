// signal1.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t got_signal = 0;

void handler(int sig) {
    printf("Hello World!\n");
    got_signal = 1;  // set flag
}

int main() {
    signal(SIGALRM, handler);
    alarm(5);
    while (!got_signal);  // wait for signal
    printf("Turing was right!\n");
    return 0;
}
