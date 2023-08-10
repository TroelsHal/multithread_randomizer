#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition;

int counter;
unsigned int number;

// Thread number 9 doesn't do anything: It XORs with 0.
// But if we only had 8 threads, then there would always be exactly
// 200 bit-flips, and we would always end up with an even number of
// 1-bits and an even number of 0-bits. 
int num_threads = 9;

void* worker (void *arg) {
    int index = (int)(intptr_t)arg; // Correct way to convert void* to int
    int shift = 128 >> index;
    
    while(1) {
        assert(pthread_mutex_lock(&counter_mutex) == 0);
        if (counter >= 200) {
            assert(pthread_mutex_unlock(&counter_mutex) == 0);
            break;    
        }

        counter++;
        number = number ^ shift;

        //The same thread should not have the lock twice.
        //So it wakes up other threads, and then goes to sleep itself.
        assert(pthread_cond_broadcast(&condition) == 0);
        assert(pthread_cond_wait(&condition, &counter_mutex) == 0);

        assert(pthread_mutex_unlock(&counter_mutex) == 0);
    }

    //Before terminating, wake up other threads,
    //so the last thread alive is not left sleeping forever.
    assert(pthread_cond_broadcast(&condition) == 0);
    return NULL;
}

int main() {
    counter = 0;
    number = 0;

    pthread_t *threads = calloc(num_threads, sizeof(pthread_t));
    assert (pthread_cond_init(&condition, NULL) == 0);

    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, &worker, (void*)(intptr_t)i);
    }

    for (int i = 0; i < num_threads; i++) {
        assert(pthread_join(threads[i], NULL) == 0);
    }

    free(threads);
    printf("Your random number is: %d\n", number );
}
