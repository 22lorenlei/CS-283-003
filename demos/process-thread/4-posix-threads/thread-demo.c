#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS 3
#define MAX_COUNT 5

// Mutex for thread synchronization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



// Structure to pass data to threads
typedef struct {
    int thread_id;
    int loop_count;
    int sleep_time;
} thread_data_t;

// Thread function
void* thread_function(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    
    for (int i = 0; i < data->loop_count; i++) {
        printf("[t] - Hello from thread id: %d\n", data->thread_id);
        sleep(data->sleep_time);
    }
    
    printf("Thread %d: Finished execution\n", data->thread_id);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    thread_data_t thread_data;
    
    printf("Main: Starting thread creation\n");
    
    // Create thread
    thread_data.thread_id = 1;
    thread_data.sleep_time = 2;  
    thread_data.loop_count = 10;
    
    int rc = pthread_create(&thread, NULL, 
                            thread_function, &thread_data);
    
    if (rc) {
        printf("Error creating thread %d\n", rc);
        exit(-1);
    }
    
    printf("Main: Created thread\n");

    // Wait for thread to complete
    pthread_join(thread, NULL);
    printf("Main: Thread has completed\n");

    printf("Main: Program completed\n");
    
    return 0;
}