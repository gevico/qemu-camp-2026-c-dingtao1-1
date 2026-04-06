#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LRUNode {
	int key;
	int value;
	struct LRUNode *prev;
	struct LRUNode *next;
} LRUNode;

typedef struct HashEntry {
	int key;
	LRUNode *node;
	struct HashEntry *next;
} HashEntry;

typedef struct {
	int capacity;
	int size;
	LRUNode *head;
	LRUNode *tail;
	size_t bucket_count;
	HashEntry **buckets;
} LRUCache;

static size_t bucket_idx(LRUCache *c, int key)
{
	return (size_t)((unsigned)key % (unsigned)c->bucket_count);
}

static HashEntry *hash_find(LRUCache *c, int key, HashEntry ***pprev_next)
{
	size_t b = bucket_idx(c, key);
	HashEntry **pp = &c->buckets[b];
	while (*pp) {
		if ((*pp)->key == key) {
			if (pprev_next)
				*pprev_next = pp;
			return *pp;
		}
		pp = &(*pp)->next;
	}
	if (pprev_next)
		*pprev_next = NULL;
	return NULL;
}

static void hash_remove(LRUCache *c, int key)
{
	size_t b = bucket_idx(c, key);
	HashEntry **pp = &c->buckets[b];
	while (*pp) {
		if ((*pp)->key == key) {
			HashEntry *t = *pp;
			*pp = t->next;
			free(t);
			return;
		}
		pp = &(*pp)->next;
	}
}

static void hash_put(LRUCache *c, int key, LRUNode *node)
{
	size_t b = bucket_idx(c, key);
	HashEntry *e = malloc(sizeof(HashEntry));
	e->key = key;
	e->node = node;
	e->next = c->buckets[b];
	c->buckets[b] = e;
}

static void list_add_to_head(LRUCache *c, LRUNode *node)
{
	node->prev = NULL;
	node->next = c->head;
	if (c->head)
		c->head->prev = node;
	else
		c->tail = node;
	c->head = node;
}

static void list_remove(LRUCache *c, LRUNode *node)
{
	if (node->prev)
		node->prev->next = node->next;
	else
		c->head = node->next;
	if (node->next)
		node->next->prev = node->prev;
	else
		c->tail = node->prev;
	node->prev = node->next = NULL;
}

static void list_move_to_head(LRUCache *c, LRUNode *node)
{
	if (c->head == node)
		return;
	list_remove(c, node);
	list_add_to_head(c, node);
}

static LRUNode *list_pop_tail(LRUCache *c)
{
	if (!c->tail)
		return NULL;
	LRUNode *t = c->tail;
	list_remove(c, t);
	return t;
}

static LRUCache *lru_create(int capacity)
{
	LRUCache *c = malloc(sizeof(LRUCache));
	if (!c)
		return NULL;
	c->capacity = capacity;
	c->size = 0;
	c->head = c->tail = NULL;
	c->bucket_count = 1024;
	c->buckets = calloc(c->bucket_count, sizeof(HashEntry *));
	if (!c->buckets) {
		free(c);
		return NULL;
	}
	return c;
}

static void lru_free(LRUCache *c)
{
	if (!c)
		return;
	LRUNode *p = c->head;
	while (p) {
		LRUNode *n = p->next;
		free(p);
		p = n;
	}
	for (size_t i = 0; i < c->bucket_count; i++) {
		HashEntry *e = c->buckets[i];
		while (e) {
			HashEntry *t = e;
			e = e->next;
			free(t);
		}
	}
	free(c->buckets);
	free(c);
}

static int lru_get(LRUCache *c, int key, int *out_value)
{
	HashEntry *he = hash_find(c, key, NULL);
	if (!he)
		return 0;
	*out_value = he->node->value;
	list_move_to_head(c, he->node);
	return 1;
}

static void lru_put(LRUCache *c, int key, int value)
{
	HashEntry *he = hash_find(c, key, NULL);
	if (he) {
		he->node->value = value;
		list_move_to_head(c, he->node);
		return;
	}

	if (c->size >= c->capacity) {
		LRUNode *victim = c->tail;
		if (!victim)
			return;
		int vk = victim->key;
		list_remove(c, victim);
		hash_remove(c, vk);
		victim->key = key;
		victim->value = value;
		list_add_to_head(c, victim);
		hash_put(c, key, victim);
		return;
	}

	LRUNode *n = malloc(sizeof(LRUNode));
	n->key = key;
	n->value = value;
	n->prev = n->next = NULL;
	list_add_to_head(c, n);
	hash_put(c, key, n);
	c->size++;
}

static void lru_print(LRUCache *c)
{
	LRUNode *p = c->head;
	int first = 1;
	while (p) {
		if (!first)
			printf(", ");
		first = 0;
		printf("%d:%d", p->key, p->value);
		p = p->next;
	}
	printf("\n");
}

int main(void)
{
	LRUCache *c = lru_create(3);
	if (!c) {
		fprintf(stderr, "创建 LRU 失败\n");
		return 1;
	}

	lru_put(c, 1, 1);
	lru_put(c, 2, 2);
	lru_put(c, 3, 3);
	lru_put(c, 4, 4);

	int val;
	if (lru_get(c, 2, &val))
		(void)val;

	lru_put(c, 5, 5);

	lru_print(c);

	lru_free(c);
	return 0;
}
