#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define BUFFER_SIZE 20
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define MAX_VALUE 30

int buffer[BUFFER_SIZE];
int sum = 0;
int buffer_count = 0;
int buffer_index = 0;

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

void *producer(void *arg) {
    int id = *((int *) arg);
    int value;

    for (int i = 1; i <= MAX_VALUE; i++) {
        value = i + id * MAX_VALUE; // each producer has a different offset
        sem_wait(&empty); // wait for an empty slot in the buffer
        pthread_mutex_lock(&mutex);
        buffer[buffer_index] = value;
        buffer_index = (buffer_index + 1) % BUFFER_SIZE;
        buffer_count++;
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // signal that a slot is now full
    }

    return NULL;
}

void *consumer(void *arg) {
    int id = *((int *) arg);
    int value;

    for (int i = 0; i < MAX_VALUE * NUM_PRODUCERS; i++) {
        sem_wait(&full); // wait for a slot to be full in the buffer
        pthread_mutex_lock(&mutex);
        value = buffer[buffer_index];
        buffer_index = (buffer_index + 1) % BUFFER_SIZE;
        buffer_count--;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // signal that a slot is now empty
        sum += value;
    }

    return NULL;
}

int main() {
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS];
    int consumer_ids[NUM_CONSUMERS];

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producer_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumer_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        pthread_join(consumers[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    printf("SUM = %d\n", sum);

    return 0;
}
