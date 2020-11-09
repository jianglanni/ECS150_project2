#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

static sigset_t *set;
static void handler(int sig){
    printf("Hello %d\n", sig);
    void preempt_disable(void)
{
    sigemptyset(set);
    sigaddset(set, SIGVTALRM);
    sigprocmask(SIG_BLOCK, set, NULL);
}

void preempt_enable(void)
{
	sigemptyset(set);
	sigaddset(set, SIGVTALRM);
	sigprocmask(SIG_UNBLOCK, set, NULL);
}

void preempt_start(void)
{
    struct sigaction new, old;
    struct itimerval old_it, new_it;

    sa.sa_handler = handler;
    sigaction(SIGVTALRM, &new, &old)
    unsigned int alarm(unsigned int secs){

    }
}

void preempt_stop(void)
{
	/* TODO Phase 4 */
}

