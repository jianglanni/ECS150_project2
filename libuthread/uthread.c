#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

uthread_ctx_t main_ctx; //Where we keep the ctx for main thread.

queue_t tcb_queue = NULL; //where we keep the TCBs

int thread_is_exit = 0; //Modified to 1 when some thread calls exit()

struct uthread_tcb {
	int state; //0 for ready, 1 for running, 2 for blocked.
	uthread_ctx_t* ctx;
	void *top_of_stack;
};

struct uthread_tcb* current_running;

struct uthread_tcb *uthread_current(void)
{
	return current_running;
}

void uthread_yield(void)
{
	current_running -> state = 0;
	queue_enqueue(tcb_queue, (void*) uthread_current()); //Put itself in line.
	uthread_ctx_switch(current_running -> ctx, &main_ctx); //Go back to main thread.
}

void uthread_exit(void)
{
	thread_is_exit = 1; //Let main thread know that there is an exit.
	uthread_ctx_switch(current_running -> ctx, &main_ctx);
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *created_thread = NULL;

	created_thread = (struct uthread_tcb*) malloc(sizeof(struct uthread_tcb)); //Allocate memory space for TCB.
	if (created_thread == NULL)
		return -1;

	created_thread -> top_of_stack = uthread_ctx_alloc_stack(); //Handle the stack.

	created_thread -> ctx = malloc(sizeof(uthread_ctx_t));
	uthread_ctx_init(created_thread -> ctx, created_thread -> top_of_stack, func, arg);
	
	queue_enqueue(tcb_queue, (void*) created_thread); //Add the created TCB into the global queue. 
}

int uthread_start(uthread_func_t func, void *arg)
{
	tcb_queue = queue_create();
	uthread_create(func, arg);

	while (queue_length(tcb_queue)) {
		queue_dequeue(tcb_queue, (void**) &current_running);
		current_running -> state = 1;
		uthread_ctx_switch(&main_ctx, current_running -> ctx); //Jump to the scheduled func

		// ** A thread is running... and done! ** //

		if (thread_is_exit) { //Exit is called
			uthread_ctx_destroy_stack(current_running -> top_of_stack);
			free(current_running);
			thread_is_exit = 0; //reset the exit note.
		}
	}

	queue_destroy(tcb_queue);
	tcb_queue = NULL; //Reset
}

void uthread_block(void)
{
	current_running -> state = 2;
	uthread_ctx_switch(current_running -> ctx, &main_ctx);
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	uthread -> state = 0;
	queue_enqueue(tcb_queue, (void*) uthread);
}

