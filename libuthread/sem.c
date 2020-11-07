#include <stddef.h>
#include <stdlib.h>
#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	int count;
	bool semUnlocked;
};

sem_t sem_create(size_t count)
{
    sem_t *new_sem;
    *new_sem = (sem_t) malloc(sizeof(struct semaphore));
    if (new_sem != NULL) {
        new_sem -> count  = count;
        new_sem->semUnlocked = 1;
    }
    return *new_sem;
}

int sem_destroy(sem_t sem)
{
	if (sem == NULL){
        return -1;
	}
	sem->count = 0;
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
    if(sem == NULL){
        return -1;
    }
    sem->count = sem->count - 1;
    while(sem->count < 0){
        //block process;
    }
    return 0;
}

int sem_up(sem_t sem)
{
	if(sem == NULL){
        return -1;
	}
	sem->count += 1;
	while (sem->count <= 0){
	    //block process
	}
	return 0;
}

