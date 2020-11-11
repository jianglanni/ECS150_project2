#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

//We are using linked list for the queue.

struct node {
	void* content;
	struct node* next;
};

struct queue {
	int size;
	struct node* head;
	struct node* tail;
};

queue_t queue_create(void)
{
	queue_t new_queue;
	new_queue = (queue_t) malloc(sizeof(struct queue));

	if (new_queue != NULL) {
		new_queue -> size = 0;
		new_queue -> head = NULL;
		new_queue -> tail = NULL;
	}

	return new_queue;
}

int queue_destroy(queue_t queue)
{
	if (queue == NULL)
		return -1;

	while (queue -> head != NULL) {
		struct node* next = queue -> head -> next;
		free(queue -> head);
		queue -> head = next;
	}

	free(queue);

	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL)
		return -1;

	struct node* new_node;
	new_node = (struct node*) malloc(sizeof(struct node));

	if (new_node == NULL)
		return -1;

	new_node -> content = data;
	new_node -> next = NULL;

	if (queue -> size != 0) {
		queue -> tail -> next = new_node;
	} else {
		queue -> head = new_node;
	}

	queue -> tail = new_node;
	++queue -> size;

	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL || !queue -> size)
		return -1;

	struct node* head;
	head = queue -> head;
	*data = head -> content;
	queue -> head = head -> next;
	free(head);
	--queue -> size;

	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if (queue == NULL || !queue -> size)
		return -1;

	struct node* current = queue -> head;

	if (current -> content == data) {
		queue -> head = current -> next;
		free(current);
		--queue -> size;
		return 0;
	}

	while (current -> next != NULL) {
		if (current -> next -> content == data) {
			struct node* target = current -> next;
			current -> next = target -> next;
			free(target);
			--queue -> size;
			return 0;
		}
		current = current -> next;
	}

	return -1;		
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if (queue == NULL || func == NULL)
		return -1;

	struct node* current = queue -> head;

	while (current != NULL) {
		(*func)(current -> content);
		current = current -> next;
	}

	return 0;
}

int queue_length(queue_t queue)
{
	if (queue == NULL)
		return -1;

	return queue -> size;
}

