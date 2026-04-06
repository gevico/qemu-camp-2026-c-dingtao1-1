#include "mywc.h"
#include <stdlib.h>

static int wc_compare(const void *a, const void *b)
{
	const WordCount *const *wa = a;
	const WordCount *const *wb = b;
	return strcmp((*wa)->word, (*wb)->word);
}

// 创建哈希表
WordCount **create_hash_table() {
  WordCount **hash_table = calloc(HASH_SIZE, sizeof(WordCount *));
  return hash_table;
}

// 简单的哈希函数
unsigned int hash(const char *word) {
  unsigned long hash = 5381;
  int c;
  while ((c = *word++))
    hash = ((hash << 5) + hash) + c; // hash * 33 + c
  return hash % HASH_SIZE;
}

// 检查字符是否构成单词的一部分
bool is_valid_word_char(char c) { return isalpha(c) || c == '\''; }

// 转换为小写
char to_lower(char c) { return tolower(c); }

// 添加单词到哈希表
void add_word(WordCount **hash_table, const char *word) {
	unsigned int h = hash(word);
	WordCount *cur = hash_table[h];

	while (cur) {
		if (strcmp(cur->word, word) == 0) {
			cur->count++;
			return;
		}
		cur = cur->next;
	}

	WordCount *node = malloc(sizeof(WordCount));
	if (!node)
		return;
	strncpy(node->word, word, MAX_WORD_LEN - 1);
	node->word[MAX_WORD_LEN - 1] = '\0';
	node->count = 1;
	node->next = hash_table[h];
	hash_table[h] = node;
}

// 打印单词统计结果
void print_word_counts(WordCount **hash_table) {
	printf("Word Count Statistics:\n");
	printf("======================\n");

	WordCount *list[10000];
	size_t n = 0;

	for (unsigned int i = 0; i < HASH_SIZE; i++) {
		for (WordCount *w = hash_table[i]; w; w = w->next) {
			if (n < sizeof(list) / sizeof(list[0]))
				list[n++] = w;
		}
	}

	qsort(list, n, sizeof(list[0]), wc_compare);

	for (size_t i = 0; i < n; i++)
		printf("%-21s%d\n", list[i]->word, list[i]->count);
}

// 释放哈希表内存
void free_hash_table(WordCount **hash_table) {
	for (unsigned int i = 0; i < HASH_SIZE; i++) {
		WordCount *cur = hash_table[i];
		while (cur) {
			WordCount *next = cur->next;
			free(cur);
			cur = next;
		}
		hash_table[i] = NULL;
	}
	free(hash_table);
}

// 处理文件并统计单词
void process_file(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  WordCount **hash_table = create_hash_table();
  char word[MAX_WORD_LEN];
  int word_pos = 0;
  int c;

  while ((c = fgetc(file)) != EOF) {
    if (is_valid_word_char(c)) {
      if (word_pos < MAX_WORD_LEN - 1) {
        word[word_pos++] = to_lower(c);
      }
    } else {
      if (word_pos > 0) {
        word[word_pos] = '\0';
        add_word(hash_table, word);
        word_pos = 0;
      }
    }
  }

  // 处理文件末尾的最后一个单词
  if (word_pos > 0) {
    word[word_pos] = '\0';
    add_word(hash_table, word);
  }

  fclose(file);
  print_word_counts(hash_table);
  free_hash_table(hash_table);
}
