// Tests how many threads a process can spawn.
// Install time, and run `/usr/bin/time -v ./thread_stress_test`
// to see maximum memory usage and other statistics.
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 15000

// Function executed by each thread
void* thread_function(void* thread_id) {
    sleep(1);
    printf("%ld, ", (long) thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];

    // Set aside only 64 KiB of virtual memory per thread
    // Note that the kernel usually only maps virtual pages
    // to physical pages when they're accessed.
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 64 * 1024);

    // Create multiple threads
    for (long t = 0; t < NUM_THREADS; t++) {
        int ret =
            pthread_create(&threads[t], &attr, thread_function, (void*) t);
        if (ret != 0) {
            perror("pthread_create");
            printf("pthread_create returned %d\n", ret);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to complete
    for (int t = 0; t < NUM_THREADS; t++) {
        int ret = pthread_join(threads[t], NULL);
        if (ret != 0) {
            perror("pthread_join");
            printf("pthread_join returned %d\n", ret);
            exit(EXIT_FAILURE);
        }
    }

    printf("\nMain: All threads completed.\n\n");
    pthread_exit(NULL);
}
