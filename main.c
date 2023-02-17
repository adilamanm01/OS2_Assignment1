
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUF_SIZE 20
#define NUM_ITEMS 30

int buffer[BUF_SIZE];
int sum = 0;
int in = 0, out = 0;
sem_t full, empty;
pthread_mutex_t mutex;

void *producer(void *arg) {
  for (int i = 0; i < NUM_ITEMS; i++) {
    int *x=(int *) arg;
    int y=*x;
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    buffer[in] = i + 1;
    printf("(producer id) i:%d, p%d, buff[%d]:%d, in:%d\n",i,y,in,buffer[in],in);
    in = (in + 1) % BUF_SIZE;
    printf("in value changed to:%d\n\n",in);
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
  }
  return NULL;
}

void *consumer(void *arg) {
  int item;
  for (int i = 0; i < NUM_ITEMS; i++) {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    //printf("i value(consumer id):%d\n",i);
    item = buffer[out];
    out = (out + 1) % BUF_SIZE;
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    sum += item;
    //printf("sum value:%d\n",sum);
  }
  printf("\n\n");
  return NULL;
}

int main() {
  pthread_t t1, t2, t3, t4;
  int a[10];
  a[0]=1;
  a[1]=2;
  sem_init(&full, 0, 0);
  sem_init(&empty, 0, BUF_SIZE);
  pthread_mutex_init(&mutex, NULL);
  pthread_create(&t1, NULL, producer, (void *) &a[0]);
  pthread_create(&t2, NULL, producer, (void *) &a[1]);
  pthread_create(&t3, NULL, consumer, (void *) &a[0]);
  pthread_create(&t4, NULL, consumer, (void *) &a[1]);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  pthread_join(t3, NULL);
  pthread_join(t4, NULL);
  printf("The value of SUM is: %d\n", sum);
  return 0;
}
