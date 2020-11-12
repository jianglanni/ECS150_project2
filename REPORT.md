##### Authors: Lan Jiang, Manan Shah

#### Phase 1: Implementing a Queue

In this phase, we choose linked list as the data structure, because we will not
do random access. Also, we do not have to worry about the memory management of
linked list since each node's memory space is independent from others'. Besides, 
deleting a node in the mid of a linked list is easy, so the implementation of 
`queue_delete()` will be just like reassigning some pointers. In 
`struct queue`, we have a pointer `head`, where we dequeue data from the queue, 
and a pointer `tail`, where we enqueue data to the queue, since this is a FIFO 
data structure. Besides, we have a `int size` that changes whenever an enqueue, 
a dequeue, or a delete is called, so that we can easily track the length of the 
queue. 


#### Phase 2: Implementing a User Level Thread

The implementation of uthread is the core of this project. First, a TCB is 
composed by its state, context, and stack pointer, which is the environment of
a thread. We keep the TCBs inside `tcb_queue` , which is a FIFO data structure 
we made in phase 1, since our scheduling algorithm is First in First served. 
A thread that calls `uthread_yield()` will add its TCB back to `tcb_queue` to 
get scheduled later, while a thread which calls `uthread_exit()` adds its TCB 
to `garbage_tcb`, waiting to be collected so that we can recycle the memory 
space. Functions `uthread_yield()`, `uthread_exit()`, and `uthread_block()` are
all critical sections because we do some jobs preparing for context switching 
and the actions are not supposed to be interrupted. So, we disable preemption 
when entering the functions and restore when finished. `uthread_start()` is the 
backbone of the implementation, it creates the first thread from the arguments it 
takes and registers the current execution flow as a manageable thread, so that 
it can call `uthread_yield()` to yield the CPU to the next thread. Besides, it 
cleans the exited threads whose TCBs are in queue `garbage_tcb` to recycle some 
memory space. When `tcb_queue` is empty, the main thread stop yielding and clear 
the memory space used by the data structures of uthread. 


#### Phase 3: Implementing a Semaphore and Block mechanism

The semaphore struct has 2 member variables: `waitlist` and `count`. The `count` 
simulates the internal count of the semaphore. As for `waitlist`, if a thread 
calls `sem_down()` and it hits a semaphore whose count is 0, it gets blocked by 
the semaphore, and the thread's TCB will go to the `waitlist` of this certain 
semaphore. The TCB does not go into the `tcb_queue` because it is blocked and 
should not run until getting unblocked. Then, the next thread in 
`tcb_queue` will take over. The `waitlist` is FIFO, because when we unblock a 
thread, we want the first blocked thread to be revived to prevent starvation. 
The way we unblock a thread is simple, we just dequeue its TCB from the 
certain `waitlist` and add it back to `tcb_queue` so that the thread can be 
scheduled to run later. 

#### Phase 4: Preemption

The preemption phase deals with setting up alarms and handling alarms, in order 
for CPU to gain control over the threads. Inside `preempt_stop()`, we decide to
stop raising signals and responding to them, so that if we don't hinder any 
other signal handlers responding to SIGVTALRM, and they could be easily 
reactivated for future use. 

The tester for this phase calls `uthread_start()`, which would start the 
preemption and register the execution flow of the function `hello()` on the main 
thread. Inside the function hello, we create a thread to execute the other 
function `goodbye()`, and get in the while loop. Since the preemption is 
enabled, SIGALRM would be raised, causing the thread executing `hello()` to 
yield. Once the thread yields, we get to the thread executing `goodbye()`, 
execute that, and the function process is killed because of SIGINT. We think 
that this tester might suffice since it clearly demonstrates how differently 
our code should run when preemption is enabled or disabled. Also, we do know
that the code works as expected when we disable preemption.

