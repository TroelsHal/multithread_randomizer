#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

// Mutex to ensure exclusive access to shared counter and number
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

// Condition variable to synchronize threads
pthread_cond_t condition;

int counter; // Count of iterations
unsigned int number; // Resulting random number

// There is an extra (9th) thread to make sure that the number of
// bit-flips is not always an equal number.
int num_threads = 9; 

void* worker (void *arg) {
    int index = (int)(intptr_t)arg;
    int shift = 128 >> index;

    while(1) {
        // Acquire lock to protect shared data
        assert(pthread_mutex_lock(&counter_mutex) == 0);

        // Check termination condition, release lock, and exit if done
        if (counter >= 200) {
            assert(pthread_mutex_unlock(&counter_mutex) == 0);
            break;    
        }

        // Increment shared counter
        counter++;

        // XOR operation on shared number
        number = number ^ shift;

        // Signal all waiting threads to proceed
        assert(pthread_cond_broadcast(&condition) == 0);

        // Wait for a signal from another thread, releasing lock during wait
        assert(pthread_cond_wait(&condition, &counter_mutex) == 0);

        // Release lock
        assert(pthread_mutex_unlock(&counter_mutex) == 0);
    }

    // Signal remaining threads to avoid any waiting forever
    assert(pthread_cond_broadcast(&condition) == 0);
    return NULL;
}

int main() {
    counter = 0;
    number = 0;

    // Allocate memory for thread handles
    pthread_t *threads = calloc(num_threads, sizeof(pthread_t));

    // Initialize condition variable
    assert(pthread_cond_init(&condition, NULL) == 0);

    // Create threads, passing index as argument to each
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, &worker, (void*)(intptr_t)i);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        assert(pthread_join(threads[i], NULL) == 0);
    }

    // Free allocated memory for thread handles
    free(threads);

    // Print the resulting random number
    printf("Your random number is: %d\n", number);
}
