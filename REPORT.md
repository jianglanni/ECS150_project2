##### Authors: Lan Jiang, Manan Shah

#### Phase 1: Implementing a Queue

In this phase, the queue is implemented using a Linked List data structure. A 
singly linked list is used, since we do not need to traverse the list in the 
reverse order. The queue has a node struct that contains pointers to the current 
next node. The head and tail of the queue are also defined. The queue has been 
implementedas a Linked List since it provides for faster deletion than an array,
and it also provides for dynamic sizes, unlike an array. The datatype has been 
set to void, in order to remove any restrictions from on the type of data that 
can use this queue. As the name suggests, the `queue_create()` is responsible 
for creating the queue. It allocates memory for the queue struct object and
initializes all the struct members.`queue_destroy()`frees up all the memory 
allocated to the queue. `queue_enqueue()` and `queue_dequeue()` enqueues and 
dequeues the `data` to and from the queue respectively. The `queue_delete()` 
deletes an item from the list, specified by `data`. The `queue_length()` returns 
the current size of the queue.

#### Phase 2: Implementing a User Level Thread

For the implementation, we instantiate the TCB struct to create a current thread 
and a main thread. The TCB struct contains a stack pointer, a backup context 
pointer, and a state. The main thread is the default thread, the current thread
switches to the main thread after execution or after yielding. There are 2 queue
objects:`garbage_tcb` and `tcb_queue`, containing the TCBs that have exited and 
that are ready respectively. `uthread_create()` creates the thread by simply 
initializing all the variables, while `thread_start()` is creates the main 
thread and makes it the current thread. In addition to that, we also remove any 
threads that have already exited. It is here that preemption is started and CPU 
starts gaining control of the threads. Before the function returns, we also make 
sure that all the preemption is stopped and all the signal handler and alarm 
configurations are restored. This is important because _______. For blocking a 
thread, we disable the preemption, and make the next thread in the queue as the 
current running thread. After the context is updated, it is important to gain 
control over the CPU again, so we unblock all the signals. A similar operation 
is done while yielding, except we put the current thread in the queue as well, 
so that it can be executed later. The same operation is used in `thread_exit()`
except, we enqueue it to the `garbage_queue` first, and it is deleted when start
will be called.

#### Phase 3: Implementing a Semaphore

The semaphore struct has 2 member variables: `waitlist` and `count`. The `count` 
simulates the internal count of the semaphore, and the `waitlist` is a queue of 
thread TCBs waiting to grab a resource when the no resources are available. We 
used a queue since the priority of dequeuing should be given to queues in order 
of their arrival. Once a thread asks for a resource, if the semaphore count is 0
all the threads asking for the resource is blocked. Once a semaphore is free,
we check the waitlist to see if there are any other threads waiting for the 
resource, in which case the semaphore is assigned to that thread.

#### Phase 4: Preemption

Preemption was done by raising and handling signals of SIGVTALRM type.Preemption
regulates the yielding of the current thread, in order to gain control over the 
CPU.In order to raise alarm signals, we used the object of struct `sigaction` 
and the function `int sigaction()` in order to manage the portability issues 
that are known to arise with `int signal()` function. 
