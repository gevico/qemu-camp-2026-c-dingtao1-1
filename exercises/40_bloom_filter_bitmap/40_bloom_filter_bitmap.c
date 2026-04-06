#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned char *bits;
	size_t m;
} Bloom;

static Bloom *bloom_create(size_t m)
{
	Bloom *bf = malloc(sizeof(Bloom));
	if (!bf)
		return NULL;
	bf->m = m;
	size_t bytes = (m + 7U) / 8U;
	bf->bits = calloc(bytes, 1);
	if (!bf->bits) {
		free(bf);
		return NULL;
	}
	return bf;
}

static void bloom_free(Bloom *bf)
{
	if (!bf)
		return;
	free(bf->bits);
	free(bf);
}

static void set_bit(unsigned char *bm, size_t idx)
{
	size_t byte = idx / 8U;
	unsigned off = (unsigned)(idx % 8U);
	bm[byte] |= (unsigned char)(1u << off);
}

static int test_bit(const unsigned char *bm, size_t idx)
{
	size_t byte = idx / 8U;
	unsigned off = (unsigned)(idx % 8U);
	return (bm[byte] >> off) & 1;
}

static size_t hash_k(const char *s, size_t m, int k)
{
	size_t sum = 0;
	for (const unsigned char *p = (const unsigned char *)s; *p; p++)
		sum += (size_t)(*p) * (size_t)k;
	return sum % m;
}

static void bloom_add(Bloom *bf, const char *s)
{
	for (int k = 1; k <= 3; k++) {
		size_t idx = hash_k(s, bf->m, k);
		set_bit(bf->bits, idx);
	}
}

static int bloom_maybe_contains(Bloom *bf, const char *s)
{
	for (int k = 1; k <= 3; k++) {
		size_t idx = hash_k(s, bf->m, k);
		if (!test_bit(bf->bits, idx))
			return 0;
	}
	return 1;
}

int main(void)
{
	const size_t m = 100;
	Bloom *bf = bloom_create(m);
	if (!bf) {
		fprintf(stderr, "bloom create failed\n");
		return 1;
	}

	bloom_add(bf, "apple");
	bloom_add(bf, "banana");

	int apple = bloom_maybe_contains(bf, "apple");
	int grape = bloom_maybe_contains(bf, "grape");

	printf("apple exists: %d\n", apple);
	printf("grape exists: %d\n", grape);

	bloom_free(bf);
	return 0;
}
