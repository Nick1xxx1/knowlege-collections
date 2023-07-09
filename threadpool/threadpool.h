/**
 * @file threadpool.h
 * @author Nick
 * @brief 线程池实现
 * @version 0.1
 * @date 2023-05-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

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

#endif // THREADPOOL_H_
