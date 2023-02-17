#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h>

#define BUFFER_SIZE 20 
#define TRUE 1 

sem_t full, empty; 
int buffer[BUFFER_SIZE]; 
int count = 0; 
int i, j; 
int SUM = 0;

// Function that a producer thread executes 
void *producer(void *arg) 
{ 
    // Entry Section
    int x;
    for(x=1;x<=30;x++) 
    { 
        // Waits for empty semaphore to be non-zero
        sem_wait(&empty); 
        // Critical Section
        buffer[count] = x; 
        count = count + 1; 
        printf("Produced item: %d\n", x); 
        // signal full semaphore 
        sem_post(&full); 
    } 
    return NULL; 
} 
// Function that a consumer thread executes 
void *consumer(void *arg) 
{ 
    // Entry Section
    int x; 
    for(x=1;x<=30;x++) 
    { 
        // Waits for full semaphore to be non-zero 
        sem_wait(&full); 
        // Critical Section
        count = count - 1; 
        printf("Consumed item: %d\n", buffer[count]); 
        SUM +=buffer[count]; 
        // signal empty semaphore 
        sem_post(&empty); 
    } 
    return NULL; 
} 

int main() 
{ 
    pthread_t pid, cid; 
    sem_init(&full, 0, 0); 
    sem_init(&empty, 0, BUFFER_SIZE); 
    pthread_create(&pid, NULL, producer, NULL); 
    pthread_create(&cid, NULL, consumer, NULL); 
    pthread_join(pid, NULL); 
    pthread_join(cid, NULL); 
    printf("Final value of SUM: %d\n", SUM); 
    return 0; 
} 
