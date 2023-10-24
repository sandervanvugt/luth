#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid == -1) {
        // If fork() returns -1, an error happened.
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        // Parent process
        printf("Parent process\n");
        printf("Child PID: %d\n", pid);
        printf("Sleeping for 10 seconds. Check the status of the child process now.\n");
        sleep(10);
        printf("Parent waking up!\n");
    } else {
        // Child process
        printf("Child process\n");
        exit(0); // Child exits immediately
    }

    return 0;
}

