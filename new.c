
#include <stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUF_SIZE 20
#define NUM_ITEMS 30

int buffer[BUF_SIZE];
int sum = 0;
int in = 0, out = 0;
sem_t not_empty, empty;
pthread_mutex_t mutex;

void *PRODUCER(void *arg) {
  int i = 0;
  do{
    
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);

    buffer[in] = i + 1;
    in = (in + 1) % BUF_SIZE;
    
    pthread_mutex_unlock(&mutex);
    sem_post(&not_empty);

    i++;
  }
  while(i<30);
  return NULL;
}

void *CONSUMER(void *arg) {
  int item;
  int count=0;
  
  do{
    sem_wait(&not_empty);
    pthread_mutex_lock(&mutex);
    
    item = buffer[out];
    out = (out + 1) % BUF_SIZE;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    
    sum += item;
    count++;
  }
  while(count<30);
  return NULL;
}

int main() {
  
  pthread_mutex_init(&mutex, NULL);

  sem_init(&not_empty, 0, 0);
  sem_init(&empty, 0, BUF_SIZE);

  pthread_t th1,th2,th3,th4;

  pthread_create(&th1, NULL, PRODUCER, NULL);
  pthread_create(&th2, NULL, PRODUCER, NULL);
  pthread_create(&th3, NULL, CONSUMER, NULL);
  pthread_create(&th4, NULL, CONSUMER, NULL);

  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  pthread_join(th3, NULL);
  pthread_join(th4, NULL);

  printf("SUM = %d\n", sum);

  return 0;
}
