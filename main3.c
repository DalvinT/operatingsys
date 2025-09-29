#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_ITERATIONS 30
#define MAX_SLEEP 10

void child_process_logic() {
    // Seed random with current time and PID to avoid duplication
    srandom(time(NULL) ^ getpid());

    int iterations = random() % MAX_ITERATIONS + 1; // 1 to 30

    for (int i = 0; i < iterations; i++) {
        int sleep_time = random() % MAX_SLEEP + 1; // 1 to 10 seconds

        printf("Child Pid: %d is going to sleep!\n", getpid());
        sleep(sleep_time);
        printf("Child Pid: %d is awake!\nWhere is my Parent: %d?\n\n", getpid(), getppid());
    }

    exit(0);
}

int main() {
    pid_t pid1, pid2;
    int status;

    pid1 = fork();

    if (pid1 < 0) {
        perror("Fork failed for child 1");
        exit(1);
    } else if (pid1 == 0) {
        // This is child 1
        child_process_logic();
    }

    // Parent continues here
    pid2 = fork();

    if (pid2 < 0) {
        perror("Fork failed for child 2");
        exit(1);
    } else if (pid2 == 0) {
        // This is child 2
        child_process_logic();
    }

    // Parent waits for both children
    for (int i = 0; i < 2; i++) {
        pid_t done = wait(&status);
        printf("Child Pid: %d has completed\n", done);
    }

    return 0;
}

