#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024

int parse_replace_command(const char *cmd, char **old_str, char **new_str)
{
	if (cmd[0] != 's' || cmd[1] != '/') {
		return -1;
	}

	const char *p = cmd + 2;
	const char *slash1 = strchr(p, '/');
	if (slash1 == NULL)
		return -1;

	const char *slash2 = strchr(slash1 + 1, '/');
	if (slash2 == NULL)
		return -1;

	size_t old_len = (size_t)(slash1 - p);
	char *old = malloc(old_len + 1);
	if (old == NULL)
		return -1;
	memcpy(old, p, old_len);
	old[old_len] = '\0';

	size_t new_len = (size_t)(slash2 - (slash1 + 1));
	char *nw = malloc(new_len + 1);
	if (nw == NULL) {
		free(old);
		return -1;
	}
	memcpy(nw, slash1 + 1, new_len);
	nw[new_len] = '\0';

	*old_str = old;
	*new_str = nw;

	return 0;
}

void replace_first_occurrence(char *str, const char *old, const char *new)
{
	char *p = strstr(str, old);
	if (p == NULL)
		return;

	size_t old_len = strlen(old);
	size_t new_len = strlen(new);
	size_t tail_len = strlen(p + old_len);

	if (new_len <= old_len) {
		memcpy(p, new, new_len);
		memmove(p + new_len, p + old_len, tail_len + 1);
	} else {
		size_t diff = new_len - old_len;
		memmove(p + new_len, p + old_len, tail_len + 1);
		memcpy(p, new, new_len);
		(void)diff;
	}
}

int main(int argc, char *argv[])
{
	const char *replcae_rules = "s/unix/linux/";

	char line[MAX_LINE_LENGTH] = {"unix is opensource. unix is free os."};

	char *old_str = NULL;
	char *new_str = NULL;

	if (parse_replace_command(replcae_rules, &old_str, &new_str) != 0) {
		fprintf(stderr, "Invalid replace command format. Use 's/old/new/'\n");
		return 1;
	}

	replace_first_occurrence(line, old_str, new_str);
	fputs(line, stdout);

	free(old_str);
	free(new_str);
	(void)argc;
	(void)argv;
	return 0;
}
