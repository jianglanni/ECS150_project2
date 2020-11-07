#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "queue.h"
#include "private.h"
#include "uthread.h"

struct uthread_tcb {
    // Need default values;
	void *sp; //stack pointer;
    uthread_ctx_t *context;
    int state;
};

struct uthread_tcb *uthread_current(void)
{
    return current_thread;
}

void uthread_yield(void)
{

}

void uthread_exit(void)
{

}

int uthread_create(uthread_func_t func, void *arg)
{
    uthread_tcb newThread = (uthread_tcb) malloc(sizeof(uthread_tcb));
    if(newThread == NULL){
        return -1;
    }
    newThread.sp = uthread_ctx_alloc_stack();
    uthread_ctx_init(newThread.context, newThread.sp, func, arg);
    newThread.state = 0;
    return 0;
}

int uthread_start(uthread_func_t func, void *arg)
{
    uthread_tcb newThread = (uthread_tcb) malloc(sizeof(uthread_tcb));
    if(newThread == NULL){
        return -1;
    }
    newThread.sp = uthread_ctx_alloc_stack();
    uthread_ctx_init(newThread.context, newThread.sp, func, arg);
    newThread.state = 0;

    return 0;
}

void uthread_block(void)
{
	/* TODO Phase 2/3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 2/3 */
}
