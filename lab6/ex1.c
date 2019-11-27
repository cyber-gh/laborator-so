#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

void *runner(void *params) {
    char *str = (char *) params;

    for (int i = strlen(str) - 1; i >= 0; i--) {
        printf("%c", str[i]);
    }

    return NULL;
}

int main() {
    pthread_t thread_id;

    if (pthread_create(&thread_id, NULL, runner, "12345678")) {
        perror(NULL);
        return errno;
    }

    if (pthread_join(thread_id, NULL)) {
        perror(NULL);
        return errno;
    }

    



    return 0;
}