#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

sem_t sem;
pthread_mutex_t lock;

#define MAX_RES (10)

pthread_t pids[MAX_RES];
int cnt = 0;

void barrier() {
    pthread_mutex_lock(&lock);
    cnt++;
    pthread_mutex_unlock(&lock);

    if (cnt < MAX_RES) {
        sem_wait(&sem);
        sem_post(&sem);
    } else {
        sem_post(&sem);
    }
}

void *runner(void *params) {
    int id = *((int *) params );

    printf("%d reached barrier\n", id);
    barrier();

    printf("%d passed barrier\n", id);

    return NULL;

}

int main() {

    if (set_init(&sem, 0, 0)) {
        perror("pzdms 1");
        return errno;
    }
    
    if (pthread_mutex_init(&lock, NULL)) {
        perror("pzdms 2");
        return errno;
    }


    for (int i = 0; i < MAX_RES; i++) {
        int *params = (int *) malloc(sizeof(int));
        *params = i;
        if (pthread_create(&pids[i], NULL, runner, params) ) {
            perror(NULL);
            return errno;
        } 
    }

    return 0;
}