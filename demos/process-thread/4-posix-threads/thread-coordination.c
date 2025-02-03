#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
#define MAX_COUNT 5

// Mutex for thread synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Shared resource
int shared_counter = 0;

// Structure to pass data to threads
typedef struct {
    int thread_id;
    int sleep_time;
} thread_data_t;

// Thread function
void* thread_function(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    
    for (int i = 0; i < MAX_COUNT; i++) {
        // Lock mutex before accessing shared resource
        pthread_mutex_lock(&mutex);
        
        // Critical section
        shared_counter++;
        printf("Thread %d: Incremented counter to %d\n", 
               data->thread_id, shared_counter);
        
        // Unlock mutex
        pthread_mutex_unlock(&mutex);
        
        // Sleep for specified time
        sleep(data->sleep_time);
    }
    
    printf("Thread %d: Finished execution\n", data->thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    
    printf("Main: Starting thread creation\n");
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].sleep_time = i + 1;  // Different sleep times
        
        int rc = pthread_create(&threads[i], NULL, 
                              thread_function, &thread_data[i]);
        
        if (rc) {
            printf("Error creating thread %d\n", i);
            exit(-1);
        }
        
        printf("Main: Created thread %d\n", i);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        printf("Main: Thread %d has completed\n", i);
    }
    
    // Clean up
    pthread_mutex_destroy(&mutex);
    
    printf("Main: Final counter value: %d\n", shared_counter);
    printf("Main: Program completed\n");
    
    return 0;
}