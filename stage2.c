#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h> 
#include <pthread.h>
#include <semaphore.h>

/*互斥锁的添加让一个进程执行完后解锁后再进行其他进程，这样使得对共享数据的操作变得更准确，维护了线程安全*/

#define addnum 10000
int share_data = 0;
//static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *task(void *argv)
{   
    int i;
    //pthread_mutex_lock(&mutex);
    for (i=0;i<addnum;i++)
    {
        int tmp = share_data+1;
        share_data = tmp;
    }
    //pthread_mutex_unlock(&mutex);
    
}

int main(int argc, char const *argv[])
{
    pthread_t threadname[3];
    int i;
    for (i=0;i<3;i++)
    {
        int ret = pthread_create(threadname+i,NULL,task,NULL);
        if(ret!= 0)
        {
            fprintf(stderr,"Create: %s\n",strerror(ret));
        }
    }
    for (i=0;i<3;i++)
    {
        int back = pthread_join(threadname[i],NULL);
        if(back!= 0)
        {
            fprintf(stderr,"Join: %s\n",strerror(back));
        }
    }
    printf("share data:%d\n",share_data);
    return 0;
}

