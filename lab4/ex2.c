#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>



int main() {

    pid_t child_pid;
    child_pid = fork();
    if (child_pid < 0) {
        perror(errno);
        return errno;
    } 
    if (child_pid == 0) {
        int n = 24;
        while (n != 1) {
            printf("%d ", n);
            if (n % 2 == 0) {
                n /= 2;
            } else {
                n = n * 3 + 1;
            }
        }

    } else {
        int res = 0;
        wait(&res);
    }

    return 0;
}