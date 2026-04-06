#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int parse_url(const char *url)
{
	int err = 0;
	const char *q = strchr(url, '?');
	if (q == NULL) {
		err = -1;
		goto exit;
	}
	q++;

	while (*q != '\0') {
		const char *end = strchr(q, '&');
		const char *eq = strchr(q, '=');
		if (eq == NULL || (end != NULL && eq >= end)) {
			err = -1;
			goto exit;
		}

		size_t key_len = (size_t)(eq - q);
		char key[128];
		if (key_len >= sizeof(key)) {
			err = -1;
			goto exit;
		}
		memcpy(key, q, key_len);
		key[key_len] = '\0';

		const char *val_start = eq + 1;
		const char *val_end = end ? end : q + strlen(q);
		size_t val_len = (size_t)(val_end - val_start);
		char val[512];
		if (val_len >= sizeof(val)) {
			err = -1;
			goto exit;
		}
		memcpy(val, val_start, val_len);
		val[val_len] = '\0';

		printf("key = %s, value = %s\n", key, val);

		if (end == NULL)
			break;
		q = end + 1;
	}

exit:
	return err;
}

int main(void)
{
	const char *test_url = "https://cn.bing.com/search?name=John&age=30&city=New+York";

	printf("Parsing URL: %s\n", test_url);
	printf("Parameters:\n");

	parse_url(test_url);

	return 0;
}
