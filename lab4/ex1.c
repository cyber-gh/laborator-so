#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>


int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror(errno);
        return errno;
    }
    if (pid == 0) {
        //this is the child
        char *binaryPath = "/bin/ls";
        char *args[] = {binaryPath, "-la", NULL};
        
        execve(binaryPath, args, NULL);
    } else {
        int status = 0;
        wait(&status);
        printf("returned status %d", status);
    }

    return 0;
}