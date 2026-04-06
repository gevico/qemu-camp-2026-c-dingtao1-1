// mytrans.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str)
{
	if (str == NULL || *str == '\0')
		return;

	char *start = str;
	while (*start && isspace((unsigned char)*start))
		start++;
	if (start != str)
		memmove(str, start, strlen(start) + 1);

	size_t len = strlen(str);
	while (len > 0 && isspace((unsigned char)str[len - 1])) {
		str[len - 1] = '\0';
		len--;
	}
}

int load_dictionary(const char *filename, HashTable *table, uint64_t *dict_count)
{
	FILE *file = fopen(filename, "r");
	if (!file) {
		perror("无法打开词典文件");
		return -1;
	}

	char line[1024];
	char current_word[100] = {0};

	*dict_count = 0;

	while (fgets(line, sizeof(line), file) != NULL) {
		size_t len = strlen(line);
		while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
			line[len - 1] = '\0';
			len--;
		}

		if (line[0] == '#') {
			strncpy(current_word, line + 1, sizeof(current_word) - 1);
			current_word[sizeof(current_word) - 1] = '\0';
			trim(current_word);
		} else if (strncmp(line, "Trans:", 6) == 0) {
			if (current_word[0] != '\0') {
				hash_table_insert(table, current_word, line + 6);
				(*dict_count)++;
			}
		}
	}

	fclose(file);
	return 0;
}
