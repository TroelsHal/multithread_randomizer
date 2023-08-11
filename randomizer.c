#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

#define NUM_ITERATIONS 200

typedef struct {
    pthread_mutex_t* counter_mutex;
    pthread_cond_t* condition;
    int* counter; 
    unsigned int* number;
    int index;
} ThreadData;

void* worker (void *arg) {
    ThreadData *data = (ThreadData*)arg; // Cast the argument back to the correct type

    int shift = 128 >> data->index;

    while(1) {

        // Acquire lock to protect shared data
        assert(pthread_mutex_lock(data->counter_mutex) == 0);
        
        // Check termination condition, release lock, and exit if done
        if ((*data->counter) >= NUM_ITERATIONS) {
            assert(pthread_mutex_unlock(data->counter_mutex) == 0);
            break;    
        }

        // Increment shared counter
        (*data->counter)++;

        // XOR operation on shared number
        (*data->number) = (*data->number) ^ shift;

        // Signal all waiting threads to proceed
        assert(pthread_cond_broadcast(data->condition) == 0);

        // Wait for a signal from another thread, releasing lock during wait
        assert(pthread_cond_wait(data->condition, data->counter_mutex) == 0);

        // Release lock
        assert(pthread_mutex_unlock(data->counter_mutex) == 0);

    }

    // Signal remaining threads to avoid any waiting forever
    assert(pthread_cond_broadcast(data->condition) == 0);
    return NULL;
}

int main() {
    // Counting number of bit shifts performed by all threads
    int counter = 0;
    // The number that all threads modify 
    unsigned int number = 0;
    // Mutex to ensure exclusive access to shared counter and number
    pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
    // Condition variable to synchronize threads
    pthread_cond_t condition;

    // There is an extra (9th) thread to make sure that the number of
    // bit-flips is not always an equal number.
    int num_threads = 9;

    ThreadData* thread_data = (ThreadData*)malloc(num_threads * sizeof(ThreadData));

    // Allocate memory for thread handles
    pthread_t *threads = calloc(num_threads, sizeof(pthread_t));

    // Initialize condition variable
    assert(pthread_cond_init(&condition, NULL) == 0);

    // Create threads, passing index as argument to each
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].counter_mutex = &counter_mutex;
        thread_data[i].condition = &condition;
        thread_data[i].counter = &counter;
        thread_data[i].number = &number;
        thread_data[i].index = i;
        
        pthread_create(&threads[i], NULL, &worker, &thread_data[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        assert(pthread_join(threads[i], NULL) == 0);
    }

    // Free resources used by the condition variable
    assert(pthread_cond_destroy(&condition) == 0);

    // Free allocated memory for thread handles
    free(threads);
    free(thread_data);

    // Print the resulting random number
    printf("Your random number is: %u\n", number);

    return EXIT_SUCCESS;
}
