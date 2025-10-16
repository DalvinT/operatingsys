#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

/*
 * Executes the command: cat scores | grep <argument> | sort
 * Usage: ./pipes_processes3 <search_term>
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <search_term>\n", argv[0]);
        exit(1);
    }

    int pipe1[2]; // between cat and grep
    int pipe2[2]; // between grep and sort

    pipe(pipe1);
    pipe(pipe2);

    pid_t pid1 = fork();

    if (pid1 == 0) {
        // === CHILD PROCESS ===
        // This will fork another process to create the grandchild

        pid_t pid2 = fork();

        if (pid2 == 0) {
            // === GRANDCHILD PROCESS (P3): sort ===

            // Input comes from pipe2
            dup2(pipe2[0], STDIN_FILENO);
            close(pipe2[0]);
            close(pipe2[1]);
            close(pipe1[0]);
            close(pipe1[1]);

            char *sort_args[] = {"sort", NULL};
            execvp("sort", sort_args);

            perror("execvp sort failed");
            exit(1);
        }
        else {
            // === CHILD PROCESS (P2): grep <arg> ===

            // Input comes from pipe1, output goes to pipe2
            dup2(pipe1[0], STDIN_FILENO);  // from cat
            dup2(pipe2[1], STDOUT_FILENO); // to sort

            // Close all pipe ends
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[0]);
            close(pipe2[1]);

            char *grep_args[] = {"grep", argv[1], NULL};
            execvp("grep", grep_args);

            perror("execvp grep failed");
            exit(1);
        }
    } else {
        // === PARENT PROCESS (P1): cat scores ===

        // Output goes to pipe1
        dup2(pipe1[1], STDOUT_FILENO);

        // Close all pipe ends
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe2[1]);

        char *cat_args[] = {"cat", "scores", NULL};
        execvp("cat", cat_args);

        perror("execvp cat failed");
        exit(1);
    }

    return 0;
}
