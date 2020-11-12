#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

void test_queue_delete(void) {
	int data1 = 0, data2 = 1;
	int *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_delete ***\n");
	q = queue_create();
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);

	queue_delete(q, &data1);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data2);
}

void add1(void* ptr) {
	int* numptr = (int*) ptr;
	*numptr += 1;
}

void test_queue_iterate(void) {
	int data1 = 0, data2 = 1;
	int *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_iterate ***\n");
	q = queue_create();
	queue_enqueue(q, &data1);
	queue_enqueue(q, &data2);

	void (*func)(void*) = &add1;
	queue_iterate(q, func);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(*ptr == 1);
}

	
int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_delete();
	test_queue_iterate();

	return 0;
}
