// timer.c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

volatile sig_atomic_t seconds_elapsed = 0;

void alarm_handler(int sig) {
    seconds_elapsed++;      // increment time
    alarm(1);               // re-arm alarm every second
}

void int_handler(int sig) {
    printf("\nProgram ran for %d seconds.\n", seconds_elapsed);
    exit(0);
}

int main() {
    signal(SIGALRM, alarm_handler);
    signal(SIGINT, int_handler);

    alarm(1);  // fire first alarm after 1 second

    while (1) {
        pause();  // wait for signal
    }

    return 0;
}