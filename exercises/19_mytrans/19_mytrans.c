// main.c
#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int main() {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  if (load_dictionary("dict.txt", table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%ld词条。\n", dict_count);

  FILE *file = fopen("text.txt", "r");
  if (file == NULL) {
    fprintf(stderr, "无法打开文件 text.txt。\n");
    free_hash_table(table);
    return 1;
  }

  char line[4096];
  while (fgets(line, sizeof(line), file) != NULL) {
    line[strcspn(line, "\n")] = '\0';

    if (strlen(line) == 0) {
      continue;
    }

    char *tok = strtok(line, " \t\r\n");
    while (tok != NULL) {
      char w[256];
      strncpy(w, tok, sizeof(w) - 1);
      w[sizeof(w) - 1] = '\0';
      trim(w);
      to_lowercase(w);

      char *end = w + strlen(w);
      while (end > w && ispunct((unsigned char)*(end - 1)))
        *--end = '\0';

      char *start = w;
      while (*start && ispunct((unsigned char)*start))
        start++;

      if (*start == '\0') {
        tok = strtok(NULL, " \t\r\n");
        continue;
      }

      if (start != w)
        memmove(w, start, strlen(start) + 1);

      const char *tr = hash_table_lookup(table, w);
      if (tr)
        printf("原文: %s\t翻译: %s\n", w, tr);
      else
        printf("原文: %s\t未找到该单词的翻译。\n", w);

      tok = strtok(NULL, " \t\r\n");
    }
  }

  free_hash_table(table);
  return 0;
}
