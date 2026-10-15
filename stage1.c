#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h> 
#include <pthread.h>
#include <semaphore.h>

#define Thread_num 3


void *task(void *argv)
{
    int i;
    int thread_name = *(int*)argv;
    for (i=0;i<5;i++)
    {
        printf("[Thread %d ID %lu] Send %d\n",thread_name,(long unsigned)pthread_self(),i+1);
    }
}

int main(int argc, char const *argv[])
{
    pthread_t thread[Thread_num];
    int i;
    int arg[Thread_num];
    for (i=0;i < Thread_num;i++)
    {
        arg[i] = i+1;
        int ret = pthread_create(thread+i,NULL,task,&arg[i]);
        if(ret!=0)
        {
            printf("create error\n");
        }
    }

    for (i=0;i < Thread_num;i++)
    {
        int back = pthread_join(thread[i],NULL);
        if(back!=0)
        {
            printf("join error\n");
        }
    }

    printf("Recieve result1: %ld\n",thread[0]);
    printf("Recieve result2: %ld\n",thread[1]);
    printf("Recieve result3: %ld\n",thread[2]);

    
    return 0;
}
