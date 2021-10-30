#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <pthread.h>

typedef enum exitStatus {
    NOT_EXIT = 0,
    EXIT
} exitStatus;

typedef struct threadWorker {
    pthread_t workId;
    exitStatus terminate;

    struct threadPool *pool;
    struct threadWorker *prev;
    struct threadWorker *next;
} threadWorker;

typedef struct threadTask {
    void (*func)(void *arg);
    void *userData;

    struct threadTask *prev;
    struct threadTask *next;
} threadTask;

typedef struct threadPool {
    struct threadWorker *workers;
    struct threadTask *tasks;

    pthread_cond_t poolCond;
    pthread_mutex_t poolMutex;
} threadPool;

int createThreadPool(threadPool *pool, int poolNum);
int destoryThreadPool(threadPool *pool);
int addThreadPoolTask(threadPool *pool, threadTask *task);

#endif
