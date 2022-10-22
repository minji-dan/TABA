#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>

int count = 0;
int nthread = 1;
int worker_loop_cnt = 1;

static void *work(void *num)
{
    long number = (long)num;

    for (int i = 0; i < worker_loop_cnt; i++)
        count++;

    printf("Thread number: %ld: %d \n", number, count);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t *th; //thread id
    void *value;
    long i;

    if (argc < 3)
    {
        fprintf(stderr, "%s parameter: nthread, worker_loop_cnt\n", argv[0]);
        exit(-1);
    }
    nthread = atoi(argv[1]);
    worker_loop_cnt = atoi(argv[2]);

    th = malloc(sizeof(pthread_t) * nthread);

    for (int i = 0; i < nthread; i++)
        assert(pthread_create(&th[i], NULL, work, (void *)&i) == 0); //thread 생성
    for (int i = 0; i < nthread; i++)
        assert(pthread_join(th[i], &value) == 0); //main을 도는 스레드가 자신이 분기시킨 스레드들이 종료되기를 기다리는 함수
    printf("Complete\n");
}
