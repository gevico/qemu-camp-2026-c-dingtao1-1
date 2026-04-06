#include "singly_linked_list.h"

#include <stdio.h>
#include <stdlib.h>

static link head = NULL;

link make_node(unsigned char item)
{
	link p = malloc(sizeof(struct node));
	if (!p)
		return NULL;
	p->item = item;
	p->next = NULL;
	return p;
}

link search(unsigned char key)
{
	for (link p = head; p; p = p->next) {
		if (p->item == key)
			return p;
	}
	return NULL;
}

void insert(link p)
{
	if (!p)
		return;
	p->next = head;
	head = p;
}

void delete(link p)
{
	if (!p || !head)
		return;
	if (head == p) {
		head = head->next;
		return;
	}
	for (link t = head; t && t->next; t = t->next) {
		if (t->next == p) {
			t->next = p->next;
			return;
		}
	}
}

void traverse(void (*visit)(link))
{
	for (link p = head; p; p = p->next)
		visit(p);
}

void destroy(void)
{
	while (head) {
		link t = head;
		head = head->next;
		free(t);
	}
}

void push(link p)
{
	insert(p);
}

link pop(void)
{
	if (!head)
		return NULL;
	link p = head;
	head = head->next;
	p->next = NULL;
	return p;
}

void free_list(link list_head)
{
	while (list_head) {
		link t = list_head;
		list_head = list_head->next;
		free(t);
	}
}
