// signal2.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

volatile sig_atomic_t got_signal = 0;

void handler(int sig) {
    printf("Hello World!\n");
    got_signal = 1;       // set flag for main to act
    alarm(5);             // re-arm the alarm
}

int main() {
    signal(SIGALRM, handler);
    alarm(5);

    while (1) {
        if (got_signal) {
            printf("Turing was right!\n");
            got_signal = 0;  // reset flag
        }
        pause();  // wait for signals (more efficient than busy wait)
    }

    return 0;
}
