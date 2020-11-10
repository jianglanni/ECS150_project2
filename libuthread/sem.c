#include <stddef.h>
#include <stdlib.h>
#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
        int count;
        queue_t waitlist;   //has the list of threads waiting for a resource.
};

/*
 * thread asking for or releasing resource.
 * It will be set to blocked state if sem count is 0
 */
//struct uthread_tcb asking_thread;

sem_t sem_create(size_t count)
{
        sem_t new_sem;
        new_sem = (sem_t) malloc(sizeof(struct semaphore));
        if (new_sem == NULL){
                return NULL;
        }

        new_sem -> count  = count;
        new_sem -> waitlist = queue_create();

        return new_sem;
}

int sem_destroy(sem_t sem)
{
	if (sem == NULL){
        	return -1;
	}

	queue_destroy(sem -> waitlist);
	free(sem);

	return 0;
}

int sem_down(sem_t sem)
{
        if(sem == NULL){
            return -1;
        }

        if (sem -> count == 0) {
                queue_enqueue(sem -> waitlist, (void*) uthread_current());
                uthread_block();
        }

        sem -> count -= 1;

        return 0;
}

int sem_up(sem_t sem)
{
        if(sem == NULL){
            return -1;
        }

        if(sem -> count == 0){
                if(queue_length(sem -> waitlist)){
                    struct uthread_tcb* unblocked_tcb;
                    queue_dequeue(sem -> waitlist, (void**) &unblocked_tcb);
                    uthread_unblock(unblocked_tcb);
                }
        }
        
        sem -> count += 1;

        return 0;
}
