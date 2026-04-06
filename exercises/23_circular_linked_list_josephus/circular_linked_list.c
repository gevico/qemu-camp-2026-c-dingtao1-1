#include "circular_linked_list.h"

#include <stdlib.h>

Node *create_circular_list(int n)
{
	if (n <= 0)
		return NULL;

	Node *head = malloc(sizeof(Node));
	if (!head)
		return NULL;
	head->id = 1;
	Node *cur = head;

	for (int i = 2; i <= n; i++) {
		Node *p = malloc(sizeof(Node));
		if (!p) {
			free_list(head);
			return NULL;
		}
		p->id = i;
		cur->next = p;
		cur = p;
	}
	cur->next = head;
	return head;
}

void free_list(Node *head)
{
	if (!head)
		return;

	Node *p = head->next;
	while (p != head) {
		Node *nx = p->next;
		free(p);
		p = nx;
	}
	free(head);
}
