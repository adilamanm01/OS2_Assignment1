

//declaration of required library
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//declaration of global values
#define NUM 30 //number of items 
#define SIZE 20 //size of buffer

//declaration of required glogal variables
int buffer[SIZE];  //declaring buffer 
int sum = 0;      //sum declaration as zero
int in = 0, out = 0; //in and out declaration
sem_t not_empty, empty; //global semaphore declaration 
pthread_mutex_t mutex;  //declarion of mutex which helps to lock and unlock the buffer access


//declaration of producer function
void *PRODUCER() {

  int i = 0; //declarion of required variables

  //producer logic
  do{

    //entry section
    sem_wait(&empty); //it allows to enter only if buffer is empty
    pthread_mutex_lock(&mutex); //it locks the buffer

    //printf("%d\n",i);
    
    //critical section
    buffer[in] = i + 1;
    in = (in + 1) % SIZE; 

    //exit section
    pthread_mutex_unlock(&mutex); //it unlocks the buffer so that next other required access
    sem_post(&not_empty); //it unlocks semaphore referred by sem

    //remainder section
    i++; //doing i+1 for moving into next iteration
  }
  while(i<NUM); //used do while for the following logic

  return NULL;
}

void *CONSUMER() {
  //declaration of required variables
  int item;
  int count=0;
  
  //consumer logic
  do{
    //entry section
    sem_wait(&not_empty); //it allows to enter only if buffer is not empty
    pthread_mutex_lock(&mutex); //it locks the buffer
    
    //critical section
    item = buffer[out];
    out = (out + 1) % SIZE;

    //exit section 
    pthread_mutex_unlock(&mutex); //it unlocks the buffer so that next other required access
    sem_post(&empty); //it unlocks semaphore referred by sem
    
    //remainder section
    sum += item; //logic to store the sum value in every iteration
    count++;     //doing count+1 for moving into next iteration
  }
  while(count<NUM); //used do while for the following logic

  
  return NULL;
}

int main() {

  //declaration of pthread mutex
  pthread_mutex_init(&mutex, NULL);

  //declaration of semaphore limits 
  sem_init(&empty, 0, SIZE);
  sem_init(&not_empty, 0, 0);
  
  //declaration of 4 threads 
  pthread_t pt1,pt2,pt3,pt4;

  //Creation of 2 threads for 2 Producers
  pthread_create(&pt1, NULL, PRODUCER, NULL);
  pthread_create(&pt2, NULL, PRODUCER, NULL);

  //creation of 2 threads for 2 Consumers
  pthread_create(&pt3, NULL, CONSUMER, NULL);
  pthread_create(&pt4, NULL, CONSUMER, NULL);


  //joining 
  pthread_join(pt1, NULL);
  pthread_join(pt2, NULL);
  pthread_join(pt3, NULL);
  pthread_join(pt4, NULL);

  //Printing the final Sum values
  printf("SUM = %d\n", sum);

  return 0;
}
