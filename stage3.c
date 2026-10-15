#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h> 
#include <pthread.h>
#include <semaphore.h>

#define Task_Num 5
#define Producer_Num 3
#define Consumer_Num 4
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_full  = PTHREAD_COND_INITIALIZER;   // 生产者等
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;   // 消费者等
int done = 0;
int producer_finished = 0;
int taskconsum = 0;
typedef struct Task
{
    int task_id;
    int data;
}Task;

typedef struct Taskqueue
{
    Task taskqueue[Task_Num];

    int front;
    int rear;
    int count;
}Queue;

Queue test;

void queue_init(Queue *queue)
{
    
    queue->count = 0;
    queue->front = 0;
    queue->rear = 0;
    
}
/*Queue结构体是共用的，所以传入指针，每个任务的数据和id不一样所以不需要传指针*/
int queue_push(Queue *queuetask,Task task)
{
    
    if(queuetask->count >= Task_Num)
    {
        
        return 0;
    }
    queuetask->taskqueue[queuetask->rear] = task;
    queuetask->rear = (queuetask->rear + 1) % Task_Num;
    queuetask->count++;
    
    return 1;
}

int queue_pop(Queue *queuetask,Task *task)
{
    
    if(queuetask->count==0)
    {
        
        return 0;
        
    }
    *task = queuetask->taskqueue[queuetask->front];
    queuetask->front = (queuetask->front+1) % Task_Num;
    queuetask->count--;
    
    return 1;
}

void *producer(void *argv)
{
    Task t;
    int i;
    // while(1)
    // {
    int productname = *(int*)argv;
    for(i = 0;i<20;i++)
    {
        pthread_mutex_lock(&mutex);
        while(test.count == Task_Num)
        {
            pthread_cond_wait(&not_full,&mutex);
        }
        t.task_id = i;
        t.data = (productname*100)+ i;
        if(!queue_push(&test,t))
        {
            
        }else{
            printf("product%d id:%d,data:%d\n",productname,t.task_id,t.data);
            printf("front:%d rear:%d count:%d\n",test.front,test.rear,test.count);
        }
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    // }
    }

    // 关键修改：所有生产者都结束后，才设置 done
    pthread_mutex_lock(&mutex);
    producer_finished++;
    if(producer_finished == Producer_Num)
    {
        done = 1;
        pthread_cond_broadcast(&not_empty);  // 唤醒所有等待消费者,避免出现只唤醒一个，而其他仍在等待导致join一直阻塞
    }
    pthread_mutex_unlock(&mutex);

}

void *consumer(void *argv)
{
    Task t;
    int i;
    int consumername = *(int*)argv;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(test.count == 0 && !done)
        {
            pthread_cond_wait(&not_empty,&mutex);
        }
        if(done && (test.count == 0))
        {   
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }

        if(!queue_pop(&test,&t))
        {
            
        }else
        {
            printf("consume%d id: %d data: %d\n",consumername,t.task_id,t.data);
            printf("front:%d rear:%d count:%d\n",test.front,test.rear,test.count);
            taskconsum++;
        }
        
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
    }
    
}

int main(int argc, char const *argv[])
{
    pthread_t Producer[Producer_Num],Consumer[Consumer_Num];
    queue_init(&test);
    int i;
    int producerid[Producer_Num];
    for(i = 0;i<Producer_Num;i++)
    {
        producerid[i] = i+1;
        int ret = pthread_create(Producer+i,NULL,producer,&producerid[i]);
        if(ret!=0)
        {
            fprintf(stderr,"thread create:%s\n",strerror(ret));
        }
    }
    int conductid[Consumer_Num];
    for(i = 0;i<Consumer_Num;i++)
    {
        conductid[i] = i+1;
        int con = pthread_create(Consumer+i,NULL,consumer,&conductid[i]);
        if(con!=0)
        {
            fprintf(stderr,"thread create:%s\n",strerror(con));
        }
    }

    for(i = 0;i<Producer_Num;i++)
    {
        int Pjoin = pthread_join(Producer[i],NULL);
        if(Pjoin)
        {
            fprintf(stderr,"producer join :%s\n",strerror(Pjoin));
        }
    }

    for(i = 0;i<Consumer_Num;i++)
    {
        int Cjoin = pthread_join(Consumer[i],NULL);
        if(Cjoin)
        {
            fprintf(stderr,"consumer join :%s\n",strerror(Cjoin));
        }
    }
    printf("Task consum num: %d\n",taskconsum);
    
    return 0;
}
