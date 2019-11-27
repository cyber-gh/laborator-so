#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define MAX_RES (5)



pthread_mutex_t lock;
int available_res = MAX_RES;

pthread_t pids[MAX_RES];
int cnt = 0;

int decrease_count(int nr) {
    pthread_mutex_lock(&lock);
    int retVal = 0;
    if (available_res < nr ) retVal = -1;
    else {
        available_res -= nr;
    }

    pthread_mutex_unlock(&lock);
    return retVal;
}

int increase_count(int nr) {
    pthread_mutex_lock(&lock);
    available_res += nr;
    pthread_mutex_unlock(&lock);

    return 0;
}

void *runner( void *params) {
    int nr = 2;
    while (decrease_count(nr) == -1);
    printf("decreased by %d -- current %d\n", nr, available_res);
    //sleep(1);
    increase_count(nr);

    printf("increased by %d -- current %d\n", nr, available_res);
}

int main() {
    if (pthread_mutex_init(&lock, NULL)) {
        perror(NULL);
        return errno;
    }

    for (int i = 0; i < MAX_RES; i++) {
        if (pthread_create(&pids[i], NULL, runner, NULL) ) {
            perror(NULL);
            return errno;
        } 
    }

    for (int i = 0; i < MAX_RES; i++) {
        if (pthread_join(pids[i], NULL)) {
            perror(NULL);
            return errno;
        }
    }

    return 0;
}