#include "doubly_circular_queue.h"

#include <stdlib.h>

static struct node tailsentinel;
static struct node headsentinel = {0, NULL, &tailsentinel};
static struct node tailsentinel = {0, &headsentinel, NULL};

static link head = &headsentinel;
static link tail = &tailsentinel;

link make_node(int data)
{
	link p = malloc(sizeof(struct node));
	if (!p)
		return NULL;
	p->data = data;
	p->prev = p->next = NULL;
	return p;
}

void free_node(link p)
{
	free(p);
}

link search(int key)
{
	for (link p = head->next; p != tail; p = p->next) {
		if (p->data == key)
			return p;
	}
	return NULL;
}

void insert(link p)
{
	p->next = head->next;
	p->prev = head;
	head->next->prev = p;
	head->next = p;
}

void delete(link p)
{
	p->prev->next = p->next;
	p->next->prev = p->prev;
}

void traverse(void (*visit)(link))
{
	for (link p = head->next; p != tail; p = p->next)
		visit(p);
}

void destroy(void)
{
	while (head->next != tail) {
		link p = head->next;
		delete(p);
		free_node(p);
	}
}
