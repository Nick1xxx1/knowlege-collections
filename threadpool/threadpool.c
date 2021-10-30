#include "threadpool.h"
#include <string.h>
#include <stdlib.h>

#define LL_ADD(item, list) do {         \
        item->prev = NULL;              \
        item->next = list;              \
        if (list) list->prev = item;    \
        list = item;                    \
} while(0)

#define LL_REMOVE(item, list) do {                      \
        if (item->prev) item->prev->next = item->next;  \
        if (item->next) item->next->prev = item->prev;  \
        if (item == list) list = item->next;            \
        item->prev = NULL;                              \
        item->next = NULL;                              \
} while(0)

static void *workerLoop(void *arg) {
    threadWorker *worker = (threadWorker *)arg;

    while (1) {
        pthread_mutex_lock(&worker->pool->poolMutex);
        while (!worker->pool->tasks) {
            if (worker->terminate == EXIT) {
                break;
            }
            pthread_cond_wait(&worker->pool->poolCond, &worker->pool->poolMutex);
        }

        if (worker->terminate == EXIT) {
            pthread_mutex_unlock(&worker->pool->poolMutex);
            break;
        }

        // 取出任务并从链表中移除
        threadTask *task = worker->pool->tasks;
        LL_REMOVE(task, worker->pool->tasks);
        pthread_mutex_unlock(&worker->pool->poolMutex);

        task->func(task->userData);
    }

    free(worker);
    pthread_exit(NULL);
}

void cleanupWorkers(threadPool *pool) {
    for (threadWorker *tmp = pool->workers; tmp; tmp = tmp->next) {
        tmp->terminate = EXIT;
    }
}

int createThreadPool(threadPool *pool, int poolNum) {
    if (!pool) {
        return -1;
    }

    if (poolNum < 1) {
        poolNum = 1;
    }

    pool = (threadPool *)malloc(sizeof(threadPool));
    if (!pool) {
        return -1;
    }

    memset(pool, 0, sizeof(threadPool));

    pthread_cond_t blankCond = PTHREAD_COND_INITIALIZER;
    memcpy(&pool->poolCond, &blankCond, sizeof(pthread_cond_t));

    pthread_mutex_t blankMutex = PTHREAD_MUTEX_INITIALIZER;
    memcpy(&pool->poolMutex, &blankMutex, sizeof(pthread_mutex_t));

    for (int i = 0; i < poolNum; ++i) {
        threadWorker *worker = (threadWorker*) malloc(sizeof(threadWorker));
        memset(worker, 0, sizeof(threadWorker));

        if (!worker) {
            cleanupWorkers(pool);
            return -1;
        }

        worker->pool = pool;
        worker->terminate = NOT_EXIT;

        int ret = pthread_create(&worker->workId, NULL, workerLoop, worker);
        if (ret != 0) {
            cleanupWorkers(pool);
            return -1;
        }

        LL_ADD(worker, pool->workers);
    }

    return 0;
}

int destoryThreadPool(threadPool *pool) {
    if (!pool) {
        return -1;
    }

    cleanupWorkers(pool);

    pthread_mutex_lock(&pool->poolMutex);

    pool->tasks = NULL;
    pool->workers = NULL;
    pthread_cond_broadcast(&pool->poolCond);

    pthread_mutex_unlock(&pool->poolMutex);

    return 0;
}

int addThreadPoolTask(threadPool *pool, threadTask *task) {
    if (!pool || !task) {
        return -1;
    }

    pthread_mutex_lock(&pool->poolMutex);

    LL_ADD(task, pool->tasks);
    pthread_cond_signal(&pool->poolCond);

    pthread_mutex_unlock(&pool->poolMutex);

    return 0;
}
