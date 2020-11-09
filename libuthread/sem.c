#include <stddef.h>
#include <stdlib.h>
#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	int count;
    queue_t waitList;   //has the list of threads waiting for a resource.
    int isLocked; // set to 1 if a sem is locked
};

/*
 * thread asking for or releasing resource.
 * It will be set to blocked state if sem count is 0
 */
//struct uthread_tcb asking_thread;

sem_t sem_create(size_t count)
{
    sem_t *new_sem;
    *new_sem = *(sem_t) malloc(sizeof(struct semaphore));
    if (new_sem == NULL){
        return 0;
    }

    new_sem->count  = count;
    new_sem->waitlist = queue_create();
    new_sem->isLocked = 0;

    return *new_sem;
}

int sem_destroy(sem_t sem)
{
	if (sem == NULL){
        return -1;
	}
	sem->count = 0;
	sem->isLocked = 0;
	queue_destroy(sem->waitList);
	free(sem);

	return 0;
}

int sem_down(sem_t sem)
{
    if(sem == NULL){
        return -1;
    }

    while(sem->count == 0){
        uthread_block();
        queue_enqueue(sem->waitlist,uthread_current());
    }

    uthread_unblock(uthread_current());
    queue_dequeue(sem->waitList,&uthread_current());
    sem->count -= 1;
    sem->isLocked = 1;

    return 0;
}

int sem_up(sem_t sem)
{
    struct uthread_tcb* curr = uthread_current();
    if(sem == NULL){
        return -1;
    }

    if(sem->count == 0){
        if(queue_length(sem->waitList)){
            uthread_unblock(curr);
            queue_dequeue(sem->waitList, &uthread_current());
        }
        sem->count += 1;
        sem->isLocked = 0;
    }

    return 0;
}
