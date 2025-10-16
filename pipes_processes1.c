#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>

int main() {
    int fd1[2]; // pipe: P1 to P2
    int fd2[2]; // pipe: P2 to P1

    char fixed_str1[] = "howard.edu";
    char fixed_str2[] = "gobison.org";
    char input_str[100];
    char final_str[300];

    pid_t p;

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        fprintf(stderr, "Pipe Failed\n");
        return 1;
    }

    printf("Enter a string to concatenate: ");
    scanf("%s", input_str);

    p = fork();

    if (p < 0) {
        fprintf(stderr, "fork Failed\n");
        return 1;
    }

    // Parent process: P1
    else if (p > 0) {
        close(fd1[0]); // Close read end of pipe1
        close(fd2[1]); // Close write end of pipe2

        // Send input to child (P2)
        write(fd1[1], input_str, strlen(input_str) + 1);
        close(fd1[1]); // Done writing

        // Read final string from P2
        char temp_str[300];
        read(fd2[0], temp_str, sizeof(temp_str));

        // Append "gobison.org"
        strcpy(final_str, temp_str);
        strcat(final_str, fixed_str2);

        printf("Final concatenated string: %s\n", final_str);

        close(fd2[0]); // Done reading
        wait(NULL);    // Wait for child to finish
    }

    // Child process: P2
    else {
        close(fd1[1]); // Close write end of pipe1
        close(fd2[0]); // Close read end of pipe2

        char concat_str[200];
        read(fd1[0], concat_str, sizeof(concat_str));
        close(fd1[0]); // Done reading

        // Append "howard.edu"
        strcat(concat_str, fixed_str1);
        printf("Concatenated string: %s\n", concat_str);

        // Get second user input
        char second_input[100];
        printf("Enter another string to append: ");
        scanf("%s", second_input);

        strcat(concat_str, second_input);

        // Send updated string back to parent
        write(fd2[1], concat_str, strlen(concat_str) + 1);
        close(fd2[1]); // Done writing

        exit(0);
    }

    return 0;
}
