#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
//#include <semaphore.h>

// int count = 0;
// int nthread = 1;
// int worker_loop_cnt = 1;
int first_count = 0;
int second_count = 0;
int nthread = 1;
int nthread_one = 1;

int main_loop_cnt = 1;

pthread_mutex_t first_lock;
pthread_mutex_t second_lock;
//sem_t semaphore;

static void *work_one(void *num)
{
    long number = (long)num;
    int answer = 0;

    //sem_wait(&semaphore);

    pthread_mutex_lock(&first_lock);
    pthread_mutex_lock(&second_lock);
    answer = first_count + second_count;

    printf("Work_one : %d\n", answer);
    first_count++;
    second_count++;

    pthread_mutex_unlock(&second_lock);
    pthread_mutex_unlock(&first_lock);
    //sem_post(&semaphore);
    return NULL;
}

static void *work_two(void *num)
{
    long number = (long)num;
    int answer = 0;
    //pthread_mutex_lock(&lock);
    //sem_wait(&semaphore);
    pthread_mutex_lock(&second_lock);
    pthread_mutex_lock(&first_lock);
    answer = first_count + second_count;
    printf("Work_two: %d\n", answer);
    first_count++;
    second_count++;
    pthread_mutex_unlock(&first_lock);
    pthread_mutex_unlock(&second_lock);
    //pthread_mutex_unlock(&lock);
    //sem_post(&semaphore);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t *th;
    void *value;
    long i;

    if (argc < 3)
    {
        fprintf(stderr, "%s", argv[0]);
        exit(-1);
    }
    nthread = atoi(argv[1]);
    //worker_loop_cnt = atoi(argv[2]);
    nthread_one = nthread / 2;

    main_loop_cnt = atoi(argv[2]);

    th = malloc(sizeof(pthread_t) * nthread);
    //pthread_mutex_init(&lock, NULL);
    //sem_init(&semaphore,0,1);

    pthread_mutex_init(&first_lock, NULL);
    pthread_mutex_init(&second_lock, NULL);

    for (int loop = 0; loop < main_loop_cnt; loop++)
    {
        printf("----loop %d----", loop);
        for (int i = 0; i < nthread_one; i++)
            assert(pthread_create(&th[i], NULL, work_one, (void *)&i) == 0);
        for (int i = 0; i < nthread; i++)
            assert(pthread_create(&th[i], NULL, work_two, (void *)&i) == 0);
        for (int i = 0; i < nthread; i++)
            assert(pthread_join(th[i], &value) == 0);
        first_count = 0;
        second_count = 0;
    }
    printf("complete");
}