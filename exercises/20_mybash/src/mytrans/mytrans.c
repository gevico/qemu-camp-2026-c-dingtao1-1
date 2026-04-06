#include "myhash.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trim(char *str) {
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

int load_dictionary(const char *filename, HashTable *table,
                    uint64_t *dict_count) {
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

void to_lowercase(char *str) {
  for (; *str; ++str)
    *str = tolower((unsigned char)*str);
}

int __cmd_mytrans(const char* filename) {
  HashTable *table = create_hash_table();
  if (!table) {
    fprintf(stderr, "无法创建哈希表\n");
    return 1;
  }

  printf("=== 哈希表版英语翻译器（支持百万级数据）===\n");
  uint64_t dict_count = 0;
  if (load_dictionary("src/mytrans/dict.txt", table, &dict_count) != 0) {
    fprintf(stderr, "加载词典失败，请确保 dict.txt 存在。\n");
    free_hash_table(table);
    return 1;
  }
  printf("词典加载完成，共计%llu词条。\n", (unsigned long long)dict_count);

  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    perror("无法打开输入文件");
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

      const char *translation = hash_table_lookup(table, w);
      printf("原文: %s\t", w);
      if (translation) {
          printf("翻译: %s\n", translation);
      } else {
          printf("未找到该单词的翻译。\n");
      }

      tok = strtok(NULL, " \t\r\n");
    }
  }

  fclose(file);
  free_hash_table(table);
  return 0;
}
