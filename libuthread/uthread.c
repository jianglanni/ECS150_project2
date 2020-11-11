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

queue_t tcb_queue = NULL; //where we keep the TCBs (ready)

queue_t garbage_tcb = NULL; //where we keep the exited TCBs

struct uthread_tcb {
	int state; //0 for ready/idle, 1 for running, 2 for blocked.
	uthread_ctx_t* ctx;
	void *top_of_stack;
};

struct uthread_tcb* main_thread; //Where we keep the main thread.

struct uthread_tcb* current_running;

struct uthread_tcb *uthread_current(void)
{
	return current_running;
}

void uthread_yield(void)
{
	current_running -> state = 0;
	struct uthread_tcb* yielded_thread = current_running;
	queue_enqueue(tcb_queue, (void*) uthread_current()); //Put itself in line.
	queue_dequeue(tcb_queue, (void**) &current_running); //Set the next thread as the running thread.
	current_running -> state = 1;
	uthread_ctx_switch(yielded_thread -> ctx, current_running -> ctx); //Go to the next thread.
}

void uthread_exit(void)
{
	current_running -> state = 0;
	queue_enqueue(garbage_tcb, (void*) uthread_current()); //Get into the garbage bin. 
	struct uthread_tcb* exited_thread = current_running;
	queue_dequeue(tcb_queue, (void**) &current_running); //Set the next thread as the running thread.
	current_running -> state = 1;
	uthread_ctx_switch(exited_thread -> ctx, current_running -> ctx); //Go to the next thread.
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
	return 0;
}

int uthread_start(uthread_func_t func, void *arg)
{
	tcb_queue = queue_create();
	garbage_tcb = queue_create();

	/* Build the TCB for main idle thread */
	main_thread = (struct uthread_tcb*) malloc(sizeof(struct uthread_tcb));
	if (main_thread == NULL)
		return -1;
	main_thread -> state = 1; //it's running.
	main_thread -> ctx = malloc(sizeof(uthread_ctx_t)); //save for later.
	main_thread -> top_of_stack = NULL; //we won't use it.
	current_running = main_thread;

	uthread_create(func, arg);

	while (1) {
		while (queue_length(garbage_tcb) > 0) {
			void* discard;
			queue_dequeue(garbage_tcb, &discard);
			free(discard);
		}

		if (!queue_length(tcb_queue))
			break;

		uthread_yield();
	}

	queue_destroy(tcb_queue);
	tcb_queue = NULL; //Reset
	return 0;
}

void uthread_block(void)
{
	current_running -> state = 2;
	struct uthread_tcb* blocked_thread = current_running;
	queue_dequeue(tcb_queue, (void**) &current_running); //Set the next thread as the running thread.
	current_running -> state = 1;
	uthread_ctx_switch(blocked_thread -> ctx, current_running -> ctx);
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	uthread -> state = 0;
	queue_enqueue(tcb_queue, (void*) uthread);
}

