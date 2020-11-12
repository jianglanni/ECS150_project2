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

void preempt_disable(void)
{
	sigset_t ss;
	sigemptyset(&ss);
	sigaddset(&ss, SIGVTALRM);
	sigprocmask(SIG_BLOCK, &ss, NULL);
}

void preempt_enable(void)
{
	sigset_t ss;
	sigemptyset(&ss);
	sigaddset(&ss, SIGVTALRM);
	sigprocmask(SIG_UNBLOCK, &ss, NULL);
}

void sig_handler(int signum) {
	uthread_yield();
}

void preempt_start(void)
{
	struct sigaction sa;
	sa.sa_handler = sig_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);
	
	//Set the alarm
	struct itimerval itv;
    	itv.it_interval.tv_sec = 0; //10ms
    	itv.it_interval.tv_usec = 1000000 / HZ;
    	itv.it_value.tv_sec = 0;
    	itv.it_value.tv_usec = 1000000 / HZ;
    	setitimer(ITIMER_VIRTUAL, &itv, NULL);
}

void preempt_stop(void)
{
	//Stop the alarm
	setitimer(ITIMER_VIRTUAL, NULL, NULL);
	
	//Stop reponding to alarms
	struct sigaction sa;
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGVTALRM, &sa, NULL);
}

