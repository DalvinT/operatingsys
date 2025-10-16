#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_ITERATIONS 25

int main() {
    int shm_id;
    int *shmPTR;
    int i; // Declare loop counter variable here

    srand(time(NULL));  // seed randomness

    // Create shared memory for 2 integers: BankAccount and Turn
    shm_id = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget error");
        exit(1);
    }

    shmPTR = (int *)shmat(shm_id, NULL, 0);
    if (shmPTR == (void *)-1) {
        perror("shmat error");
        exit(1);
    }

    int *BankAccount = &shmPTR[0];
    int *Turn = &shmPTR[1];

    *BankAccount = 0;
    *Turn = 0;  // 0 = Parent’s turn, 1 = Child’s turn

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork error");
        exit(1);
    }

    // ------------------------------------------------------------
    // CHILD PROCESS (Poor Student)
    // ------------------------------------------------------------
    else if (pid == 0) {
        for (i = 0; i < NUM_ITERATIONS; i++) {
            sleep(rand() % 6);  // Sleep 0–5 seconds

            while (*Turn != 1) {
                // Wait for turn
            }

            int account = *BankAccount;
            int need = rand() % 51;  // 0–50
            printf("Poor Student needs $%d\n", need);

            if (need <= account) {
                account -= need;
                printf("Poor Student: Withdraws $%d / Balance = $%d\n", need, account);
            } else {
                printf("Poor Student: Not Enough Cash ($%d)\n", account);
            }

            *BankAccount = account;
            *Turn = 0;  // Switch turn back to Parent
        }

        exit(0);
    }

    // ------------------------------------------------------------
    // PARENT PROCESS (Dear Old Dad)
    // ------------------------------------------------------------
    else {
        for (i = 0; i < NUM_ITERATIONS; i++) {
            sleep(rand() % 6);  // Sleep 0–5 seconds

            while (*Turn != 0) {
                // Wait for turn
            }

            int account = *BankAccount;

            if (account <= 100) {
                int deposit = rand() % 101;  // 0–100
                if (deposit % 2 == 0) {
                    account += deposit;
                    printf("Dear Old Dad: Deposits $%d / Balance = $%d\n", deposit, account);
                } else {
                    printf("Dear Old Dad: Doesn't have any money to give\n");
                }
            } else {
                printf("Dear Old Dad: Thinks Student has enough Cash ($%d)\n", account);
            }

            *BankAccount = account;
            *Turn = 1;  // Switch turn to Child
        }

        wait(NULL);  // Wait for child to finish

        // Detach and remove shared memory
        shmdt((void *)shmPTR);
        shmctl(shm_id, IPC_RMID, NULL);

        printf("\nSimulation finished. Shared memory removed.\n");
    }

    return 0;
}
