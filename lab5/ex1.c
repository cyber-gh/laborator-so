#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

// to many libraries



int arr[] = {9, 16, 25, 36};
int sz = sizeof(arr) / sizeof(arr[0]);
const char* shm_name = "shared_memory";

int shm_fd; // descriptor used to access the memory

size_t shm_size = 0;

int PAGE_SIZE = 0;


int main() {

    shm_size = getpagesize() * sz ;
    PAGE_SIZE = getpagesize();
    shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (shm_fd < 0) {
        perror("pzdms 1");
        return errno;
    }

    //printf("size = %d ", shm_size);
    if (ftruncate(shm_fd, shm_size) == -1) {
        perror("pzdms 2");
        shm_unlink(shm_name);
        return errno;
    }

    for (int i = 0; i < sz; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("pzdms 3");
            //todo
            return errno;
        }
        if (pid == 0) {
            void* shm_ptr = mmap(0, PAGE_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, PAGE_SIZE * i);
            if (shm_ptr == MAP_FAILED) {
                perror("pzdms 8");
                return errno;
            }
            int *ptr = (int *)shm_ptr;
            printf(" size = %d\n", sizeof(ptr));

            int nr = arr[i];
            int index = 0;
            while ( 1 ) {
                printf("%d ", nr);
                ptr[index++] = nr;
                if (nr == 1) break;
                if (nr % 2 == 0) nr /= 2;
                else nr = nr * 3 + 1;
                
            }
            printf("\n");
            if (munmap(shm_ptr, PAGE_SIZE) != 0) {
                perror("pzdms 4");
                return errno;
            }
            return 0;
        } else {
            //nothing
        }
    }

    for (int i = 0; i < sz; i++) wait(NULL);

    for (int i = 0; i < sz; i++) {
        void *shm_ptr = mmap(0, PAGE_SIZE, PROT_READ, MAP_SHARED, shm_fd, PAGE_SIZE * i);
        int *ptr = (int *)shm_ptr;
        int index = 0;
        while (ptr[index] != 1) {
            printf("%d ", ptr[index]);
            index++;
            if (ptr[index] == 0) break;
        }
        printf("\n");
        if (munmap(shm_ptr, PAGE_SIZE) ) {
            perror("pzdms 5");
            return errno;
        }
    }


    shm_unlink(shm_name);
    return 0;
}