#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int *buf;
	size_t capacity;
	size_t head;
	size_t tail;
	size_t count;
	pthread_mutex_t mtx;
	pthread_cond_t not_full;
	pthread_cond_t not_empty;
} ring_buffer_t;

static int rb_init(ring_buffer_t *rb, size_t capacity)
{
	rb->buf = malloc(capacity * sizeof(int));
	if (!rb->buf)
		return -1;
	rb->capacity = capacity;
	rb->head = rb->tail = rb->count = 0;
	if (pthread_mutex_init(&rb->mtx, NULL) != 0) {
		free(rb->buf);
		return -1;
	}
	if (pthread_cond_init(&rb->not_full, NULL) != 0) {
		pthread_mutex_destroy(&rb->mtx);
		free(rb->buf);
		return -1;
	}
	if (pthread_cond_init(&rb->not_empty, NULL) != 0) {
		pthread_cond_destroy(&rb->not_full);
		pthread_mutex_destroy(&rb->mtx);
		free(rb->buf);
		return -1;
	}
	return 0;
}

static void rb_destroy(ring_buffer_t *rb)
{
	free(rb->buf);
	pthread_mutex_destroy(&rb->mtx);
	pthread_cond_destroy(&rb->not_full);
	pthread_cond_destroy(&rb->not_empty);
}

static void rb_push(ring_buffer_t *rb, int val)
{
	pthread_mutex_lock(&rb->mtx);
	while (rb->count == rb->capacity)
		pthread_cond_wait(&rb->not_full, &rb->mtx);
	rb->buf[rb->tail] = val;
	rb->tail = (rb->tail + 1U) % rb->capacity;
	rb->count++;
	pthread_cond_signal(&rb->not_empty);
	pthread_mutex_unlock(&rb->mtx);
}

static int rb_pop(ring_buffer_t *rb, int *out)
{
	pthread_mutex_lock(&rb->mtx);
	while (rb->count == 0)
		pthread_cond_wait(&rb->not_empty, &rb->mtx);
	*out = rb->buf[rb->head];
	rb->head = (rb->head + 1U) % rb->capacity;
	rb->count--;
	pthread_cond_signal(&rb->not_full);
	pthread_mutex_unlock(&rb->mtx);
	return 0;
}

typedef struct {
	ring_buffer_t *rb;
	const int *data;
	size_t n;
} producer_arg_t;

typedef struct {
	ring_buffer_t *rb;
	size_t n;
} consumer_arg_t;

static void *producer(void *arg)
{
	producer_arg_t *pa = arg;
	for (size_t i = 0; i < pa->n; i++)
		rb_push(pa->rb, pa->data[i]);
	return NULL;
}

static void *consumer(void *arg)
{
	consumer_arg_t *ca = arg;
	for (size_t i = 0; i < ca->n; i++) {
		int v;
		rb_pop(ca->rb, &v);
		if (i)
			printf(",");
		printf("%d", v);
	}
	printf("\n");
	return NULL;
}

int main(void)
{
	setvbuf(stdout, NULL, _IOLBF, 0);

	ring_buffer_t rb;
	if (rb_init(&rb, 5) != 0) {
		fprintf(stderr, "ring buffer init failed\n");
		return 1;
	}

	const int data[] = {1, 2, 3, 4, 5, 6};
	const size_t n = sizeof(data) / sizeof(data[0]);

	pthread_t th_p, th_c;
	producer_arg_t pa = {.rb = &rb, .data = data, .n = n};
	consumer_arg_t ca = {.rb = &rb, .n = n};

	if (pthread_create(&th_c, NULL, consumer, &ca) != 0) {
		rb_destroy(&rb);
		return 1;
	}
	if (pthread_create(&th_p, NULL, producer, &pa) != 0) {
		rb_destroy(&rb);
		return 1;
	}

	pthread_join(th_p, NULL);
	pthread_join(th_c, NULL);

	rb_destroy(&rb);
	return 0;
}
