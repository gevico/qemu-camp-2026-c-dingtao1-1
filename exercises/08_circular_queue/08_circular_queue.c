#include <stdio.h>
#include <stdbool.h>

#define MAX_PEOPLE 50

typedef struct {
	int id;
} People;

typedef struct {
	People data[MAX_PEOPLE];
	int head;
	int tail;
	int count;
} Queue;

static void q_init(Queue *q)
{
	q->head = 0;
	q->tail = 0;
	q->count = 0;
}

static void q_enqueue(Queue *q, People p)
{
	q->data[q->tail] = p;
	q->tail = (q->tail + 1) % MAX_PEOPLE;
	q->count++;
}

static People q_dequeue(Queue *q)
{
	People p = q->data[q->head];
	q->head = (q->head + 1) % MAX_PEOPLE;
	q->count--;
	return p;
}

int main(void)
{
	Queue q;
	int total_people = 50;
	int report_interval = 5;

	(void)total_people;
	q_init(&q);
	for (int i = 1; i <= total_people; i++)
		q_enqueue(&q, (People){i});

	while (q.count > 1) {
		for (int k = 0; k < report_interval - 1; k++) {
			People p = q_dequeue(&q);
			q_enqueue(&q, p);
		}
		People out = q_dequeue(&q);
		printf("淘汰: %d\n", out.id);
	}

	printf("最后剩下的人是: %d\n", q.data[q.head].id);
	(void)report_interval;

	return 0;
}
