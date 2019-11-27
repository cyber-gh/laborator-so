#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_PROC 100

int arr[] = {4,5, 7, 9 ,10, 20, 30, 40, 50};
int sz = sizeof(arr) / sizeof(arr[0]);

int child_pids[MAX_PROC];



int main() {

    for (int i = 0; i < sz; i++) {
        pid_t child_pid = fork();
        if (child_pid < 0) {
            perror(errno);
            return errno;
        }

        if (child_pid == 0) {
            int nr = arr[i];
            while (nr != 1) {
                printf("%d ", nr );
                if (nr % 2 == 0) nr /= 2;
                else nr = nr * 3 + 1;
            }
            printf("\n");
            return 0;
        }  else {
            child_pids[i] = child_pid;
        }
    }

    for (int i = 0; i < sz; i++) {
        int status = 0;
        wait(NULL);
        //waitpid(child_pids[i], &status, 0);
    }
    return 0;
}