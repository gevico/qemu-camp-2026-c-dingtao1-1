#include "simple_tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct StackNode {
	TreeNode *tree_node;
	struct StackNode *next;
} StackNode;

static TreeNode *new_node(int v)
{
	TreeNode *n = malloc(sizeof(TreeNode));
	if (!n)
		return NULL;
	n->val = v;
	n->left = n->right = NULL;
	return n;
}

Queue *create_queue(void)
{
	Queue *q = malloc(sizeof(Queue));
	if (!q)
		return NULL;
	q->front = q->rear = NULL;
	return q;
}

void enqueue(Queue *q, TreeNode *tree_node)
{
	QueueNode *qn = malloc(sizeof(QueueNode));
	if (!qn)
		return;
	qn->tree_node = tree_node;
	qn->next = NULL;
	if (q->rear == NULL)
		q->front = q->rear = qn;
	else {
		q->rear->next = qn;
		q->rear = qn;
	}
}

TreeNode *dequeue(Queue *q)
{
	if (q->front == NULL)
		return NULL;
	QueueNode *qn = q->front;
	TreeNode *t = qn->tree_node;
	q->front = qn->next;
	if (q->front == NULL)
		q->rear = NULL;
	free(qn);
	return t;
}

bool is_empty(Queue *q)
{
	return q->front == NULL;
}

void free_queue(Queue *q)
{
	while (!is_empty(q))
		dequeue(q);
	free(q);
}

TreeNode *build_tree_by_level(int *level_order, int size)
{
	if (size <= 0 || level_order == NULL)
		return NULL;
	if (level_order[0] == INT_MIN)
		return NULL;

	Queue *q = create_queue();
	if (!q)
		return NULL;

	TreeNode *root = new_node(level_order[0]);
	if (!root) {
		free_queue(q);
		return NULL;
	}
	enqueue(q, root);

	int idx = 1;
	while (!is_empty(q) && idx < size) {
		TreeNode *node = dequeue(q);
		if (idx < size) {
			if (level_order[idx] != INT_MIN) {
				node->left = new_node(level_order[idx]);
				enqueue(q, node->left);
			}
			idx++;
		}
		if (idx < size) {
			if (level_order[idx] != INT_MIN) {
				node->right = new_node(level_order[idx]);
				enqueue(q, node->right);
			}
			idx++;
		}
	}

	free_queue(q);
	return root;
}

void preorder_traversal(TreeNode *root)
{
	if (root == NULL)
		return;
	printf("%d ", root->val);
	preorder_traversal(root->left);
	preorder_traversal(root->right);
}

void preorder_traversal_iterative(TreeNode *root)
{
	if (root == NULL)
		return;

	TreeNode **stack = malloc(sizeof(TreeNode *) * 1024);
	if (!stack)
		return;
	int sp = 0;
	stack[sp++] = root;

	while (sp > 0) {
		TreeNode *n = stack[--sp];
		printf("%d ", n->val);
		if (n->right)
			stack[sp++] = n->right;
		if (n->left)
			stack[sp++] = n->left;
	}

	free(stack);
}

void free_tree(TreeNode *root)
{
	if (root == NULL)
		return;
	free_tree(root->left);
	free_tree(root->right);
	free(root);
}
