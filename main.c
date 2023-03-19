#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

int nmax=14;
int counter=0;
int readmax=250000000;
int flag=0;

sem_t mutex,wrt;



void* relax()
{
    int i;
    for(i=0;i<250000000;i++)
    {
        i=i;
    }
}

void* writer_thread(void* arg)
{
    int i=0;
    flag=1;
    sem_wait(&wrt);
    while(i<25000)
    {
        counter=counter+1;
    }
    printf("writer done\n");
    flag=0;
    sem_post(&wrt);
    return NULL;
}

void* reader_thread(void* arg)
{
    int *x=(int *) arg;
    int y=*x;

    sem_wait(&mutex);
    if(flag==1)
    {
        printf("error\n");
    }
    else
    {
        int temp=counter;
        printf("reader done :%d\n",temp);
    }
    sem_post(&mutex);
    relax();
    return NULL;
}

int main(int argc,char* argv[])
{
    int i,k,n;
    n=atoi(argv[1]);
    //printf("%d\n",n);
    k = (int) (n/2);
    //printf("%d\n",n);

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    pthread_t readers[nmax],writer[nmax];

    if(argc==2)
    {
        //printf("okay\n");
        if(n>0 && n<15)
        {
            //printf("okay2\n");
            for(i=0;i<k;i++)
            {
                for(i = 0; i < k; i++)
                    {
                        //printf("%d\n",i);
                        pthread_create(&readers[i],NULL, reader_thread,(void*) i);
                    }

                    for(i = 0; i < k; i++)
                    {
                        //printf("%d\n",i);
                        pthread_join(readers[i],NULL);
                    }

                    pthread_create(&writer[0], NULL, writer_thread,NULL);
                    pthread_join(writer[0],NULL);

                    for(i = k ; i < n ; i++)
                        {
                            pthread_create(&readers[i], NULL, reader_thread,(void*) i);
                        }
                    for(i = k ; i < n ; i++)
                        {
                            pthread_join(readers[i], NULL);
                        }


            }
        }
        else{
            printf("invalid : n should be more than 0 and less than 15\n");
        }
    }
    else{
        printf("enter valid input\n");
    }

    return 0;
}
