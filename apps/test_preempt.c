#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "uthread.h"
#include "private.h"

void goodbye(void *arg) {
	printf("See you!\n");
	raise(SIGINT);
}

void hello(void *arg)
{
	uthread_create(goodbye, NULL);
	while(1) {}
}

int main(void)
{
	uthread_start(hello, NULL);
	return 0;
}
